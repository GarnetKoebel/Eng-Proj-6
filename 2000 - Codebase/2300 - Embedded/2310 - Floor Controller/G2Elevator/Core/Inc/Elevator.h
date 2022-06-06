// This is the header for elevator control functions.
// It is used by all three controllers with the relevent
// Control code selected by CONTROLLER_TYPE

#ifndef ELEVATOR_H
#define ELEVATOR_H



#define SUPER    0
#define CAR      1
#define FLOOR1   2
#define FLOOR2   3
#define FLOOR3   4

#define CONTROLLER_TYPE FLOOR3

// Common Functions
void msgRx();
void msgTx(uint8_t canId, uint8_t msg); // wrapper around HAL CAN TX function

#if CONTROLLER_TYPE == SUPER

uint8_t SC_Enable(uint8_t enable);
uint8_t SC_FloorReq(uint8_t floor);

#endif

#if CONTROLLER_TYPE == CAR


uint8_t CC_FloorReq(); // get Elevator Controller Status

#endif

// Include this code if controller is a floor controller
#if CONTROLLER_TYPE >= FLOOR1 
uint8_t getFloorButtonState(); // check if floor call has been requested
uint8_t floorReq();

#if CONTROLLER_TYPE == FLOOR1

#endif

#if CONTROLLER_TYPE == FLOOR2

#endif

#if CONTROLLER_TYPE == FLOOR3

#endif

#endif





#endif
