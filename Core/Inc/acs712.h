/*
 * acs712.h
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */

#ifndef __ACS712_H
#define __ACS712_H

#include "main.h" // Needed for HAL_ADC types

// Function prototype
float ACS712_GetCurrentv(ADC_HandleTypeDef *hadc);
float ACS712_GetCurrent(ADC_HandleTypeDef *hadc);

#endif /* INC_ACS712_H_ */
