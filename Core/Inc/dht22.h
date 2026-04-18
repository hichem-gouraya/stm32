#ifndef DHT22_SIMPLE_H_
#define DHT22_SIMPLE_H_

#include "stm32f1xx_hal.h"

#define DHT22_PORT  GPIOA
#define DHT22_PIN   GPIO_PIN_5

void DHT22_Init(TIM_HandleTypeDef *htim);
float DHT22_GetHumidity(void);

#endif
