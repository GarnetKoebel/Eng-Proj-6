
//#include "main.h"
#include "Elevator.h"
#include "UART.h"
#include "SysClock.h"
#include <stm32f303xe.h>

#define NO_BUTTON_PRESSED 	0
#define BLUE_BUTTON_PRESSED 1

int main(void) {
	elevatorInit(); // initialize elevator controls

	UARTinit(USART2, 0x1D4C);
	UARTputc('S', USART2);

  while (1) {



	  processMsg();

	  //Transmit
	  //if (BUTTON !=0) {
		  //msgTx();
	 // }
  }

}
