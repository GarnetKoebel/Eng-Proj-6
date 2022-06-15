/*
 * 
 *	Name: 			CAN.cpp	
 *	Author:			Garnet Koebel, adapted from examples by Micheal Galle
 *	Version: 		V0.3
 *	Created:		Refer to source control
 *	Last Update:	Refer to source control
 *	Description:	This file defines the implementation elevator CAN protocol.
 *
 */

#include "../include/CAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>  
#include <errno.h>
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <fcntl.h>    					// O_RDWR
#include <unistd.h>
#include <ctype.h>
#include <libpcan.h>

// Defines
#define PCAN_RECEIVE_QUEUE_EMPTY        0x00020U  	// Receive queue is empty
#define PCAN_NO_ERROR               	0x00000U  	// No error 

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

// Globals
DWORD status;

// Functions
static int pcanTx(int id, int data) {
	/*
	 *	Function Name: 				
	 *	Function Type: 				
	 *	Function Inputs: 			
	 *	Function Outputs: 		
	 *	Error Handling:				
	 *	Function Description:	
	 *
	*/

	TPCANMsg Txmsg;
	HANDLE h;

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

static TPCANMsg pcanRx(void) {
	/*
	 *	Function Name: 				
	 *	Function Type: 				
	 *	Function Inputs: 			
	 *	Function Outputs: 		
	 *	Error Handling:				
	 *	Function Description:	
	 *
	*/

	TPCANMsg Rxmsg;
	HANDLE h2;

	// Open a CAN channel 
	h2 = LINUX_CAN_Open("/dev/pcanusb32", O_RDWR);

	// Initialize an opened CAN 2.0 channel with a 125kbps bitrate, accepting standard frames
	status = CAN_Init(h2, CAN_BAUD_125K, CAN_INIT_TYPE_ST);

	// Clear the channel - new - Must clear the channel before Tx/Rx
	status = CAN_Status(h2);

	// Read 'num' messages on the CAN bus
		while((status = CAN_Read(h2, &Rxmsg)) == PCAN_RECEIVE_QUEUE_EMPTY){
			sleep(1);
		}
		if(status != PCAN_NO_ERROR) {						// If there is an error, display the code
			printf("Error 0x%x\n", (int)status);
			//break;
		}

	// Close CAN 2.0 channel and exit	
	CAN_Close(h2);
	//printf("\nEnd Rx\n");
	return (Rxmsg);						// Return the received message
}

int processCANMsg(void) {
	/*
	 *	Function Name: 				
	 *	Function Type: 				
	 *	Function Inputs: 			
	 *	Function Outputs: 		
	 *	Error Handling:				
	 *	Function Description:	
	 *
	*/
	static char ecBaseStr[] = "EC Status:";
	static char ccBaseStr[] = "CC Status:";
	static char fcBaseStr[] = "FC Status:";

	TPCANMsg Rxmsg;
	Rxmsg = pcanRx(); // wait for next CAN msg
	// Extract relevent portions of structure to save a few cycles
	unsigned int msgId = Rxmsg.ID;
	unsigned char msgData = Rxmsg.DATA[0];

	// Decide whether and what to transmit depending on sender and msg content
	if (msgData == EC_ID && msgData == EC_STATUS_DEEN && Rxmsg.LEN != 0x04) {
		printf("%s Disabled", ecBaseStr);

	} else if (msgData == EC_ID && msgData == EC_STATUS_EN && Rxmsg.LEN != 0x04) {
		printf("%s Enabled", ecBaseStr);

	} else if (msgData == EC_ID && msgData == EC_POS_MOV && Rxmsg.LEN != 0x04) {
		printf("%s Moving", ecBaseStr);

	} else if (msgData == EC_ID && msgData == EC_POS_1 && Rxmsg.LEN != 0x04) { // Elevator at floor 1 
		printf("%s Floor 1", ecBaseStr);

	} else if (msgData == EC_ID && msgData == EC_POS_2 && Rxmsg.LEN != 0x04) { // Elevator at floor 2
		printf("%s Floor 2", ecBaseStr);

	} else if (msgData == EC_ID && msgData == EC_POS_3 && Rxmsg.LEN != 0x04) { // Elevator at floor 3
		printf("%s Floor 3", ecBaseStr);

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_1 && Rxmsg.LEN != 0x04) {
		printf("%s Request Floor 1", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_1);

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_2 && Rxmsg.LEN != 0x04) {
		printf("%s Request Floor 2", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_2);

	} else if (msgData == CC_ID && msgData == CC_FLOOR_REQ_3 && Rxmsg.LEN != 0x04) {
		printf("%s Request Floor 3", ccBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_3);

	} else if (msgData == FC1_ID && msgData == FC_FLOOR_REQ && Rxmsg.LEN != 0x04) {
		printf("%s Floor 1 Requests Car\n", fcBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_1);
			
	} else if (msgData == FC2_ID && msgData == FC_FLOOR_REQ && Rxmsg.LEN != 0x04) {
		printf("%s Floor 2 Requests Car\n", fcBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_2);
			
	} else if (msgData == FC3_ID && msgData == FC_FLOOR_REQ && Rxmsg.LEN != 0x04) {
		printf("%s Floor 3 Requests Car\n", fcBaseStr);
		pcanTx(SC_ID, SC_FLOOR_REQ_3);
			
	}
}

static void ecStatus(int f) { // helper function to print ec status
	printf("EC Status: %d", f);
}