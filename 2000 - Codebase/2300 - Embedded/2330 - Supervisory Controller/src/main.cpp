#include "../include/CAN.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <iostream>

using namespace std;


// ******************************************************************

#define CAN_ID 0x100
#define GO_TO_FLOOR_1 0x05
#define GO_TO_FLOOR_2 0x06
#define GO_TO_FLOOR_3 0x07

int main() {
	while(1) {
		// receive can message and transmit if required
		TPCANMsg Rxmsg;
		Rxmsg = pcanRx();

		if (Rxmsg.ID == 0x201 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 1 floor call request
			printf("Floor 1 Request\n");
			pcanTx(CAN_ID, GO_TO_FLOOR_1);
			
		} else if (Rxmsg.ID == 0x202 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 2 floor call request
			printf("Floor 2 Request\n");
			pcanTx(CAN_ID, GO_TO_FLOOR_2);
			
		} else if (Rxmsg.ID == 0x203 && Rxmsg.DATA[0] == 0x01 && Rxmsg.LEN != 0x04) { // Floor 3 floor call request
			printf("Floor 3 Request\n");
			pcanTx(CAN_ID, GO_TO_FLOOR_3);
			
		}
	}
	
	return 0;
}






	
