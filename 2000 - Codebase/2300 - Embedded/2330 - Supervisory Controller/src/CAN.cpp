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

// Globals
DWORD status;

// Functions
int pcanTx(int id, int data) {
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

TPCANMsg pcanRx(void) {
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