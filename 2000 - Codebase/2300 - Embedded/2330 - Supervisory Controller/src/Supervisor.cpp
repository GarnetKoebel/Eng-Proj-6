#include "../include/CAN.h"
#include "../include/Supervisor.h"
#include "../include/linkedList.h"
#include "../include/databaseFunctions.h"
#include <libpcan.h>

#include <unistd.h>
#include <fcntl.h>

// Defines
#define PCAN_RECEIVE_QUEUE_EMPTY 0x00020U // Receive queue is empty
#define PCAN_NO_ERROR 0x00000U            // No error

// CAN IDs
#define SC_ID 0x100
#define EC_ID 0x101
#define CC_ID 0x200
#define FC1_ID 0x201
#define FC2_ID 0x202
#define FC3_ID 0x203

// Command Definitions (As Per Protocol Spec)
// Supervisor Commands (issued by supervisor)
#define SC_ENABLE 0x00      // Supervisor enables elevator
#define SC_DISABLE 0x01     // Supervisor disables elevator
#define SC_FLOOR_REQ_1 0x05 // Supervisor requests floor 1
#define SC_FLOOR_REQ_2 0x06 // Supervisor requests floor 2
#define SC_FLOOR_REQ_3 0x07 // Supervisor requests floor 3

// Elevator Controller Commands (Received by all)
#define EC_STATUS_DEEN 0x00 // Elevator reports it is disabled
#define EC_STATUS_EN 0x01   // Elevator reports it is enabled
#define EC_POS_MOV 0x00     // Elevator reports it is moving
#define EC_POS_1 0x05       // Elevator reports is it at floor 1
#define EC_POS_2 0x06       // Elevator reports it is at floor 2
#define EC_POS_3 0x07       // Elevator reports it is at floor 3

// Car Controller Commands (Received by supervisor)
#define CC_FLOOR_REQ_1 0x05 // Car requests floor 1
#define CC_FLOOR_REQ_2 0x06 // Car requests floor 2
#define CC_FLOOR_REQ_3 0x07 // Car requests floor 3

// Floor Controller Command (Received by supervisor)
#define FC_FLOOR_REQ 0x01 // Floor controller requests car

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

enum currentFloorEnum
{
    MOV,
    F1,
    F2,
    F3

};
enum currentFloorEnum currentFloor;

// enum carStateEnum { // Supervisor makes decisions based on car state
// 	STOP,		// Initial state / e-stop state
// 	FLOOR1,		// Arrives at floor 1
// 	FLOOR2,		// Arrives at floor 2
// 	FLOOR3,  	// Arrives at floor 3
// 	DESCENDING,	// Going down
// 	ASCENDING 	// Going up
// };

enum carStateEnum
{                // Supervisor makes decisions based on car state
    STOP,        // Initial state / e-stop state
    FLOOR1_IDLE, // Arrives at floor 1 no queued commands
    FLOOR2_IDLE, // Arrives at floor 2 no queued commands
    FLOOR3_IDLE, // Arrives at floor 3 no queued commands
    UP_TO_F3,
    UP_TO_F2,
    DOWN_TO_F2,
    DOWN_TO_F1,
    UP_F2_THEN_F3,  // go to floor 2 then floor 3
    DOWN_F2_THEN_F1 // go to floor 2 then floor 1
};
enum carStateEnum carState;
enum carStateEnum prevCarState;

enum msgTypeEnum
{ // NOTE: Seperating calls made from inside the car and outside may be a good idea for future expansion
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

int commandComplete = 1; // determines if next command should be run
int destFloor = 0;       // tracks where car should be before next command
int newCommand = 0;      // flag tracking if a new message has arrived
int queueLen = 0;
linkedList *commandQueue = (linkedList *)malloc(sizeof(linkedList) + sizeof(TPCANMsg)); // create empty list

int deDuplicateCommandQueue(void)
{
    return 0;
}

int receiveMsg(void)
{ // wait for message to come in
    TPCANMsg RxMsg;
    int temp = 0;
    HANDLE h2;
    DWORD status;

    // Open a CAN channel
    h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

    // Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
    status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

    // Clear the channel - new - Must clear the channel before Tx/Rx
    status = CAN_Status(h2);

    // check database requested floor db_getReqFloor();
    // set current floor in database db_setFloorNum(currentFloor);

    while (((status = CAN_Read(h2, &RxMsg)) == PCAN_RECEIVE_QUEUE_EMPTY) || ((temp = db_getReqFloor()) == currentFloor))
    {
        sleep(1);
    }
    printf("\n\ntemp:%d - ", temp);
    printf("cf:%d *****\n", currentFloor);

    CAN_Close(h2);

    unsigned int msgId = RxMsg.ID;
    unsigned char msgData = RxMsg.DATA[0];
    static char ecBaseStr[] = "EC Status:";

    printf("msgId = %4x\n", msgId);
    printf("msgData = %02x\n", msgData);

    if (msgId == EC_ID && msgData == EC_STATUS_DEEN && RxMsg.LEN != 0x04)
    {
        cmdType = EC_DEEN;
        newCommand = 1;
    }
    else if (msgId == EC_ID && msgData == EC_STATUS_EN && RxMsg.LEN != 0x04)
    {
        cmdType = EC_EN;
        newCommand = 1;
    }
    else if (msgId == EC_ID && msgData == EC_POS_MOV && RxMsg.LEN != 0x04)
    {
        currentFloor = MOV;
        printf("%s Moving\n", ecBaseStr);
        return 0;
    }
    else if ((msgId == EC_ID && msgData == EC_POS_1 && RxMsg.LEN != 0x04) && (temp == currentFloor || currentFloor == 0))
    { // Elevator at floor 1
        if (currentFloor != F1)
        {
            db_setFloorNum(1);
            db_incrementFloor1();
        }
        currentFloor = F1;
        printf("%s Floor 1\n", ecBaseStr);
        return 0;
    }
    else if ((msgId == EC_ID && msgData == EC_POS_2 && RxMsg.LEN != 0x04))
    { // Elevator at floor 2
        if (currentFloor != F2)
        {
            db_setFloorNum(2);
            db_incrementFloor2();
        }
        currentFloor = F2;
        printf("%s Floor 2\n", ecBaseStr);
        return 0;
    }
    else if ((msgId == EC_ID && msgData == EC_POS_3 && RxMsg.LEN != 0x04) && (temp == currentFloor || currentFloor == 0))
    { // Elevator at floor 3
        if (currentFloor != F3)
        {
            db_setFloorNum(3);
            db_incrementFloor3();
        }
        currentFloor = F3;
        printf("%s Floor 3\n", ecBaseStr);
        return 0;
    }
    else if (msgId == CC_ID && msgData == CC_FLOOR_REQ_1 && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR1_CALL;
        newCommand = 1;
    }
    else if (msgId == CC_ID && msgData == CC_FLOOR_REQ_2 && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR2_CALL;
        newCommand = 1;
    }
    else if (msgId == CC_ID && msgData == CC_FLOOR_REQ_3 && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR3_CALL;
        newCommand = 1;
    }
    else if (msgId == FC1_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR1_CALL;
        newCommand = 1;
    }
    else if (msgId == FC2_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR2_CALL;
        printf("F2 CALL\n CMDTyp: %d\n", cmdType);
        newCommand = 1;
    }
    else if (msgId == FC3_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04)
    {
        cmdType = FLOOR3_CALL;
        newCommand = 1;
    }
    else if (temp == 1 && currentFloor != 0)
    {
        cmdType = FLOOR1_CALL;
        newCommand = 1;
        printf("fc1 temp: %d\n", temp);
    }
    else if (temp == 2 && currentFloor != 0)
    {
        cmdType = FLOOR2_CALL;
        newCommand = 1;
        printf("fc2 temp: %d\n", temp);
    }
    else if (temp == 3 && currentFloor != 0)
    {
        cmdType = FLOOR3_CALL;
        newCommand = 1;
        printf("fc3 temp: %d\n", temp);
    }

    if (addElement((int)cmdType, &RxMsg, &commandQueue, 0, sizeof(TPCANMsg) + sizeof(linkedList)) == 0 && newCommand == 1)
    { // always add to head since command execution will determine order of execution
        //    printf("added type: %d", cmdType);
        // printAllElements(commandQueue);
        newCommand = 0; // set new command flag
        return 0;
    }
    else
    {
        return 1;
    }

    temp = 0;
}

void stateMachine()
{ // if last command has b
    prevCarState = carState;
    printf("State: %d CMDTyp: %d\n", carState, cmdType);
    switch (carState)
    { // NOTE: run through each transition state by state for multiple button press cases. Elevator stops at floor two now but forgets it was supposed to go to floor three as well
    case STOP:
        carState = FLOOR1_IDLE;
        break;
    case FLOOR1_IDLE:
        if (cmdType == FLOOR2_CALL)
        {
            carState = UP_TO_F2;
        }
        else if (cmdType == FLOOR3_CALL)
        {
            carState = UP_TO_F3;
        }
        break;
    case FLOOR2_IDLE:
        if (cmdType == FLOOR1_CALL)
        {
            carState = DOWN_TO_F1;
        }
        else if (cmdType == FLOOR3_CALL)
        {
            carState = UP_TO_F3;
        }
        break;
    case FLOOR3_IDLE:
        if (cmdType == FLOOR2_CALL)
        {
            carState = DOWN_TO_F2;
        }
        else if (cmdType == FLOOR1_CALL)
        {
            carState = DOWN_TO_F1;
        }
        break;
    case UP_TO_F3:
        if (cmdType == FLOOR2_CALL)
        {
            carState = UP_F2_THEN_F3;
        }
        else if (currentFloor == F3)
        {
            carState = FLOOR3_IDLE;
        }
        break;
    case UP_TO_F2:
        if (cmdType == FLOOR3_CALL)
        {
            carState = UP_F2_THEN_F3;
        }
        else if (currentFloor == F2)
        {
            carState = FLOOR2_IDLE;
        }
        break;
    case DOWN_TO_F2:
        if (cmdType == FLOOR1_CALL)
        {
            carState = DOWN_F2_THEN_F1;
        }
        else if (currentFloor == F2)
        {
            carState = FLOOR2_IDLE;
        }
        break;
    case DOWN_TO_F1:
        if (cmdType == FLOOR2_CALL)
        {
            carState = DOWN_F2_THEN_F1;
        }
        else if (currentFloor == F1)
        {
            carState = FLOOR1_IDLE;
        }
        break;
    case UP_F2_THEN_F3:
        if (currentFloor == F2)
        { // send to floor 3
        }
        else if (currentFloor == F3)
        {
            carState = FLOOR3_IDLE;
        }
        break;
    case DOWN_F2_THEN_F1:
        if (currentFloor == F2)
        {
        }
        else if (currentFloor == F1)
        {
            carState = FLOOR1_IDLE;
        }
        break;
    }
}

int nextCommand()
{ // looks at state machine state and spits out the next valid command from the linked list
    /* Theory of Operation
        1. look at state machine state
        2. scan commandQueue for command valid for current state
        3. return index of command to send
        4. DO NOT remove from list in case a higher priority command comes in


    */

    // Memory of recent commands
    static int prevCommand = 0;
    static int nextCommand = 0;

    prevCommand = nextCommand;

    switch (carState)
    {
    case STOP:
        // Do Nothing
        break;
    case FLOOR1_IDLE:
        // Grab first command, remove all floor 1 requests
        nextCommand = 0;
        removeAllById(commandQueue, FLOOR1_CALL);
        nextCommand = findByIndex(commandQueue, 0)->dataInt;
        break;
    case FLOOR2_IDLE:
        // Grab first command, remove all floor 2 requests
        nextCommand = 0;
        removeAllById(commandQueue, FLOOR2_CALL);
        nextCommand = findByIndex(commandQueue, 0)->dataInt;
        break;
    case FLOOR3_IDLE:
        // Grab first command, remove all floor 3 requests
        nextCommand = 0;
        removeAllById(commandQueue, FLOOR3_CALL);
        nextCommand = findByIndex(commandQueue, 0)->dataInt;
        break;
    case UP_TO_F3:
        // Look for command going to floor two (if didn't start at floor 2), remove all floor 3 requests
        removeAllById(commandQueue, FLOOR3_CALL);
        if (prevCommand != FLOOR2_CALL)
        {
            nextCommand = findFirstByID(commandQueue, FLOOR3_CALL)->dataInt;
        }
        break;
    case UP_TO_F2:
        // Look for command going to floor three, remove all floor 2 requests
        removeAllById(commandQueue, FLOOR2_CALL);
        nextCommand = findFirstByID(commandQueue, FLOOR3_CALL)->dataInt;
        break;
    case DOWN_TO_F2:
        // Look for command going to floor one, remove all floor 2 requests
        removeAllById(commandQueue, FLOOR2_CALL);
        nextCommand = findFirstByID(commandQueue, FLOOR1_CALL)->dataInt;
        break;
    case DOWN_TO_F1:
        // Look for command going to floor two (if started at floor 3), remove all floor 1 requests
        removeAllById(commandQueue, FLOOR1_CALL);
        if (prevCommand == FLOOR3_CALL)
        {
            nextCommand = findFirstByID(commandQueue, FLOOR2_CALL)->dataInt;
        }
        break;
    case UP_F2_THEN_F3:
        // Look for command going to floor 1, remove all floor 2 and 3 requests
        removeAllById(commandQueue, FLOOR2_CALL);
        removeAllById(commandQueue, FLOOR3_CALL);
        nextCommand = findFirstByID(commandQueue, FLOOR1_CALL)->dataInt;
        break;
    case DOWN_F2_THEN_F1:
        // Look for command going to floor three, remove all floor 2 and 1 requests
        removeAllById(commandQueue, FLOOR2_CALL);
        removeAllById(commandQueue, FLOOR1_CALL);
        nextCommand = findFirstByID(commandQueue, FLOOR3_CALL)->dataInt;
        break;
    default:
        return -1;
        break;
    }

    cmdType = (enum msgTypeEnum)nextCommand;
    return nextCommand;
}

int executeCommand(int cmd)
{ // execute a command and set the commandServiced flag
    /* Theory of Operation
        1. execute command at input index
        2. set commandServiced flag
        3. remove command from queue

    */
    switch (cmd)
    {
    case FLOOR1_CALL:
        pcanTx(SC_ID, SC_FLOOR_REQ_1);
        break;
    case FLOOR2_CALL:
        printf("F2 SEND\n");
        pcanTx(SC_ID, SC_FLOOR_REQ_2);
        break;
    case FLOOR3_CALL:
        pcanTx(SC_ID, SC_FLOOR_REQ_3);
        break;
    default:
        return -1;
        break;
    }

    return 0;
}

// void stateMachine() { // if last command has b //NOTE: Kinda works
//     prevCarState = carState;

//     switch (carState) { // NOTE: run through each transition state by state for multiple button press cases. Elevator stops at floor two now but forgets it was supposed to go to floor three as well
//         case STOP:
//         carState = FLOOR1_IDLE;
//         break;
//         case FLOOR1_IDLE:
//             if (cmdType == FLOOR2_CALL) {
//                 carState = UP_TO_F2;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_2);
//             } else if (cmdType == FLOOR3_CALL) {
//                 carState = UP_TO_F3;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_3);
//             }
//         break;
//         case FLOOR2_IDLE:
//             if (cmdType == FLOOR1_CALL) {
//                 carState = DOWN_TO_F1;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_1);
//             } else if (cmdType == FLOOR3_CALL) {
//                 carState = UP_TO_F3;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_3);
//             }
//         break;
//         case FLOOR3_IDLE:
//             if (cmdType == FLOOR2_CALL) {
//                 carState = DOWN_TO_F2;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_2);
//             } else if (cmdType == FLOOR1_CALL) {
//                 carState = DOWN_TO_F1;
//                 pcanTx(SC_ID, SC_FLOOR_REQ_1);
//             }
//         break;
//         case UP_TO_F3:
//             if (cmdType == FLOOR2_CALL) {
//                 carState = UP_F2_THEN_F3;

//             } else if (currentFloor == F3) {
//                 carState = FLOOR3_IDLE;
//             }
//         break;
//         case UP_TO_F2:
//             if (cmdType == FLOOR3_CALL) {
//                 carState = UP_F2_THEN_F3;
//             } else if (currentFloor == F2) {
//                 carState = FLOOR2_IDLE;
//             }
//         break;
//         case DOWN_TO_F2:
//             if (cmdType == FLOOR1_CALL) {
//                 carState = DOWN_F2_THEN_F1;
//             } else if (currentFloor == F2) {
//                 carState = FLOOR2_IDLE;
//             }
//         break;
//         case DOWN_TO_F1:
//             if (cmdType == FLOOR2_CALL) {
//                 carState = DOWN_F2_THEN_F1;
//             } else if (currentFloor == F1) {
//                 carState = FLOOR1_IDLE;
//             }
//         break;
//         case UP_F2_THEN_F3:
//             if (prevCarState == UP_TO_F3) {pcanTx(SC_ID, SC_FLOOR_REQ_2);} // make sure car always stops at floor 2 in this state
//             if (currentFloor == F2) { // send to floor 3
//                 pcanTx(SC_ID, SC_FLOOR_REQ_3);
//             } else if (currentFloor == F3) {
//                 carState = FLOOR3_IDLE;
//             }
//         break;
//         case DOWN_F2_THEN_F1:
//             if (prevCarState == DOWN_TO_F1) {pcanTx(SC_ID, SC_FLOOR_REQ_2);} // make sure car always stops at floor 2 in this state
//             if (currentFloor == F2) {
//                 pcanTx(SC_ID, SC_FLOOR_REQ_1);
//             } else if (currentFloor == F1) {
//                 carState = FLOOR1_IDLE;
//             }
//         break;
//     }

// }

// static void executeCommand(int cmdType) {
//    	static char ecBaseStr[] = "EC Status:";
// 	static char ccBaseStr[] = "CC Status:";
// 	static char fcBaseStr[] = "FC Status:";

// 	if (currentFloor == destFloor) {commandComplete = 1;}

// 	if (commandComplete == 1) {

// 	if (cmdType == EC_DEEN) {
// 		printf("%s Disabled\n", ecBaseStr);

// 	} else if (cmdType == EC_EN) {
// 		printf("%s Enabled\n", ecBaseStr);

//     } else if (cmdType == FLOOR1_CALL) {
//         if (FLOOR1_CALL > currentFloor) {carState = ASCENDING;}
//         else if (FLOOR1_CALL < currentFloor) {carState = DESCENDING;}
//         else {carState = (enum carStateEnum)currentFloor;}
// 		printf("%s Requested at Floor 1\n", ccBaseStr);
// 		pcanTx(SC_ID, SC_FLOOR_REQ_1);
// 		destFloor = FLOOR1;
// 		commandComplete = 0;

// 	} else if (cmdType == FLOOR2_CALL) {
//         if (FLOOR2_CALL > currentFloor) {carState = ASCENDING;}
//         else if (FLOOR2_CALL < currentFloor) {carState = DESCENDING;}
//         else {carState = (enum carStateEnum)currentFloor;}
// 		printf("%s Requested at Floor 2\n", ccBaseStr);
// 		pcanTx(SC_ID, SC_FLOOR_REQ_2);
// 		destFloor = FLOOR2;
// 		commandComplete =0;

// 	} else if (cmdType == FLOOR3_CALL) {
//         if (FLOOR3_CALL > currentFloor) {carState = ASCENDING;}
//         else if (FLOOR3_CALL < currentFloor) {carState = DESCENDING;}
//         else {carState = (enum carStateEnum)currentFloor;}
// 		printf("%s Requested at Floor 3\n", ccBaseStr);
// 		pcanTx(SC_ID, SC_FLOOR_REQ_3);
// 		destFloor = FLOOR3;
// 		commandComplete = 0;
//     }
// }
// }

// int processMsg(void) { // determine command message to execute based on car state.
// 	/*
// 	 *	Function Name:
// 	 *	Function Type:
// 	 *	Function Inputs:
// 	 *	Function Outputs:
// 	 *	Error Handling:
// 	 *	Function Description:
// 	 *
// 	*/
//     linkedList* tmp = (linkedList*)malloc(sizeof(linkedList)+sizeof(TPCANMsg));

//     // State Machine Control Logic
// 	if (carState == STOP) {
// 	carState = DESCENDING;

// 	} else if (carState == FLOOR1 || carState == FLOOR2 || carState == FLOOR3) { // in this state everything except an e-stop is equal
//        tmp = findByIndex(commandQueue, 0); // get head of command queue
//        if (tmp != NULL) {
//        printf("CMD: %d\n", tmp->dataInt);
//        executeCommand(tmp->dataInt); // dataInt holds command type
//        removeElement(&commandQueue, 0); // delete head element
//        }
//        return 0;

// 	} else if (carState == DESCENDING) { // in this state floor calls below CURRENT_FLOOR are highest priority
//         if (currentFloor == 3) {
//             tmp = findFirstByID(commandQueue, currentFloor - 1);
//             if (tmp == NULL) {
//                 tmp = findFirstByID(commandQueue, currentFloor - 2);
//                 if (tmp == NULL) { // no down calls
//                     printf("No Down\n");
//                     return 0;
//                 }
//             }

//         } else if (currentFloor == 2) {
//             // execute first command with a floor less than current floor

//             tmp = findFirstByID(commandQueue, currentFloor - 1);
//             if (tmp == NULL) {
//                 return 0;
//             }

//         } else if (currentFloor == 1) { // already at lowest floor this state should not be possible

//         } else { // elevator moving do nothing

//         }

// 	} else if (carState == ASCENDING ) { // in this state floor calls above CURRENT_FLOOR are highest priority
//         // execute first command with a floor greater than current floor
//         if (currentFloor == 1) {
//             tmp = findFirstByID(commandQueue, currentFloor + 1);
//             if (tmp == NULL) {
//                 tmp = findFirstByID(commandQueue, currentFloor + 2);
//                 if (tmp == NULL) { // no up calls
//                     return 0;
//                 }
//             }

//         } else if (currentFloor == 2) {
//             // execute first command with a floor less than current floor

//             tmp = findFirstByID(commandQueue, currentFloor + 1);
//             if (tmp == NULL) {
//                 carState = (enum carStateEnum)currentFloor; // no up calls
//                 return 0;
//             }

//         } else if (currentFloor == 3) { // already at lowest floor this state should not be possible
//             carState = (enum carStateEnum)currentFloor;

//         } else { // elevator moving do nothing

//         }
// 	}
//     // execute command for ascending and descending if/else chains
//     if (tmp != NULL) {
//         executeCommand(tmp->dataInt);
//         int pos = getElementPosById(commandQueue, tmp->dataInt); // remove element after command execution
//         removeElement(&commandQueue, pos);

//     }
//     return 0;

// 	// static char ecBaseStr[] = "EC Status:";
// 	// static char ccBaseStr[] = "CC Status:";
// 	// static char fcBaseStr[] = "FC Status:";

// 	// Decide whether and what to transmit depending on sender and msg content
// 	// if (msgData == EC_ID && msgData == EC_STATUS_DEEN && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Disabled", ecBaseStr);

// 	// } else if (msgData == EC_ID && msgData == EC_STATUS_EN && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Enabled", ecBaseStr);

// 	// } else if (msgData == EC_ID && msgData == EC_POS_MOV && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Moving", ecBaseStr);

// 	// } else if (msgData == EC_ID && msgData == EC_POS_1 && RxMsg.LEN != 0x04) { // Elevator at floor 1
// 	// 	printf("%s Floor 1", ecBaseStr);

// 	// } else if (msgData == EC_ID && msgData == EC_POS_2 && RxMsg.LEN != 0x04) { // Elevator at floor 2
// 	// 	printf("%s Floor 2", ecBaseStr);

// 	// } else if (msgData == EC_ID && msgData == EC_POS_3 && RxMsg.LEN != 0x04) { // Elevator at floor 3
// 	// 	printf("%s Floor 3", ecBaseStr);

// 	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_1 && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Request Floor 1", ccBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_1);

// 	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_2 && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Request Floor 2", ccBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_2);

// 	// } else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_3 && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Request Floor 3", ccBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_3);

// 	// } else if (msgData == FC1_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Floor 1 Requests Car\n", fcBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_1);

// 	// } else if (msgData == FC2_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Floor 2 Requests Car\n", fcBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_2);

// 	// } else if (msgData == FC3_ID && msgData == FC_FLOOR_REQ && RxMsg.LEN != 0x04) {
// 	// 	printf("%s Floor 3 Requests Car\n", fcBaseStr);
// 	// 	pcanTx(SC_ID, SC_FLOOR_REQ_3);

// 	// }
// }
