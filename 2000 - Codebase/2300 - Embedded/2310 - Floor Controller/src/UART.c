// Copyright 2021 Garnet Koebel
// All Rights Reserved

//! Mobile Robotics Platform - UART Subsystem Definitions

/*! \file UART.c
 *  <pre>
 *	Header Name:        UART.c
 *	Header Author:      Garnet Koebel
 *  Header Version:     V0.1
 *	Header Created:     2021-01-30
 *	Last Header Update: Refer to version control
 *	Header Description: This header contains the function definitions for using USART's
 *  </pre>
 */	
 /*	Update 2021-02-13:
 *	- Rewrote UARTgetc to be non-blocking. Function now returns a null character if no new character has been received at call time.
 *
 *	Update 2021-04-21
 *	- Added UARTgets for receiving strings
 *		
 *	Update for 2021-09-13
 *	- Added copyright information to file
 *
 */
 	
	
#include "UART.h"	

 void UARTinit(USART_TypeDef * USARTx, uint16_t baud) { //inits a given UART / USART
 	/*
	 *	Function Name: 			UARTinit
	 *	Function Type: 			void
	 *	Function Inputs: 		USART_TypeDef * USARTx - the USART to initialize
	 *	Function Outputs: 		none
	 *	Error Handling:			No
	 *	Function Description:	This function initialzes a USART specified by USARTx
	 *
	*/
		if (USARTx == USART3) {
		 
		 //GPIO Setup for USART3
		 GPIO_EN(B); // Enable GPIO port B Clock
		 GPIO_SET_MODE(B, USART3_TX_PIN, GPIO_MODE_AF);
		 GPIO_SET_MODE(B, USART3_RX_PIN, GPIO_MODE_AF);
		 GPIO_SET_AF(1, B, 2, GPIO_AF_7); // set 3rd bit of AFR high register
		 GPIO_SET_AF(1, B, 3, GPIO_AF_7); // set 4th bit of AFR high register
		 
		 GPIO_SET_PUPD(B, USART3_TX_PIN, GPIO_PUPD_NONE);
		 GPIO_SET_PUPD(B, USART3_TX_PIN, GPIO_PUPD_NONE);
		 
		 GPIO_SET_OSPEED(B, USART3_TX_PIN, GPIO_OSPEED_LOW);
		 GPIO_SET_OSPEED(B, USART3_RX_PIN, GPIO_OSPEED_LOW);
		 
		 // USART Setup for USART3
		 UART_CLK_EN(USART3);				// enable clock
		 UART_CLK_CLR_SRC(USART3);  // clear clock source selection
		 UART_CLK_SET_SRC(USART3, RCC_CFGR3_USART2SW_0); // select clock source
		 UART_DEEN(USART3); // disable USART
		 
		 USART3->CR1 &= ~USART_CR1_M;		  // set datalength to 8 bits
		
		 USART3->CR2 &= ~USART_CR2_STOP; 	// set number of stop bits to 1 
	 
		 USART3->CR1 &= ~USART_CR1_PCE;		// set parity control to NO
		 USART3->CR1 &= ~USART_CR1_OVER8;	// set oversampling to by 16
	 
		 USART3->BRR = baud;						// set baud rate to 9600 for a 72 MHz clock (NOTE: Set this to calculate at run time later)
	 
		 USART3->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// enable reception and transmission
	 
		 UART_EN(USART3);					// enable USART
	 
		 while ((USART3->ISR & USART_ISR_TEACK) ==0); 	// verify USART is ready for transmission
	 
		 while ((USART3->ISR & USART_ISR_REACK) ==0);	// verify USART is ready for reception
		 
		 
	 } else {
	 
	 
		
		USARTx->CR1 &= ~USART_CR1_UE; 		// disable USART
		USARTx->CR1 &= ~USART_CR1_M;		  // set datalength to 8 bits
		
		USARTx->CR2 &= ~USART_CR2_STOP; 	// set number of stop bits to 1 
	 
		USARTx->CR1 &= ~USART_CR1_PCE;		// set parity control to NO
		USARTx->CR1 &= ~USART_CR1_OVER8;	// set oversampling to by 16
	 
		USARTx->BRR = baud;						// set baud rate to 9600 for a 72 MHz clock (NOTE: Set this to calculate at run time later)
	 
		USARTx->CR1 |= (USART_CR1_TE | USART_CR1_RE);	// enable reception and transmission
	 
		USARTx->CR1 |= USART_CR1_UE;					// enable USART
	 
		while ((USARTx->ISR & USART_ISR_TEACK) ==0); 	// verify USART is ready for transmission
	 
		while ((USARTx->ISR & USART_ISR_REACK) ==0);	// verify USART is ready for reception
	}
	 
 }	 
 
 int UARTputc(char cx, USART_TypeDef * USARTx) {
	/*
	 *	Function Name: 			UARTputc
	 *	Function Type: 			void
	 *	Function Inputs: 		char cx - character to transmit, USART_TypeDef * USARTx - USART to use for transmission
	 *	Function Outputs: 	none
	 *	Error Handling:			No
	 *	Function Description:	This function takes an input character and transmits it using the specified USART.
	 *
	*/
	 
	 	while (!(USARTx->ISR & USART_ISR_TXE)); //wait until something is set
	 	USARTx->TDR = (uint8_t)cx;

	  return 0;
 }
 
 void UARTputs(char *str, USART_TypeDef* USARTx){ //output characters until we hit a null (NULL's terminate a C string
	/*
	 *	Function Name: 			UARTputs
	 *	Function Type: 			void
	 *	Function Inputs: 		char *str - string to transmit, USART_TypeDef* USARTx
	 *	Function Outputs: 	none
	 *	Error Handling:			No
	 *	Function Description:	This function transmits an input string using a specified USART
	 *
	*/
	
		while(*str) {	//transmit characters one by one until null character null
			UARTputc(*str++, USARTx); // call transmit function
			
		}
 }
 
int UARTprintf(char *fmt, ...) { //prints to a given UART / USART (add error handling to prevent printing to uninitialized UART's)
	/*
	 *	Function Name: 			UARTprintf
	 *	Function Type: 			int
	 *	Function Inputs: 		char *fmt - format string, ... variable number of additional parameters according to format string.
	 *	Function Outputs: 	returns 0 on successful run
	 *	Error Handling:			No
	 *	Function Description:	This function implements a UART based printf. Function uses vsnprintf to internally parse the input format string (fmt)
	 *							vsnprintf prints the parsed format string to an internal buffer which is then transmitted using a specified USART (using UARTputs).
	 *
	*/
	
	//NOTE: add USARTx as a parameter for this function
	
		//thisArg = va_arg(myArgs, <type>);
		va_list args; // declare a variable argument list
	
		char buffer[MAX_UART_BUFSIZE]; // declare a buffer to hold parsed string
		
		va_start(args, fmt); // call va_start on fixed parameter
		//thisArg = va_arg(args, <type>);
		vsnprintf(buffer, MAX_UART_BUFSIZE, fmt, args); // parse format string using vsnprintf
		va_end(args); // cleanup reference pointers
	
		UARTputs(buffer, USART2);	// transmit parsed string from buffer over USART2
		return 0;
 }	
 
char UARTgetc(USART_TypeDef* USARTx) {
	/*
	 *	Function Name: 			UARTgetc	
	 *	Function Type: 			char
	 *	Function Inputs: 		USART_TypeDef* USARTx - USART to use for getting a character
	 *	Function Outputs: 	outputs the character gotten
	 *	Error Handling:			No
	 *	Function Description:	This function recieves a character using a specified USART and returns that character. This function is non-blocking.
	 *
	*/
	 
		if ((USARTx->ISR & USART_ISR_RXNE)) { // check if a character has been recieved
			return(USARTx->RDR); // return received character
		
		} else {
			return('\0'); // return null if no character recieved
			
		}
		
	 
 }

char UARTgetc_Int(USART_TypeDef* USARTx) {
	/*
	 *	Function Name: 			UARTgetc_Int
	 *	Function Type: 			char
	 *	Function Inputs: 		USART_TypeDef* USARTx - USART to use for getting a character
	 *	Function Outputs: 	outputs the character gotten
	 *	Error Handling:			No
	 *	Function Description:	This function recieves a character using a specified USART and returns that character. This function is non-blocking
	 *  											interrupt friendly. DO NOT use this function in polling applications
	 *
	*/
	 
			return(USARTx->RDR); // return received character
		
 }

 // EVERYTIME A CHAR IS RECEIVED ADD TO A STRING BUFFER AND CHECK FOR WHEN ENTER IS PRESSED TO PROCESS AS STRING - Idea Adapted from Spencer
 
char* UARTgets(USART_TypeDef* USARTx) {
	/*
	 *	Function Name: 				UARTgets		
	 *	Function Type: 				char*
	 *	Function Inputs: 			USART_TypeDef* USARTx - UART to use for reception
	 *	Function Outputs: 		returns a pointer to a null terminated character array
	 *	Error Handling:				No
	 *	Function Description:	This function uses a specified USART to receive a string of characters
	 *												it stops once a line feed character is received and then returns a pointer to
	 *												a null terminated character array containing the received string. This function echos back
														characters to the terminal for easier entry.
	 *
	*/
	 
	 // NOTE: implement next week
	 // Get characters until a NULL is received
	 
	 char recv = '\0'; // holder variable for received characters
	 char outStr[MAX_UART_BUFSIZE]; // output string holder 
	 
	 uint8_t i = 0; // counter for while loop
	 
	 do { 											// Attempt to receive characters until a NULL character is received (indicating end of string
		 recv = UARTgetc(USARTx); // attempt to receive character
		 outStr[i] = recv; 				// store received character in output string
		 UARTputc(recv, USART2); 	// Echo back entered character
		 
		 if (recv != '\0') { // if something goes wrong, there's no message, or someone is messing with the program we need to terminate on a NULL receipt.
			 i++; // increment counter 
			
		 }
	 } while (recv != 0x0A); // A newline is used to detect the intended end of string. Also terminate if buffer is about to overflow.
	 
	 outStr[i] = '\0'; //NULL terminate string
	 
	 
	 // At this point we should either have a full string (NULL Terminated) or a NULL character. Either way return it.
	 return outStr;
	 
 }
 

