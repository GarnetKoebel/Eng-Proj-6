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

 // Defined Names
 #define HI 1
 #define LO 0

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
