#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h> 

#define CAN_ID 0x100
#define GO_TO_FLOOR_1 0x05
#define GO_TO_FLOOR_2 0x06
#define GO_TO_FLOOR_3 0x07

int pcanTx(int id, int data){
	h = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);		// Open PCAN channel

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h);

	// Set up message
	Txmsg.ID = id; 	
	Txmsg.MSGTYPE = MSGTYPE_STANDARD; 
	Txmsg.LEN = 1; 
	Txmsg.DATA[0] = data; 

	sleep(1);  
	status = CAN_Write(h, &Txmsg);

	// Close CAN 2.0 channel and exit	
	CAN_Close(h);
}

int pcanRx(int num_msgs){
	int i = 0;

	// Open a CAN channel 
	h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	// Clear screen to show received messages
	system("@cls||clear");

	
	// Read 'num' messages on the CAN bus
	while(i < num_msgs) {
		while((status = CAN_Read(h2, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY){
			sleep(1);
		}
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}
		
		if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 1 floor call request
			printf("Floor 1 Request");
			pcanTx(CAN_ID, GO_TO_FLOOR_1);
			i++
			
		} else if (Rxmsg.ID == 0x202 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 2 floor call request
			printf("Floor 2 Request");
			pcanTx(CAN_ID, GO_TO_FLOOR_2);
			i++
			
		} else if (Rxmsg.ID == 0x203 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 3 floor call request
			printf("Floor 3 Request");
			pcanTx(CAN_ID, GO_TO_FLOOR_3);
			i++
			
		}
			
							
	}
	

	// Close CAN 2.0 channel and exit	
	CAN_Close(h2);
	//printf("\nEnd Rx\n");
	return ((int)Rxmsg.DATA[0]);						// Return the last value received
}

void main() {
	while(1) {
		// receive can message and transmit if required
		pcanRx(1);
	}
	
}
