#include "../include/Supervisor.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <iostream>

using namespace std;

int main() {
	printf("G2 Elevator Supervisor\nV0.3\nBooted\n\n");
	while(1) {
		receiveMsg(); // wait for next message
		executeCommand(nextCommand());
		stateMachine();
		
		//processMsg(); // process highest priority command
		
	}
	return 0;
}






	
