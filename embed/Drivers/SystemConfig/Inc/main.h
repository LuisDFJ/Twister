#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SW_Pin GPIO_PIN_2
#define SW_GPIO_Port GPIOA
#define SPI_CS_Pin GPIO_PIN_0
#define SPI_CS_GPIO_Port GPIOB
#define TFT_RESET_Pin GPIO_PIN_1
#define TFT_RESET_GPIO_Port GPIOB
#define TFT_DC_Pin GPIO_PIN_10
#define TFT_DC_GPIO_Port GPIOB
#define DIR_Pin GPIO_PIN_5
#define DIR_GPIO_Port GPIOB
#define STEP_Pin GPIO_PIN_6
#define STEP_GPIO_Port GPIOB
#define MS3_Pin GPIO_PIN_7
#define MS3_GPIO_Port GPIOB
#define MS2_Pin GPIO_PIN_8
#define MS2_GPIO_Port GPIOB
#define MS1_Pin GPIO_PIN_9
#define MS1_GPIO_Port GPIOB


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
