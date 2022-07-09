// Copyright 2021 Garnet Koebel
// All Rights Reserved

//! Mobile Robotics Platform - GPIO Subsystem Definitions

/*
 * 
 *	Name: 				GPIO.c
 *	Author:				Garnet Koebel
 *	Version: 			V0.1	
 *	Created:			2021-02-13
 *	Last Update:	2021-09-13
 *	Description:	This file contains all the function definitions relating to GPIO setup
 *
 *	Update for 2021-02-14
 *	- Fixed a bug with seting GPIOC modes (off by one when setting pins)
 *	- Added previously missing pin settings (for pullups and speed)
 *		
 *	Update for 2021-09-13
 *	- Added copyright information to file
 */
 
 #include "GPIO.h"
 #include "macros.h"
 
void GPIOinit() {
	/*
	 *	Function Name: 			GPIOinit
	 *	Function Type: 			void
	 *	Function Inputs: 		none
	 *	Function Outputs: 	none
	 *	Error Handling:			No
	 *	Function Description:	This function initializes GPIO's required for the main program
	 *
	*/

}
