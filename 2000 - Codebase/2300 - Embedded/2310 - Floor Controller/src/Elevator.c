// This is the implementation for the
#include "Elevator.h"
#include <stm32f303xe.h>
#include "macros.h"
#include "GPIO.h"
#include "CAN.h"
#include "UART.h"

#define BUTTON_NOT_PRESSED 	0
#define BUTTON_PRESSED 		1

// Switch Lighting Commands (move to controller specific sections later)
#define SLOW_BLINK 0
#define FAST_BLINK 1
#define SOLID	   2
#define OFF		   3

// Elevator Controller Commands (Received by all)
#define EC_STATUS_DEEN 0x00 // Elevator reports it is disabled
#define EC_STATUS_EN   0x01 // Elevator reports it is enabled
#define EC_POS_MOV     0x00 // Elevator reports it is moving
#define EC_POS_1       0x05 // Elevator reports is it at floor 1
#define EC_POS_2       0x06 // Elevator reports it is at floor 2
#define EC_POS_3       0x07 // Elevator reports it is at floor 3

static uint8_t		   RxData[8]; // stores most recent message
//static uint8_t		   newMsg = 0; // tracks if a new message has come in

static uint8_t		   lightStatus = OFF;
static uint8_t		   ecStatus = EC_STATUS_DEEN; // Keeps track of elevator controller status
uint8_t 		   	   BUTTON = BUTTON_NOT_PRESSED;

CAN_msg       CAN_TxMsg;      // CAN messge for sending

//static CAN_TxHeaderTypeDef TxHeader;
//CAN_HandleTypeDef hcan;

// NOTE: HAVE PERIODIC (timer driven) INTERRUPT THAT USES EC STATUS TO DETERMINE LIGHT FLASHING PATTERNS

static void ledInterruptInit(void) {
		 TIMER3_CLK_EN; 									// enable clock to timer 3
		 TIMER_SET_PSC(3, 71UL); 					// Program the TIM3 prescaler so TIM3 counts in useconds (tick rate)
		 TIMER_EN(3); 										// enables counter on TIM3
		 TIMER_SET_ARR(3, (1000000UL / 4));  		// Program TIM3 ARR to  period ( useconds) (ticks before reload) // Trigger 4 times a second
		 TIMER_SET_DIR(3, TIMER_DIR_UP); 	// Set TIM3 counting direction to up-counting
		 TIM3->CR1 |= TIM_CR1_ARPE;
	     TIMER_MAINOUT_EN(3);							// Set Main Output Enable (MOE) in BDTR

		 TIM3->DIER |= TIM_DIER_UIE; // enable update interrupt

		 TIM3->EGR |= TIM_EGR_UG;	  // Force an Update Event to preload all the registers
		 TIM3->CR1 |= TIM_CR1_CEN;   // Enable TIM3 to start counting
		 NVIC_SetPriority(EXTI15_10_IRQn, 1);
		 NVIC_EnableIRQ(TIM3_IRQn);			// Enable TIM3 interrupt line (PI Control System Handler Trigger)
	
	

}

#if CONTROLLER_TYPE == SUPER
#define CAN_ID 0x100
uint8_t SC_Enable(uint8_t enable) {

}

uint8_t SC_FloorReq(uint8_t floor) {

}

#endif

#if CONTROLLER_TYPE == CAR
#define CAN_ID 0x200
#define CC_FLOOR_REQ_1 0x05 // Car requests floor 1
#define CC_FLOOR_REQ_2 0x06 // Car requests floor 2
#define CC_FLOOR_REQ_3 0x07 // Car requests floor 3

uint8_t CC_FloorReq() { // Make floor request


}
#endif

// Include this code if controller is a floor controller
#if CONTROLLER_TYPE >= FLOOR1 
void swLightControl() {
	if (BUTTON == BUTTON_PRESSED) {
		if (RxData[0] == 0xDEEDBEEF) {

		}
	}
}

uint8_t floorReq(void) { // Register floor request

	return 0;

}



#if CONTROLLER_TYPE == FLOOR1
#define CAN_ID 0x201
#define FLOOR_CALL		0x01

void processMsg() { // process most recent message and clear flag
	if (RxData[0] == EC_POS_1) {
		lightStatus = SOLID;
	}

}


#endif

#if CONTROLLER_TYPE == FLOOR2
#define CAN_ID 0x202
#define FLOOR_CALL		0x01

void processMsg() { // process most recent message and clear flag
	if (RxData[0] == EC_POS_2) {
		lightStatus = SOLID;
	}

}

#endif

#if CONTROLLER_TYPE == FLOOR3
#define CAN_ID 0x203
#define FLOOR_CALL		0x01

void processMsg() { // process most recent message and clear flag
	if (RxData[0] == EC_POS_3) {
		lightStatus = SOLID;
	}

}

#endif

static void switchInit(void) { // Initialize hardware used by elevator switches
	// Enable GPIO Bank
	GPIO_EN(C);
	GPIO_EN(A);

	// Set GPIO Mode
	GPIO_SET_MODE(C, 2, GPIO_MODE_GPOM); // Debounce2 Pin
	GPIO_SET_MODE(C, 7, GPIO_MODE_GPOM); // Switch_LED2_Pin
	GPIO_SET_MODE(C, 8, GPIO_MODE_GPOM); // Switch_LED3_Pin
	GPIO_SET_MODE(C, 10, GPIO_MODE_GPOM); //Switch_LED_Pin
	GPIO_SET_MODE(C, 13, GPIO_MODE_AF); //B1 Pin Setup

	GPIO_SET_MODE(C, 12, GPIO_MODE_IN); // Floor switch pin
	
	GPIO_SET_MODE(A, 0, GPIO_MODE_GPOM); // Floor1LED
	GPIO_SET_MODE(A, 1, GPIO_MODE_GPOM); // Floor2LED
	GPIO_SET_MODE(A, 4, GPIO_MODE_GPOM); // Floor3LED
	GPIO_SET_MODE(A, 5, GPIO_MODE_GPOM); // LD2
	GPIO_SET_MODE(A, 15, GPIO_MODE_GPOM); // Debounce1

	// Set GPIO Output Type
	GPIO_SET_OTYPE(C, 2, GPIO_OTYPE_PUSHPULL); // Debounce2 Pin
	GPIO_SET_OTYPE(C, 7, GPIO_OTYPE_PUSHPULL); // Switch_LED2_Pin
	GPIO_SET_OTYPE(C, 8, GPIO_OTYPE_PUSHPULL); // Switch_LED3_Pin
	GPIO_SET_OTYPE(C, 10, GPIO_OTYPE_PUSHPULL); //Switch_LED_Pin

	GPIO_SET_OTYPE(A, 0, GPIO_OTYPE_PUSHPULL); // Floor1LED
	GPIO_SET_OTYPE(A, 1, GPIO_OTYPE_PUSHPULL); // Floor2LED
	GPIO_SET_OTYPE(A, 4, GPIO_OTYPE_PUSHPULL); // Floor3LED
	GPIO_SET_OTYPE(A, 5, GPIO_OTYPE_PUSHPULL); // LD2
	GPIO_SET_OTYPE(A, 15, GPIO_OTYPE_PUSHPULL); // Debounce1

	// Set Output Speed
	GPIO_SET_OSPEED(C, 2, GPIO_OSPEED_LOW); // Debounce2 Pin
	GPIO_SET_OSPEED(C, 7, GPIO_OSPEED_LOW); // Switch_LED2_Pin
	GPIO_SET_OSPEED(C, 8, GPIO_OSPEED_LOW); // Switch_LED3_Pin
	GPIO_SET_OSPEED(C, 10, GPIO_OSPEED_LOW); //Switch_LED_Pin
	
	GPIO_SET_OSPEED(A, 0, GPIO_OSPEED_LOW);  // Floor1LED
	GPIO_SET_OSPEED(A, 1, GPIO_OSPEED_LOW); // Floor2LED
	GPIO_SET_OSPEED(A, 4, GPIO_OSPEED_LOW); // Floor3LED 
	GPIO_SET_OSPEED(A, 5, GPIO_OSPEED_LOW); // LD2
	GPIO_SET_OSPEED(A, 15, GPIO_OSPEED_LOW); // Debounce1

	// Set Pullup/Pulldown Type
	GPIO_SET_PUPD(C, 2, GPIO_PUPD_NONE); // Debounce2 Pin
	GPIO_SET_PUPD(C, 7, GPIO_PUPD_NONE); // Switch_LED2_Pin
	GPIO_SET_PUPD(C, 8, GPIO_PUPD_NONE); // Switch_LED3_Pin
	GPIO_SET_PUPD(C, 10, GPIO_PUPD_NONE); //Switch_LED_Pin
	GPIO_SET_PUPD(C, 13, GPIO_PUPD_NONE); //B1 Pin Setup

	GPIO_SET_PUPD(C, 12, GPIO_PUPD_NONE);

	GPIO_SET_PUPD(A, 0, GPIO_PUPD_NONE); // Floor1LED
	GPIO_SET_PUPD(A, 1, GPIO_PUPD_NONE); // Floor2LED
	GPIO_SET_PUPD(A, 4, GPIO_PUPD_NONE); // Floor3LED
	GPIO_SET_PUPD(A, 5, GPIO_PUPD_NONE); // LD2
	GPIO_SET_PUPD(A, 15, GPIO_PUPD_NONE); // Debounce1
	
	GPIO_SET_AF(GPIO_AFR_HI, C, 13-8, GPIO_AF_4);

}

void elevatorInit(void) { // initialize required subsystems
	switchInit();
	ledInterruptInit();
	canInit();

	// Enable Button Interrupts
	// Setup Stepper Limit Switch Inturrupts
	// Set SYSCFG EXTICR4 Bits 7-4 to select pin PC13 as interrupt source
	 RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // enable syscfg clock
	 SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI13_PC; // select interuppt source
	 EXTI_SET_IMR1(IMR1_BIT_13); 		// Set interrupt mask register for EXTI line 13
	 EXTI_SET_RTSR1(RTSR1_BIT_13);		// Set rising edge interrupt trigger register for EXTI line 13
	 NVIC_SetPriority(EXTI15_10_IRQn, 0);
	 NVIC_EnableIRQ(EXTI15_10_IRQn);	// Enable EXTI lines 15 to 10 in NVIC

	// Populate CAN_TxMsg default values
	CAN_TxMsg.id = CAN_ID;
	CAN_TxMsg.len = 0x02; // set DLC to 2 for 2 bytes of data
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
}

void msgRx() { // wrapper around low level CAN, abstracts some stuff for the user (unneeded messages are read by interrupt)
	//Receive
	//if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			//Error_Handler(); // Reception error
	//}
}

void msgTx(unsigned char data[8]) { // transmit a CAN message
	for (int i = 0; i < 8; i++) {
		CAN_TxMsg.data[i] = data[i];
	}
	canWrMsg(&CAN_TxMsg);
	newMsg = 1;

}

// void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) { // receive messae
// 	msgRx();

// 	//	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
// //		Error_Handler(); // Reception error
// //	}
// }
void EXTI15_10_IRQHandler(void) {
	UARTputc('Q', USART2);
	BUTTON = BUTTON_PRESSED;
	GPIO_SET_ODR(C, 10, 1);
	
	EXTI->PR |= EXTI_PR_PR13; // clear pending bit by writing a 1 to it.

}

void TIM3_IRQHandler(void) { //! <pre>Breif Description: PI Control system trigger handler.</pre>
	 /*! <pre>
	 *  Detailed Descritpion:
	 *	Function Name:        TIM6_IRQHandler
	 *	Function Type:        void
	 *	Function Inputs:      none
	 *	Function Outputs:     none
	 *	Error Handling:       No
	 *	Function Description: This function triggers when TIM6 reloads
	 *                        to zero. This triggers state change calculation for the
	 *                        indicator lights
	 *</pre>
	*/
		UARTputc('T', USART2);
		if (BIT_IS_SET(GPIOC->ODR, GPIO_PIN_13)) {
			GPIO_SET_ODR(C, 13, 0);
		} else {
			GPIO_SET_ODR(C, 13, 1);
		}
		//TGLBITS(GPIOC, GPIO_PIN_13);
		//HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		//HAL_Delay(2000);
		TIMER_CLEAR_UIF(3); // Clear update interrupt flag after handling
	
 }

#endif
