/*
 * ds18b20.h
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */

#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f1xx_hal.h" // Change this if using F4, L4, etc.

// Configuration: Change these to match your setup
#define DS18B20_PORT GPIOA
#define DS18B20_PIN  GPIO_PIN_3

// Function Prototypes
void DS18B20_Init(TIM_HandleTypeDef *htim);
float DS18B20_GetTemp(void);

#endif /* INC_DS18B20_H_ */
