// Copyright 2021 Garnet Koebel
// All Rights Reserved

/*
 * 
 *	Name: 				GPIO.h
 *	Author:				Garnet Koebel
 *	Version: 			V0.1	
 *	Created:			2021-02-13
 *	Last Update:	2021-09-13
 *	Description:	This file contains all the function definitions relating to GPIO setup as well as various defined symbols for setting GPIO registers
		
		Update for 2021-03-01
		- Added definition for PB0
		
		Update for 2021-09-13
		- Added copyright information to file
 */
 
 #ifndef GPIO_H
 #define GPIO_H
 
 #include <stm32f303xe.h>
 
  // GPIO REGISTER DEFINES
 
 // pin definitions (will make more useful in future)
 //NOTE: could make it so using these macros expand to the register for each pin (e.g PA0 expands to GPIO(A, 0, etc..))
 
 #define PA0 0 // Port A Pin/Bit 0
 #define PA1 1 // Port A Pin/Bit 1
 #define PA2 2 // Port A Pin/Bit 2
 #define PA3 3 // Port A Pin/Bit 3
 #define PA4 4 // Port A Pin/Bit 4
 #define PA5 5 // Port A Pin/Bit 5
 #define PA6 6 // Port A Pin/Bit 6
 #define PA7 7 // Port A Pin/Bit 7
 #define PA8 8 // Port A Pin/Bit 8
 #define PA9 9 // Port A Pin/Bit 9
 #define PA10 10 // Port A Pin/Bit 10
 #define PA11 11 // Port A Pin/Bit 11
 #define PA12 12 // Port A Pin/Bit 12
 #define PA13 13 // Port A Pin/Bit 13
 #define PA14 14 // Port A Pin/Bit 14
 #define PA15 15 // Port A Pin/Bit 15
 
 #define PB0 0 // Port B Pin/Bit 0
 #define PB1 1 // Port B Pin/Bit 1
 #define PB2 2 // Port B Pin/Bit 2
 #define PB3 3 // Port B Pin/Bit 3
 #define PB4 4 // Port B Pin/Bit 4
 #define PB5 5 // Port B Pin/Bit 5
 #define PB6 6 // Port B Pin/Bit 6
 #define PB7 7 // Port B Pin/Bit 7
 #define PB8 8 // Port B Pin/Bit 8
 #define PB9 9 // Port B Pin/Bit 9
 #define PB10 10 // Port B Pin/Bit 10
 #define PB11 11 // Port B Pin/Bit 11
 #define PB12 12 // Port B Pin/Bit 12
 #define PB13 13 // Port B Pin/Bit 13
 #define PB14 14 // Port B Pin/Bit 14
 #define PB15 15 // Port B Pin/Bit 15
 
 #define PC0 0 // Port C Pin/Bit 0
 #define PC1 1 // Port C Pin/Bit 1
 #define PC2 2 // Port C Pin/Bit 2
 #define PC3 3 // Port C Pin/Bit 3
 #define PC4 4 // Port C Pin/Bit 4
 #define PC5 5 // Port C Pin/Bit 5
 #define PC6 6 // Port C Pin/Bit 6
 #define PC7 7 // Port C Pin/Bit 7
 #define PC8 8 // Port C Pin/Bit 8
 #define PC9 9 // Port C Pin/Bit 9
 #define PC10 10 // Port C Pin/Bit 10
 #define PC11 11 // Port C Pin/Bit 11
 #define PC12 12 // Port C Pin/Bit 12
 #define PC13 13 // Port C Pin/Bit 13 
 #define PC14 14 // Port C Pin/Bit 14
 #define PC15 15 // Port C Pin/Bit 15
 
 #define PD0 	0 // Port D Pin/Bit 0
 #define PD1 	1 // Port D Pin/Bit 1
 #define PD2 	2 // Port D Pin/Bit 2
 #define PD3 	3 // Port D Pin/Bit 3
 #define PD4 	4 // Port D Pin/Bit 4
 #define PD5 	5 // Port D Pin/Bit 5
 #define PD6 	6 // Port D Pin/Bit 6
 #define PD7 	7 // Port D Pin/Bit 7
 #define PD8 	8 // Port D Pin/Bit 8
 #define PD9 	9 // Port D Pin/Bit 9
 #define PD10 10 // Port D Pin/Bit 10
 #define PD11 11 // Port D Pin/Bit 11
 #define PD12 12 // Port D Pin/Bit 12
 #define PD13 13 // Port D Pin/Bit 13
 #define PD14 14 // Port D Pin/Bit 14
 #define PD15 15 // Port D Pin/Bit 15
 
 // GPIO Pin Bit Definitions
 #define GPIO_PIN_0  0x00000001U                         
 #define GPIO_PIN_1  0x00000002U                         
 #define GPIO_PIN_2  0x00000004U                         
 #define GPIO_PIN_3  0x00000008U                         
 #define GPIO_PIN_4  0x00000010U                         
 #define GPIO_PIN_5  0x00000020U                         
 #define GPIO_PIN_6  0x00000040U                         
 #define GPIO_PIN_7  0x00000080U                         
 #define GPIO_PIN_8  0x00000100U                         
 #define GPIO_PIN_9  0x00000200U                         
 #define GPIO_PIN_10 0x00000400U                         
 #define GPIO_PIN_11 0x00000800U                         
 #define GPIO_PIN_12 0x00001000U                         
 #define GPIO_PIN_13 0x00002000U                         
 #define GPIO_PIN_14 0x00004000U                         
 #define GPIO_PIN_15 0x00008000U
 
 // definitions for various modes (to reduce use of "magic numbers")
 #define GPIO_MODE_IN 0UL 	// 0UL translates to 0b00 which is the input mode bit pattern
 #define GPIO_MODE_GPOM 1UL	// 1UL translates to 0b01 which is the General Purpose Output Mode bit pattern
 #define GPIO_MODE_AF 2UL 	// 2UL translates to 0b10 which is the alternate function bit pattern.
 #define GPIO_MODE_ANA 3UL	// 3UL translates to 0b11 which is the analog mode bit pattern
 
 // Alternate Function Register "Magic Number Removal"
 #define GPIO_AFR_LO 0
 #define GPIO_AFR_HI 1
 
 // Alternate function defines, xUL translates to the bitpattern for that function
 #define GPIO_AF_0 0UL
 #define GPIO_AF_1 1UL
 #define GPIO_AF_2 1UL
 #define GPIO_AF_3 2UL
 #define GPIO_AF_4 4UL
 #define GPIO_AF_5 5UL
 #define GPIO_AF_6 6UL
 #define GPIO_AF_7 7UL
 #define GPIO_AF_8 8UL
 #define GPIO_AF_9 9UL
 #define GPIO_AF_10 10UL
 #define GPIO_AF_11 11UL
 #define GPIO_AF_12 12UL
 #define GPIO_AF_13 13UL
 #define GPIO_AF_14 14UL
 #define GPIO_AF_15 15UL
 
 // GPIO Output Types
 #define GPIO_OTYPE_PUSHPULL 0UL // 0UL translates to 0b00 which is the push pull bit pattern
 #define GPIO_OTYPE_OPNDRAIN 1UL // 1UL translates to 0b01 which is the open drain bit pattern
 
 // GPIO Speed Types
 #define GPIO_OSPEED_LOW 0UL // 0UL translates to 0b00 which is the low speed bit pattern
 #define GPIO_OSPEED_MED 1UL // 0UL translates to 0b01 which is the medium speed bit pattern
 #define GPIO_OSPEED_HIH 3UL // 0UL translates to 0b11 which is the high speed bit pattern
 
 // GPIO Pullup/down Types
 #define GPIO_PUPD_NONE 0UL // 0UL translates to 0b00 which is the no pullup/down bit pattern
 #define GPIO_PUPD_UP 1UL		// 1UL translates to 0b01 which is the pull up bit pattern
 #define GPIO_PUPD_DOWN 2UL // 2UL translates to 0b10 which is the pull down bit pattern
 
 
 void GPIOinit(void);
 
 
 //Future functions
 //void GPIO_CLK_EN(GPIO_TypeDef * GPIOx);
 //void setPinMode(pin);
 //void setPinSpeed(pin);
 //void setPinState(pin); // write high / low
 
 #endif
