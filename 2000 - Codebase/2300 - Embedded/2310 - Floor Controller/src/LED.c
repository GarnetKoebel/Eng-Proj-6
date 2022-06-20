#include "LED.h"


//******************************************************************************************
// Nucleo-64 STM32F303RE User LED: 
//  LD5 Green = PA5
//******************************************************************************************

void LED_Init(void){
	/* Enable GPIOs clock */ 	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	//Sets up the Timer 7
	TIMER7_CLK_EN(7); 					// enable clock to timer 1
	TIMER_SET_PSC(7, 719UL); 		// Program the TIM7 prescaler so TIM7 counts in �seconds
	TIMER_EN(7); 								//enables counter on TIM7
	TIMER_SET_ARR(7, 1000000UL);  // Program TIM1 ARR to 1s period (1,000,000 �seconds)
	TIM7->CR1 |= TIM_CR1_ARPE;
	TIMER_MAINOUT_EN(7);				// Set Main Output Enable (MOE) in BDTR
	TIM7->DIER |= TIM_DIER_UIE; 
	
	TIM7->EGR |= TIM_EGR_UG;	  // Force an Update Event to preload all the registers
	TIM7->CR1 |= TIM_CR1_CEN;   // Enable TIM1 to start counting
	
	//Sets up the interupts
	NVIC_EnableIRQ(TIM7_IRQn);				// Enable IRQ response by NVIC
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	// LED Green = PA5
	///////////////////////////////////////////////////////////////////////////////////////////////
	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIO_SET_MODE(A, 15, GPIO_MODE_GPOM);
	
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIO_SET_OSPEED(A, 15, GPIO_OSPEED_HIH);
	
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1) 
	GPIO_SET_OTYPE(A, 15, GPIO_OTYPE_PUSHPULL);       // Push-pull
	
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIO_SET_PUPD(A, 15, GPIO_PUPD_NONE);  // No pull-up, no pull-down
}

//******************************************************************************************
// Turn Green LED On
//******************************************************************************************
void Green_LED_On(void){
	GPIOA->ODR |= GPIO_ODR_15;
}

//******************************************************************************************
// Turn Green LED Off
//******************************************************************************************
void Green_LED_Off(void){
	GPIOA->ODR &= ~GPIO_ODR_15;
}

//******************************************************************************************
// Toggle Green LED
//******************************************************************************************
void Green_LED_Toggle(void){
	GPIOA->ODR ^= GPIO_ODR_15;
}

void TIM7_IRQHandler(void) {
	Green_LED_Toggle();
	TIMER_CLEAR_UIF(7);
}
