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

	// Enable GPIO Bank
	GPIO_EN(C);
	GPIO_EN(F);
	GPIO_EN(A);
	GPIO_EN(B);

	// Set GPIO Mode
	GPIO_SET_MODE(C, GPIO_PIN_2, GPIO_MODE_GPOM); // Debounce2 Pin
	GPIO_SET_MODE(C, GPIO_PIN_7, GPIO_MODE_GPOM); // Switch_LED2_Pin
	GPIO_SET_MODE(C, GPIO_PIN_8, GPIO_MODE_GPOM); // Switch_LED3_Pin
	GPIO_SET_MODE(C, GPIO_PIN_10, GPIO_MODE_GPOM); //Switch_LED_Pin
	GPIO_SET_MODE(C, GPIO_PIN_13, GPIO_MODE_IN); //B1 Pin Setup

	GPIO_SET_MODE(C, GPIO_PIN_12, GPIO_MODE_IN); // Floor switch pin
	
	GPIO_SET_MODE(A, GPIO_PIN_0, GPIO_MODE_GPOM); // Floor1LED
	GPIO_SET_MODE(A, GPIO_PIN_1, GPIO_MODE_GPOM); // Floor2LED
	GPIO_SET_MODE(A, GPIO_PIN_4, GPIO_MODE_GPOM); // Floor3LED
	GPIO_SET_MODE(A, GPIO_PIN_5, GPIO_MODE_GPOM); // LD2
	GPIO_SET_MODE(A, GPIO_PIN_15, GPIO_MODE_GPOM); // Debounce1

	// Set GPIO Output Type
	GPIO_SET_OTYPE(C, GPIO_PIN_2, GPIO_OTYPE_PUSHPULL); // Debounce2 Pin
	GPIO_SET_OTYPE(C, GPIO_PIN_7, GPIO_OTYPE_PUSHPULL); // Switch_LED2_Pin
	GPIO_SET_OTYPE(C, GPIO_PIN_8, GPIO_OTYPE_PUSHPULL); // Switch_LED3_Pin
	GPIO_SET_OTYPE(C, GPIO_PIN_10, GPIO_OTYPE_PUSHPULL); //Switch_LED_Pin

	GPIO_SET_OTYPE(A, GPIO_PIN_0, GPIO_OTYPE_PUSHPULL); // Floor1LED
	GPIO_SET_OTYPE(A, GPIO_PIN_1, GPIO_OTYPE_PUSHPULL); // Floor2LED
	GPIO_SET_OTYPE(A, GPIO_PIN_4, GPIO_OTYPE_PUSHPULL); // Floor3LED
	GPIO_SET_OTYPE(A, GPIO_PIN_5, GPIO_OTYPE_PUSHPULL); // LD2
	GPIO_SET_OTYPE(A, GPIO_PIN_15, GPIO_OTYPE_PUSHPULL); // Debounce1

	// Set Output Speed
	GPIO_SET_OSPEED(C, GPIO_PIN_2, GPIO_OSPEED_LOW); // Debounce2 Pin
	GPIO_SET_OSPEED(C, GPIO_PIN_7, GPIO_OSPEED_LOW); // Switch_LED2_Pin
	GPIO_SET_OSPEED(C, GPIO_PIN_8, GPIO_OSPEED_LOW); // Switch_LED3_Pin
	GPIO_SET_OSPEED(C, GPIO_PIN_10, GPIO_OSPEED_LOW); //Switch_LED_Pin
	
	GPIO_SET_OSPEED(A, GPIO_PIN_0, GPIO_OSPEED_LOW);  // Floor1LED
	GPIO_SET_OSPEED(A, GPIO_PIN_1, GPIO_OSPEED_LOW); // Floor2LED
	GPIO_SET_OSPEED(A, GPIO_PIN_4, GPIO_OSPEED_LOW); // Floor3LED 
	GPIO_SET_OSPEED(A, GPIO_PIN_5, GPIO_OSPEED_LOW); // LD2
	GPIO_SET_OSPEED(A, GPIO_PIN_15, GPIO_OSPEED_LOW); // Debounce1

	// Set Pullup/Pulldown Type
	GPIO_SET_PUPD(C, GPIO_PIN_2, GPIO_PUPD_NONE); // Debounce2 Pin
	GPIO_SET_PUPD(C, GPIO_PIN_7, GPIO_PUPD_NONE); // Switch_LED2_Pin
	GPIO_SET_PUPD(C, GPIO_PIN_8, GPIO_PUPD_NONE); // Switch_LED3_Pin
	GPIO_SET_PUPD(C, GPIO_PIN_10, GPIO_PUPD_NONE); //Switch_LED_Pin
	GPIO_SET_PUPD(C, GPIO_PIN_13, GPIO_PUPD_NONE); //B1 Pin Setup

	GPIO_SET_PUPD(C, GPIO_PIN_12, GPIO_PUPD_NONE);

	GPIO_SET_PUPD(A, GPIO_PIN_0, GPIO_PUPD_NONE); // Floor1LED
	GPIO_SET_PUPD(A, GPIO_PIN_1, GPIO_PUPD_NONE); // Floor2LED
	GPIO_SET_PUPD(A, GPIO_PIN_4, GPIO_PUPD_NONE); // Floor3LED
	GPIO_SET_PUPD(A, GPIO_PIN_5, GPIO_PUPD_NONE); // LD2
	GPIO_SET_PUPD(A, GPIO_PIN_15, GPIO_PUPD_NONE); // Debounce1

	  /* EXTI interrupt init*/
  	NVIC_SetPriority(EXTI15_10_IRQn, 0);
  	NVIC_EnableIRQ(EXTI15_10_IRQn);

}
