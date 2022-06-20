
// Copyright 2021 Garnet Koebel
// All Rights Reserved

/*
 * 
 *	Name: 				macros.h
 *	Author:				Garnet Koebel
 *	Version: 			V0.1	
 *	Created:			2021-02-20
 *	Last Update:	2021-09-13
 *	Description:	Various useful macros
 
    Update for 2021-04-30
		- Added utility macros
		
		Update for 2021-09-13
		- Added copyright information to file
 */
 
 #ifndef MACROS_H
 #define MACROS_H
  #include <stm32f303xe.h>
	
 // Defined Names
 #define HI 1
 #define LO 0
 #define ASCII_ACK 0x06 // ASCII Acknowledge Character
 #define ASCII_NAK 0x15 // ASCII Negative Acknowledge Character
	
 // forces bits identified by the mask to take on the values of the matching bits in value.
 // Credit to Bill Stefanuk for this macro.
 #define FORCE_BITS(port, mask, value) (port) = (((port) & (~(mask))) | ((value) & (mask))) 
 #define CLR_BITS( port, bits )		((port) &= ~(bits))
 #define SET_BITS( port, bits )		((port) |= (bits))
 
 // Helper macros (lessens chance of typos when making macros)
 #define GPIO(port) GPIOx(port)
 #define GPIOx(port) GPIO##port // stick GPIO and port together after substituting (use this to insert GPIOport into macros)
 #define TIM(timer) TIMx(timer)
 #define TIMx(timer) TIM##timer
 
 // UTILITY MACROS
 #define LO_NYBBLE(val) ((val)&0x0F)     // Get low nybble of a byte
 #define HI_NYBBLE(val) (((val)>>4)&0x0F) // Get high nybble of a byte
 #define CLRBITS(reg, bit) (reg) &= ~(bit) // Clear bit at reg
 #define SETBITS(reg, bit) (reg) |= (bit)  // Clear bit at reg
 
 //GPIO MACROS
 #define GPIO_EN(port) RCC->AHBENR |= RCC_AHBENR_GPIO##port##EN // many a minute was lost to make this macro // simple substitution to make enabling GPIO ports easier
 #define GPIO_DEEN(port) RCC->AHBENR &= ~RCC_AHBENR_GPIO##port##EN // disable a GPIO port
 #define GPIO_SET_OTYPE(port, pin, type) FORCE_BITS(GPIOx(port)->OTYPER, (1UL << (pin)), ((type) << (pin)))				// allows setting of GPIO output type
 #define GPIO_SET_OSPEED(port, pin, speed) FORCE_BITS(GPIOx(port)->OSPEEDR, (1UL << (pin)), ((speed) << (pin)))		// allows setting of GPIO output speed
 #define GPIO_SET_MODE(port, pin, mode) FORCE_BITS(GPIOx(port)->MODER, (3UL << ((pin)*2)), ((mode) << ((pin)*2))) // allows setting of GPIO mode
 #define GPIO_SET_PUPD(port, pin, mode) FORCE_BITS(GPIOx(port)->PUPDR, (3UL << ((pin)*2)), ((mode) << ((pin)*2)))  // allows setting of GPIO pullup/down resistors
 #define GPIO_SET_AF(reg, port, pin, mode) FORCE_BITS(GPIOx(port)->AFR[reg], (15UL << ((pin)*4)), ((mode) << ((pin)*4))) // allows setting of GPIO alternate functions // This could be improved by auto setting the 'reg' value
 #define GPIO_SET_AF_LO(port, pin, mode) FORCE_BITS(GPIOx(port)->AFR[0], (15UL << ((pin)*4)), ((mode) << ((pin)*4))) // allows setting of GPIO alternate functions // This could be improved by auto setting the 'reg' value
 #define GPIO_SET_AF_HI(port, pin, mode) FORCE_BITS(GPIOx(port)->AFR[1], (15UL << (((pin)-8)*4)), ((mode) << ((pin)*4))) // allows setting of GPIO alternate functions // This could be improved by auto setting the 'reg' value
 #define GPIO_SET_ODR(port, pin, val) FORCE_BITS(GPIOx(port)->ODR, 1UL << ((pin)), ((val) << ((pin))))
 #define GPIO_GET_IDR(port, pin) GPIOx(port)->IDR & 0x##pin
 
 //TIMER MACROS
 #define TIMER_DIR_MSK 0x0010
 #define TIMER_DIR_UP 0UL
 #define TIMER_DIR_DOWN 1UL
 
 #define TIMER_MAINOUTEN_MSK 0x8000 // select MOE bit
 
 #define TIMER_PWM_MODE_1
 
 #define TIMER_ARR_MSK 0xFFFF
 
 #define TIMER_CLK_EN(timer) RCC->APB2ENR |= RCC_APB2ENR_TIM##timer##EN // allows for enabling timers
 #define TIMER7_CLK_EN(timer) RCC->APB1ENR |= RCC_APB1ENR_TIM##timer##EN // allows for enabling timers
 #define TIMER6_CLK_EN(timer) RCC->APB1ENR |= RCC_APB1ENR_TIM##timer##EN // allows for enabling timers
 #define TIMER2_CLK_EN RCC->APB1ENR |= RCC_APB1ENR_TIM2EN // allows for enabling timers
 #define TIMER3_CLK_EN RCC->APB1ENR |= RCC_APB1ENR_TIM3EN // allows for enabling timers
 #define TIMER_CLK_DEEN(timer) RCC->APB2ENR &= ~RCC_APB2ENR_TIM##timer##EN // disable a timer
 #define TIMER_EN(timer) TIMx(timer)->CR1 |= 1UL// allows for enabling timers
 #define TIMER_DEEN(timer) TIM(timer)->CR1 &= ~1UL // disable a timer
 
 #define TIMER_SET_PSC(timer, value) TIMx(timer)->PSC |= value
 #define TIMER_SET_ARR(timer, value) FORCE_BITS((TIMx(timer)->ARR), (TIMER_ARR_MSK), (value))
 #define TIMER_SET_DIR(timer, dir) FORCE_BITS(TIMx(timer)->CR1, TIMER_DIR_MSK, dir) // sets timer directions
 #define TIMER_MAINOUT_EN(timer) TIMx(timer)->BDTR |= TIMER_MAINOUTEN_MSK
			
 #define TIMER_SEL_PWM_MODE(timer, mode) TIMx(timer)->CMR1 
 #define TIMER_EN_OUT_COMPARE_PRELOAD(timer) TIMx(timer)->CCMR1
 #define TIMER_EN_COMPLEM_OUT_CHANNEL(timer) TIMx(timer)->CCER
 //#define TIMER_SET_ACTIVE_HI(timer) TIMx(timer)->CCER &= ~TIM_CCER_CC2P
 //#define TIMER_SET_ACTIVE_LO(timer) TIMx(timer)-CCER |= ~TIM_CCER_CC2P
 //#define TIMER_SET_INITIAL_VALUE(timer, value) TIMx(timer)->CCR2
 
 #define TIMER_CLEAR_UIF(timer) CLR_BITS(TIMx(timer)->SR, TIM_SR_UIF) //clears the UIF after an interupt
 
 #define BIT_IS_SET( port, bits )		((port) & (bits))
 #define BIT_NOT_SET( port, bits )		((port) & ()
 
 // EXTI MACROS
 // IMR1 Register Bit Masks
 #define IMR1_BIT_0		0x00000001U
 #define IMR1_BIT_1		0x00000002U
 #define IMR1_BIT_2		0x00000004U
 #define IMR1_BIT_3		0x00000008U
 #define IMR1_BIT_4		0x00000010U
 #define IMR1_BIT_5		0x00000020U
 #define IMR1_BIT_6		0x00000040U
 #define IMR1_BIT_7		0x00000080U
 #define IMR1_BIT_8		0x00000100U
 #define IMR1_BIT_9		0x00000200U
 #define IMR1_BIT_10	0x00000400U
 #define IMR1_BIT_11	0x00000800U
 #define IMR1_BIT_12	0x00001000U
 #define IMR1_BIT_13	0x00002000U
 #define IMR1_BIT_14	0x00004000U
 #define IMR1_BIT_15	0x00008000U
 #define IMR1_BIT_16	0x00010000U
 #define IMR1_BIT_17	0x00020000U
 #define IMR1_BIT_18	0x00040000U
 #define IMR1_BIT_19	0x00080000U
 #define IMR1_BIT_20	0x00100000U
 #define IMR1_BIT_21	0x00200000U
 #define IMR1_BIT_22	0x00400000U
 #define IMR1_BIT_23	0x00800000U
 #define IMR1_BIT_24	0x01000000U
 #define IMR1_BIT_25	0x02000000U
 #define IMR1_BIT_26	0x04000000U
 #define IMR1_BIT_28	0x08000000U
 #define IMR1_BIT_29	0x10000000U
 #define IMR1_BIT_30	0x20000000U
 #define IMR1_BIT_31	0x40000000U
 
  // EMR1 Register Bit Masks
 #define EMR1_BIT_0		0x00000001U
 #define EMR1_BIT_1		0x00000002U
 #define EMR1_BIT_2		0x00000004U
 #define EMR1_BIT_3		0x00000008U
 #define EMR1_BIT_4		0x00000010U
 #define EMR1_BIT_5		0x00000020U
 #define EMR1_BIT_6		0x00000040U
 #define EMR1_BIT_7		0x00000080U
 #define EMR1_BIT_8		0x00000100U
 #define EMR1_BIT_9		0x00000200U
 #define EMR1_BIT_10	0x00000400U
 #define EMR1_BIT_11	0x00000800U
 #define EMR1_BIT_12	0x00001000U
 #define EMR1_BIT_13	0x00002000U
 #define EMR1_BIT_14	0x00004000U
 #define EMR1_BIT_15	0x00008000U
 #define EMR1_BIT_16	0x00010000U
 #define EMR1_BIT_17	0x00020000U
 #define EMR1_BIT_18	0x00040000U
 #define EMR1_BIT_19	0x00080000U
 #define EMR1_BIT_20	0x00100000U
 #define EMR1_BIT_21	0x00200000U
 #define EMR1_BIT_22	0x00400000U
 #define EMR1_BIT_23	0x00800000U
 #define EMR1_BIT_24	0x01000000U
 #define EMR1_BIT_25	0x02000000U
 #define EMR1_BIT_26	0x04000000U
 #define EMR1_BIT_28	0x08000000U
 #define EMR1_BIT_29	0x10000000U
 #define EMR1_BIT_30	0x20000000U
 #define EMR1_BIT_31	0x40000000U
 
  // RTSR1 Register Bit Masks
 #define RTSR1_BIT_0		0x00000001U
 #define RTSR1_BIT_1		0x00000002U
 #define RTSR1_BIT_2		0x00000004U
 #define RTSR1_BIT_3		0x00000008U
 #define RTSR1_BIT_4		0x00000010U
 #define RTSR1_BIT_5		0x00000020U
 #define RTSR1_BIT_6		0x00000040U
 #define RTSR1_BIT_7		0x00000080U
 #define RTSR1_BIT_8		0x00000100U
 #define RTSR1_BIT_9		0x00000200U
 #define RTSR1_BIT_10		0x00000400U
 #define RTSR1_BIT_11		0x00000800U
 #define RTSR1_BIT_12		0x00001000U
 #define RTSR1_BIT_13		0x00002000U
 #define RTSR1_BIT_14		0x00004000U
 #define RTSR1_BIT_15		0x00008000U
 #define RTSR1_BIT_16		0x00010000U
 #define RTSR1_BIT_17		0x00020000U
 #define RTSR1_BIT_18		0x00040000U
 #define RTSR1_BIT_19		0x00080000U
 #define RTSR1_BIT_20		0x00100000U
 #define RTSR1_BIT_21		0x00200000U
 #define RTSR1_BIT_22		0x00400000U
 #define RTSR1_BIT_23		0x00800000U
 #define RTSR1_BIT_24		0x01000000U
 #define RTSR1_BIT_25		0x02000000U
 #define RTSR1_BIT_26		0x04000000U
 #define RTSR1_BIT_28		0x08000000U
 #define RTSR1_BIT_29		0x10000000U
 #define RTSR1_BIT_30		0x20000000U
 #define RTSR1_BIT_31		0x40000000U
 
  // FTSR1 Register Bit Masks
 #define FTSR1_BIT_0		0x00000001U
 #define FTSR1_BIT_1		0x00000002U
 #define FTSR1_BIT_2		0x00000004U
 #define FTSR1_BIT_3		0x00000008U
 #define FTSR1_BIT_4		0x00000010U
 #define FTSR1_BIT_5		0x00000020U
 #define FTSR1_BIT_6		0x00000040U
 #define FTSR1_BIT_7		0x00000080U
 #define FTSR1_BIT_8		0x00000100U
 #define FTSR1_BIT_9		0x00000200U
 #define FTSR1_BIT_10		0x00000400U
 #define FTSR1_BIT_11		0x00000800U
 #define FTSR1_BIT_12		0x00001000U
 #define FTSR1_BIT_13		0x00002000U
 #define FTSR1_BIT_14		0x00004000U
 #define FTSR1_BIT_15		0x00008000U
 #define FTSR1_BIT_16		0x00010000U
 #define FTSR1_BIT_17		0x00020000U
 #define FTSR1_BIT_18		0x00040000U
 #define FTSR1_BIT_19		0x00080000U
 #define FTSR1_BIT_20		0x00100000U
 #define FTSR1_BIT_21		0x00200000U
 #define FTSR1_BIT_22		0x00400000U
 #define FTSR1_BIT_23		0x00800000U
 #define FTSR1_BIT_24		0x01000000U
 #define FTSR1_BIT_25		0x02000000U
 #define FTS1_BIT_26		0x04000000U
 #define FTSR1_BIT_28		0x08000000U
 #define FTSR1_BIT_29		0x10000000U
 #define FTSR1_BIT_30		0x20000000U
 #define FTSR1_BIT_31		0x40000000U
 
 // Setter Macros
 #define EXTI_SET_IMR1( bit )	SETBITS(EXTI->IMR, bit)
 #define EXTI_SET_EMR1( bit ) 	SETBITS(EXTI->EMR, bit)
 #define EXTI_SET_RTSR1( bit ) SETBITS(EXTI->RTSR, bit)
 #define EXTI_SET_FTSR1( bit )	SETBITS(EXTI->FTSR, bit)
 // Clearing Macros
 #define EXTI_CLR_IMR1( bit )	CLRBITS(EXTI->IMR, bit)
 #define EXTI_CLR_EMR1( bit )	CLRBITS(EXTI->EMR, bit)
 #define EXTI_CLR_RTSR1( bit ) CLRBITS(EXTI->RTSR, bit)
 #define EXTI_CLR_FTSR1( bit ) CLRBITS(EXTI->FTSR, bit)
 
 #endif
