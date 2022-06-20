// Copyright 2021 Garnet Koebel
// All Rights Reserved

/*
 * 
 *	Header Name: 				UART.h
 *	Header Author: 			Garnet Koebel
 *  Header Version: 		V0.1
 *	Header Created:			2021-01-30
 *	Last Header Update:	2021-09-13
 *	Header Description:	This header contains the function declarations for using USART's
 *
 *	Update for 2021-09-13
 * 	- Added copyright information to file
 */
 
 #ifndef UART_H
 #define UART_H
 
 #include <stdio.h>
 #include <stdarg.h>
 #include "GPIO.h"
 #include "Macros.h"
 
 #define MAX_UART_BUFSIZE 64
 
 #define USART3_TX_PIN PB10
 #define USART3_RX_PIN PB11
 
 // UART MACROS
 #define UART_CLK_EN(UARTx) RCC->APB1ENR |= RCC_APB1ENR_##UARTx##EN
 #define UART_CLK_CLR_SRC(UARTx) RCC->CFGR3 &= ~(RCC_CFGR3_##UARTx##SW) // clear clock selection bits
 #define UART_CLK_SET_SRC(UARTx, src) RCC->CFGR3 |= (RCC_CFGR3_##UARTx##SW_0) // select clock source
 #define UART_EN(UARTx) UARTx->CR1 |= USART_CR1_UE					// enable USART
 #define UART_DEEN(UARTx) UARTx->CR1 &= ~USART_CR1_UE				// disable USART
 //#define UART_SET_DATLEN(UARTx) UARTx->CR1 &= ~USART_CR1_M;		  // set datalength to 8 bits 
 
 // UART INTURRUPT MACROS
 #define UART_INT_RXNE_EN(UARTx) UARTx->CR1 |= USART_CR1_RXNEIE 	 // set Receive Data Register Not Empty Empty Intrrupt Enable Bit
 #define UART_INT_RXNE_DEEN(UARTx) UARTx->CR1 &= ~USART_CR1_RXNEIE // clear Receive Data Register Not Empty Empty Intrrupt Enable Bit
 
 // UART Setup Functions
 void UARTinit(USART_TypeDef* USARTx, uint16_t baud);		//inits a given UART / USART
 
 
 // UART Output Functions
 int UARTputc(char cx, USART_TypeDef* USARTx);
 void UARTputs(char *str, USART_TypeDef* USARTx);

 int UARTprintf(char *fmt, ...);	//prints to a given UART / USART (add error handling to prevent printing to uninitialized UART's) - Smart variant of UARTputs built ontop of UARTputs it will be hardcoded to print out the intended USART for the project
 
 // UART Input Functions
 char UARTgetc(USART_TypeDef* USARTx); // Polling UARTgetc
 char UARTgetc_Int(USART_TypeDef* USARTx); // Interrupt UARTgetc
 char* UARTgets(USART_TypeDef* USARTx);
 
 #endif
