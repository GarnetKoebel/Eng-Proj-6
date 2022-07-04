#include "main.h"
#include "Elevator.h"
#include <stm32f303xe.h>

#define NO_BUTTON_PRESSED 	0
#define BLUE_BUTTON_PRESSED 1

int main(void) {
	elevatorInit();

  while (1) {



	  processMsg();

	  //Transmit
	  //if (BUTTON !=0) {
		  //msgTx();
	 // }
  }

}
