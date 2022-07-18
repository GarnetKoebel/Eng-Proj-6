#ifndef __STM32F303RE_NUCLEO64_LED_H
#define __STM32F303RE_NUCLEO64_LED_H

#include "stm32f303xe.h"
#include "Macros.h"
#include "GPIO.h"

void LED_Init(void);

void Green_LED_On(void);
void Green_LED_Off(void);
void Green_LED_Toggle(void);

#endif
