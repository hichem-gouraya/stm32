/*
 * mq8.h
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */

#ifndef __MQ8_H
#define __MQ8_H

#include "main.h" // Needed for HAL_ADC types

// Function prototype
float MQ8_GetPPM(ADC_HandleTypeDef *hadc);

#endif /* INC_MQ8_H_ */
