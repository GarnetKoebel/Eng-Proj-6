
#include "Utils.h"

void waitMs(uint32_t ms) {
	// This function taken directly from lecture slides. Will write interrupt based function in future.
	
	// wait for ms
	SysTick->CTRL = 0;																			// Disable SysTick
	SysTick->LOAD = ((SystemCoreClock/8) / 1000UL) * ms;  // Set reload register
  SysTick->VAL = 0;             	    										// Reset the SysTick counter value
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; 								// Start SysTick. Use AHBclock/8
  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); 	// Wait for timeout
  SysTick->CTRL = 0;                       								// Disable SysTick until next time

	
}

void waitUs(uint32_t us) { //micro seconds
	// This function taken directly from lecture slides. Will write interrupt based function in future.
	
	// wait for ms
	SysTick->CTRL = 0;																			// Disable SysTick
	SysTick->LOAD = ((SystemCoreClock/8) / 1000UL /1000UL) * us;   // Set reload register
  SysTick->VAL = 0;             	    										// Reset the SysTick counter value
  SysTick->CTRL = SysTick_CTRL_ENABLE_Msk; 								// Start SysTick. Use AHBclock/8
  while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); 	// Wait for timeout
  SysTick->CTRL = 0;                       								// Disable SysTick until next time

	
}
