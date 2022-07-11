// This is the header for elevator control functions.
// It is used by all three controllers with the relevent
// Control code selected by CONTROLLER_TYPE

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <stm32f303xe.h>
#include "CAN.h"

#define SUPER    0
#define CAR      1
#define FLOOR1   2
#define FLOOR2   3
#define FLOOR3   4

#define CONTROLLER_TYPE FLOOR1

// Common Functions
void elevatorInit(void);
void canInit(void);
void msgRx();
void msgTx(unsigned char data[8]); // wrapper around HAL CAN TX function
void processMsg();
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#if CONTROLLER_TYPE == SUPER

uint8_t SC_Enable(uint8_t enable);
uint8_t SC_FloorReq(uint8_t floor);

#endif

#if CONTROLLER_TYPE == CAR


uint8_t CC_FloorReq(void); // get Elevator Controller Status

#endif

// Include this code if controller is a floor controller
#if CONTROLLER_TYPE >= FLOOR1 
uint8_t getFloorButtonState(void); // check if floor call has been requested
uint8_t floorReq(void);

#if CONTROLLER_TYPE == FLOOR1

#endif

#if CONTROLLER_TYPE == FLOOR2

#endif

#if CONTROLLER_TYPE == FLOOR3

#endif

#endif





#endif
