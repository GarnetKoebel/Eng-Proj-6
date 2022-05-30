#include "../include/pcanFunctions.h"
#include "../include/databaseFunctions.h"
#include "../include/mainFunctions.h"

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
		pcanRx(1);
	}
	
	return 0;
}






	
