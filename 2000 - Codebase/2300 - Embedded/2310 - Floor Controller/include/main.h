
#ifndef MAIN_H
#define MAIN_H

#include "Elevator.h"

void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define Debounce2_Pin GPIO_PIN_2
#define Debounce2_GPIO_Port GPIOC
#define Floor1LED_Pin GPIO_PIN_0
#define Floor1LED_GPIO_Port GPIOA
#define Floor2LED_Pin GPIO_PIN_1
#define Floor2LED_GPIO_Port GPIOA
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define Floor3LED_Pin GPIO_PIN_4
#define Floor3LED_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define Switch_LED2_Pin GPIO_PIN_7
#define Switch_LED2_GPIO_Port GPIOC
#define Switch_LED3_Pin GPIO_PIN_8
#define Switch_LED3_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define Debouce1_Pin GPIO_PIN_15
#define Debouce1_GPIO_Port GPIOA
#define Switch_LED_Pin GPIO_PIN_10
#define Switch_LED_GPIO_Port GPIOC
#define Floor_switch_Pin GPIO_PIN_12
#define Floor_switch_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

#endif
