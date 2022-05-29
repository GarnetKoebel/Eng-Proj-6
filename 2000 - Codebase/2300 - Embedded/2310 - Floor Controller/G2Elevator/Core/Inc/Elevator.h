// This is the header for elevator control functions.
// It is used by all three controllers with the relevent
// Control code selected by CONTROLLER_TYPE

#ifndef ELEVATOR_H
#define ELEVATOR_H

#include "stm32f3xx_hal.h"

#define SUPER    0
#define CAR      1
#define FLOOR1   2
#define FLOOR2   3
#define FLOOR3   4

#define CONTROLLER_TYPE FLOOR3

// Common Functions
void cmdTX(uint8_t canId, uint8_t dlc, uint8_t cmd); // wrapper around HAL CAN TX function

#if CONTROLLER_TYPE == SUPER
#define CAN_ID 0x100

uint8_t SC_Enable(uint8_t enable);
uint8_t SC_FloorReq(uint8_t floor);

#endif

#if CONTROLLER_TYPE == CAR
#define CAN_ID 0x200

uint8_t CC_FloorReq(); // get Elevator Controller Status

#endif

// Include this code if controller is a floor controller
#if CONTROLLER_TYPE >= FLOOR1 
uint8_t getFloorButtonState(); // check if floor call has been requested
uint8_t floorReq();

#if CONTROLLER_TYPE == FLOOR1
#define CAN_ID 0x201
#endif

#if CONTROLLER_TYPE == FLOOR2
#define CAN_ID 0x202
#endif

#if CONTROLLER_TYPE == FLOOR3
#define CAN_ID 0x203
#endif

#endif





#endif
