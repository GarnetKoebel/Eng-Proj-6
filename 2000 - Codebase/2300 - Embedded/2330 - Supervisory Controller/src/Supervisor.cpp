#include "../include/CAN.h"
#include "../include/Supervisor.h"
#include "../include/linkedList.h"
#include <libpcan.h>


// CAN IDs
#define SC_ID  0x100
#define EC_ID  0x101
#define CC_ID  0x200
#define FC1_ID 0x201
#define FC2_ID 0x202
#define FC3_ID 0x203

// Command Definitions (As Per Protocol Spec)
// Supervisor Commands (issued by supervisor)
#define SC_ENABLE      0x00 // Supervisor enables elevator
#define SC_DISABLE     0x01 // Supervisor disables elevator
#define SC_FLOOR_REQ_1 0x05 // Supervisor requests floor 1
#define SC_FLOOR_REQ_2 0x06 // Supervisor requests floor 2
#define SC_FLOOR_REQ_3 0x07 // Supervisor requests floor 3

// Elevator Controller Commands (Received by all)
#define EC_STATUS_DEEN 0x00 // Elevator reports it is disabled
#define EC_STATUS_EN   0x01 // Elevator reports it is enabled
#define EC_POS_MOV     0x00 // Elevator reports it is moving
#define EC_POS_1       0x05 // Elevator reports is it at floor 1
#define EC_POS_2       0x06 // Elevator reports it is at floor 2
#define EC_POS_3       0x07 // Elevator reports it is at floor 3

// Car Controller Commands (Received by supervisor)
#define CC_FLOOR_REQ_1 0x05 // Car requests floor 1
#define CC_FLOOR_REQ_2 0x06 // Car requests floor 2
#define CC_FLOOR_REQ_3 0x07 // Car requests floor 3

// Floor Controller Command (Received by supervisor)
#define FC_FLOOR_REQ   0x01 // Floor controller requests car

/*
	Queue priority theory of operation
	1. command comes in and is put into queue
	2. supervisor sorts queue by priority based on state (e.g service floors above if already moving up)
	3. supervisor executes first command in queue


	Operation V2
	1. all commands are stored in a buffer (array)
	2. a new command comes in a flag is set
	3. when the supervisor goes to process a command it first checks the flag, if the flag is set then it sorts the buffer
	   the method of sorting depends on the current car state. The buffer is then de-duplicated
	4. Once the buffer is sorted and de-duplicated the supervisor runs the command in buffer slot 0 and removes that command
	   from the buffer by shuffling everything else down.
	5. repeat

	NOTE: It may be more efficient to use a linked list queue for this
*/

enum currentFloorEnum {
    MOV,
    F1,
    F2,
    F3

};
enum currentFloorEnum currentFloor;

enum carStateEnum { // Supervisor makes decisions based on car state
	STOP,		// Initial state / e-stop state
	FLOOR1,		// Arrives at floor 1
	FLOOR2,		// Arrives at floor 2
	FLOOR3,  	// Arrives at floor 3
	DESCENDING,	// Going down
	ASCENDING 	// Going up
};
enum carStateEnum carState;

enum msgTypeEnum { // NOTE: Seperating calls made from inside the car and outside may be a good idea for future expansion
    CAR_MOV,
    FLOOR1_CALL,
    FLOOR2_CALL,
    FLOOR3_CALL,
    CAR_POS_F1,
    CAR_POS_F2,
    CAR_POS_F3,
    EC_DEEN,
    EC_EN

};
enum msgTypeEnum cmdType;

int newCommand = 0; // flag tracking if a new message has arrived
int queueLen = 0;
linkedList* commandQueue = createList(); // create empty list

int deDuplicateCommandQueue(void) {
    return 0;
}

int receiveMsg(void) { // wait for message to come in
    TPCANMsg RxMsg;
	RxMsg = pcanRx(); // wait for next CAN msg
    unsigned int msgId = RxMsg.ID;
    unsigned char msgData = RxMsg.DATA[0];
    static char ecBaseStr[] = "EC Status:";
    
    if (msgData == EC_ID && msgData == EC_STATUS_DEEN && RxMsg.LEN != 0x04) {
        cmdType = EC_DEEN;

	} else if (msgData == EC_ID && msgData == EC_STATUS_EN && RxMsg.LEN != 0x04) {
        cmdType = EC_EN;

	} else if (msgData == EC_ID && msgData == EC_POS_MOV && RxMsg.LEN != 0x04) {
        currentFloor = MOV;
        printf("%s Moving\n", ecBaseStr);
        return 0;

	} else if (msgData == EC_ID && msgData == EC_POS_1 && RxMsg.LEN != 0x04) { // Elevator at floor 1 
        currentFloor = F1;
        printf("%s Floor 1\n", ecBaseStr);
        return 0;

	} else if (msgData == EC_ID && msgData == EC_POS_2 && RxMsg.LEN != 0x04) { // Elevator at floor 2
        currentFloor = F2;
        printf("%s Floor 2\n", ecBaseStr);
        return 0;

	} else if (msgData == EC_ID && msgData == EC_POS_3 && RxMsg.LEN != 0x04) { // Elevator at floor 3
        currentFloor = F3;
        printf("%s Floor 3\n", ecBaseStr);
        return 0;

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_1 && RxMsg.LEN != 0x04) {
		cmdType = FLOOR1_CALL;

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_2 && RxMsg.LEN != 0x04) {
        cmdType = FLOOR2_CALL;

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_3 && RxMsg.LEN != 0x04) {
        cmdType = FLOOR3_CALL;

	} else if (msgData == FC1_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
        cmdType = FLOOR1_CALL;
			
	} else if (msgData == FC2_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
        cmdType = FLOOR2_CALL;
			
	} else if (msgData == FC3_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
        cmdType = FLOOR3_CALL;
			
	}
    
    if(addElement(cmdType, &RxMsg, &commandQueue, 0, sizeof(TPCANMsg)) == 0) { // always add to head since command execution will determine order of execution
        newCommand = 1; // set new command flag
        return 0;
    } else {
        return 1;
    }
    

}

static void executeCommand(int cmdType) {
   	static char ecBaseStr[] = "EC Status:";
	static char ccBaseStr[] = "CC Status:";
	static char fcBaseStr[] = "FC Status:";

	if (cmdType == EC_DEEN) {
		printf("%s Disabled\n", ecBaseStr);

	} else if (cmdType == EC_EN) {
		printf("%s Enabled\n", ecBaseStr);

    } else if (cmdType == FLOOR1_CALL) {
        if (FLOOR1_CALL > currentFloor) {carState = ASCENDING;}
        else if (FLOOR1_CALL < currentFloor) {carState = DESCENDING;}
        else {carState = (enum carStateEnum)currentFloor;}
		printf("%s Requested at Floor 1\n", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_1);

	} else if (cmdType == FLOOR2_CALL) {
        if (FLOOR2_CALL > currentFloor) {carState = ASCENDING;}
        else if (FLOOR2_CALL < currentFloor) {carState = DESCENDING;}
        else {carState = (enum carStateEnum)currentFloor;}
		printf("%s Requested at Floor 2\n", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_2);

	} else if (cmdType == FLOOR3_CALL) {
        if (FLOOR3_CALL > currentFloor) {carState = ASCENDING;}
        else if (FLOOR3_CALL < currentFloor) {carState = DESCENDING;}
        else {carState = (enum carStateEnum)currentFloor;}
		printf("%s Requested at Floor 3\n", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_3);

    }
}

int processMsg(void) { // determine command message to execute based on car state.
	/*
	 *	Function Name: 				
	 *	Function Type: 				
	 *	Function Inputs: 			
	 *	Function Outputs: 		
	 *	Error Handling:				
	 *	Function Description:	
	 *
	*/
    
    linkedList* tmp = createList();

    // State Machine Control Logic
	if (carState == STOP) {


	} else if (carState == FLOOR1 || carState == FLOOR2 || carState == FLOOR3) { // in this state everything except an e-stop is equal
       tmp = findByIndex(commandQueue, 0); // get head of command queue
       executeCommand(tmp->dataInt); // dataInt holds command type 
       removeElement(&commandQueue, 0); // delete head element
       return 0;

	} else if (carState == DESCENDING) { // in this state floor calls below CURRENT_FLOOR are highest priority
        if (currentFloor == 3) {
            tmp = findFirstByID(commandQueue, currentFloor - 1);
            if (tmp == NULL) {
                tmp = findFirstByID(commandQueue, currentFloor - 2);
                if (tmp == NULL) { // no down calls
                    printf("No Down\n")
                    carState = (enum carStateEnum)currentFloor;
                    return 0;
                }
            }

        } else if (currentFloor == 2) {
            // execute first command with a floor less than current floor
            
            tmp = findFirstByID(commandQueue, currentFloor - 1);
            if (tmp == NULL) {
                carState = (enum carStateEnum)currentFloor; // no down calls
                return 0;
            }

        } else if (currentFloor == 1) { // already at lowest floor this state should not be possible
            carState = (enum carStateEnum)currentFloor;

        } else { // elevator moving do nothing


        }
        


	} else if (carState == ASCENDING ) { // in this state floor calls above CURRENT_FLOOR are highest priority
        // execute first command with a floor greater than current floor
        if (currentFloor == 1) {
            tmp = findFirstByID(commandQueue, currentFloor + 1);
            if (tmp == NULL) {
                tmp = findFirstByID(commandQueue, currentFloor + 2);
                if (tmp == NULL) { // no up calls
                    carState = (enum carStateEnum)currentFloor;
                    return 0;
                }
            }

        } else if (currentFloor == 2) {
            // execute first command with a floor less than current floor
            
            tmp = findFirstByID(commandQueue, currentFloor + 1);
            if (tmp == NULL) {
                carState = (enum carStateEnum)currentFloor; // no up calls
                return 0;
            }

        } else if (currentFloor == 3) { // already at lowest floor this state should not be possible
            carState = (enum carStateEnum)currentFloor;

        } else { // elevator moving do nothing


        }
	}
    // execute command for ascending and descending if/else chains
    if (tmp != NULL) {
        executeCommand(tmp->dataInt); 
        int pos = getElementPosById(commandQueue, tmp->dataInt); // remove element after command execution
        removeElement(&commandQueue, pos); 

    }
    return 0;

	// static char ecBaseStr[] = "EC Status:";
	// static char ccBaseStr[] = "CC Status:";
	// static char fcBaseStr[] = "FC Status:";

	// Decide whether and what to transmit depending on sender and msg content
	// if (msgData == EC_ID && msgData == EC_STATUS_DEEN && RxMsg.LEN != 0x04) {
	// 	printf("%s Disabled", ecBaseStr);

	// } else if (msgData == EC_ID && msgData == EC_STATUS_EN && RxMsg.LEN != 0x04) {
	// 	printf("%s Enabled", ecBaseStr);

	// } else if (msgData == EC_ID && msgData == EC_POS_MOV && RxMsg.LEN != 0x04) {
	// 	printf("%s Moving", ecBaseStr);

	// } else if (msgData == EC_ID && msgData == EC_POS_1 && RxMsg.LEN != 0x04) { // Elevator at floor 1 
	// 	printf("%s Floor 1", ecBaseStr);

	// } else if (msgData == EC_ID && msgData == EC_POS_2 && RxMsg.LEN != 0x04) { // Elevator at floor 2
	// 	printf("%s Floor 2", ecBaseStr);

	// } else if (msgData == EC_ID && msgData == EC_POS_3 && RxMsg.LEN != 0x04) { // Elevator at floor 3
	// 	printf("%s Floor 3", ecBaseStr);

	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_1 && RxMsg.LEN != 0x04) {
	// 	printf("%s Request Floor 1", ccBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_1);

	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_2 && RxMsg.LEN != 0x04) {
	// 	printf("%s Request Floor 2", ccBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_2);

	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_3 && RxMsg.LEN != 0x04) {
	// 	printf("%s Request Floor 3", ccBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_3);

	// } else if (msgData == FC1_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
	// 	printf("%s Floor 1 Requests Car\n", fcBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_1);
			
	// } else if (msgData == FC2_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
	// 	printf("%s Floor 2 Requests Car\n", fcBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_2);
			
	// } else if (msgData == FC3_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
	// 	printf("%s Floor 3 Requests Car\n", fcBaseStr);
	// 	pcanTx(SC_ID, SC_FLOOR_REQ_3);
			
	// }
}
