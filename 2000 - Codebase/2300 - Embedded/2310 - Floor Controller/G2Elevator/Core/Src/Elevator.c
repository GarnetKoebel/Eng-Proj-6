// This is the implementation for the
#include "Elevator.h"

void cmdTX(uint8_t canId, uint8_t dlc, uint8_t cmd) {
    

}

#if CONTROLLER_TYPE == SUPER
uint8_t SC_Enable(uint8_t enable) {

}

uint8_t SC_FloorReq(uint8_t floor) {

}

#endif

#if CONTROLLER_TYPE == CAR
uint8_t CC_FloorReq() { // get Elevator Controller Status


}
#endif

// Include this code if controller is a floor controller
#if CONTROLLER_TYPE >= FLOOR1 
uint8_t getFloorButtonState() { // check if floor call has been requested
	return 0;
}

uint8_t floorReq() { // Register floor request

	return 0;

}

#if CONTROLLER_TYPE == FLOOR1


#endif

#if CONTROLLER_TYPE == FLOOR2

#endif

#if CONTROLLER_TYPE == FLOOR3

#endif

#endif
