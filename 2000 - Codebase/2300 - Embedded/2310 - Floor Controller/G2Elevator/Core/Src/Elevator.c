// This is the implementation for the
#include "Elevator.h"
#include "stm32f3xx_hal.h"

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
uint8_t BUTTON = NO_BUTTON_PRESSED;

// NOTE: HAVE PERIODIC (timer driven) INTERRUPT THAT USES EC STATUS TO DETERMINE LIGHT FLASHING PATTERN

void msgRx() {
	CAN_RxHeaderTypeDef	RxHeader;

	//Receive
	if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK) {
			Error_Handler(); // Reception error
	}
}

void msgTx(uint8_t canId, uint8_t msg) {
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t			TxMailbox;
	uint8_t				TxData[8];

	//Transmit
	TxData[0] = msg;

	if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
		Error_Handler();
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
		if (RxData[0] == )
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

#endif
