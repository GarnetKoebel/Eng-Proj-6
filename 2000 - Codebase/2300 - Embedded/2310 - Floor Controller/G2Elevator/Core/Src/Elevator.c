// This is the implementation for the
#include "Elevator.h"
#include "stm32f3xx_hal.h"
#include <stm32f303xe.h>
#include "macros.h"

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
static uint8_t		   newMsg = 0; // tracks if a new message has come in

static uint8_t		   lightStatus = OFF;
static uint8_t		   ecStatus = EC_STATUS_DEEN; // Keeps track of elevator controller status
uint8_t 		   BUTTON = BUTTON_NOT_PRESSED;


static CAN_TxHeaderTypeDef TxHeader;
CAN_HandleTypeDef hcan;

// NOTE: HAVE PERIODIC (timer driven) INTERRUPT THAT USES EC STATUS TO DETERMINE LIGHT FLASHING PATTERNS

static void initLEDInterrupt(void) {
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

		 NVIC_EnableIRQ(TIM3_IRQn);			// Enable TIM3 interrupt line (PI Control System Handler Trigger)

}

void msgRx() {
	CAN_RxHeaderTypeDef	RxHeader;

	//Receive
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			//Error_Handler(); // Reception error
	}
}

void msgTx(uint8_t canId, uint8_t msg) {

	uint32_t			TxMailbox;
	uint8_t				TxData[8];

	//Transmit
	TxData[0] = msg;

	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
		//Error_Handler();
	}
	newMsg = 1;

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

uint8_t floorReq() { // Register floor request

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

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) { // receive messae
	msgRx();

	//	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
//		Error_Handler(); // Reception error
//	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_13) {
		BUTTON = BUTTON_PRESSED;
	}
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

		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(2000);
		TIMER_CLEAR_UIF(3); // Clear update interrupt flag after handling

 }

void canInit(void) {
	  hcan.Instance = CAN;
	  hcan.Init.Prescaler = 32;
	  hcan.Init.Mode = CAN_MODE_NORMAL;
	  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	  hcan.Init.TimeSeg1 = CAN_BS1_4TQ;
	  hcan.Init.TimeSeg2 = CAN_BS2_4TQ;
	  hcan.Init.TimeTriggeredMode = DISABLE;
	  hcan.Init.AutoBusOff = DISABLE;
	  hcan.Init.AutoWakeUp = DISABLE;
	  hcan.Init.AutoRetransmission = DISABLE;
	  hcan.Init.ReceiveFifoLocked = DISABLE;
	  hcan.Init.TransmitFifoPriority = DISABLE;
	  if (HAL_CAN_Init(&hcan) != HAL_OK)
	  {
	    //Error_Handler();
	  }
	  /* USER CODE BEGIN CAN_Init 2 */
	  CAN_FilterTypeDef filter;

	  filter.FilterBank = 0;
	  filter.FilterIdHigh = 0x0100 << 5;
	  filter.FilterIdLow = 0x0000;
	  filter.FilterMaskIdHigh = 0xFFC << 5;
	  filter.FilterMaskIdLow = 0x0000;
	  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	  filter.FilterMode = CAN_FILTERMODE_IDMASK;
	  filter.FilterScale = CAN_FILTERSCALE_32BIT;
	  filter.FilterActivation = ENABLE;
	  filter.SlaveStartFilterBank = 0;

	  if (HAL_CAN_ConfigFilter(&hcan, &filter) != HAL_OK) {
		 // Error_Handler();
	  }

	  if (HAL_CAN_Start(&hcan) != HAL_OK) {
		//  Error_Handler();
	  }

	  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK) {
		 // Error_Handler();
	  }

	  TxHeader.IDE = CAN_ID_STD;
	  TxHeader.ExtId = 0x00;
	  TxHeader.StdId = CAN_ID;
	  TxHeader.RTR = CAN_RTR_DATA;
	  TxHeader.DLC = 1;
	  TxHeader.TransmitGlobalTime = DISABLE;
}

#endif
