/*
 * acs712.c
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */

#include "acs712.h"
static float currentv ;
 float ACS712_GetCurrentv(ADC_HandleTypeDef *hadc){
	   ADC_ChannelConfTypeDef sConfig = {0};

	    // Configure ADC for ACS712 (Channel 4)
	    sConfig.Channel = ADC_CHANNEL_4;
	    sConfig.Rank = ADC_REGULAR_RANK_1;
	    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	    HAL_ADC_ConfigChannel(hadc, &sConfig);


	        for(int i = 0; i < 1000; i++){
	                    HAL_ADC_Start(hadc);
	                    while(HAL_ADC_PollForConversion(hadc, 1000) != HAL_OK);
	                    currentv += HAL_ADC_GetValue(hadc);
	                    HAL_ADC_Stop(hadc);
	                }
	            currentv /= 1000.0;
	        	currentv = (currentv / 4096.0f) * 3.49f * 2.0f;

	        	        return currentv;
	        }



float ACS712_GetCurrent(ADC_HandleTypeDef *hadc) {
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC for ACS712 (Channel 4)
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(hadc, &sConfig);
    float current = 0;




    //===========================================test===========================
    for(int i = 0; i < 300; i++){
            HAL_ADC_Start(hadc);
            while(HAL_ADC_PollForConversion(hadc, 1000) != HAL_OK);
            current += HAL_ADC_GetValue(hadc);
            HAL_ADC_Stop(hadc);
        }

        // Math conversions
        current /= 300.0;
        current = (current / 4096.0) * 3.49 * 2.0;

        current = ((current - currentv ) / 0.185);
        	if (current < 0){
        		current = -(current);
        	}
        	if (current < 0.15) {
        	        			current = 0.0;
        	 }
    return current;
}

