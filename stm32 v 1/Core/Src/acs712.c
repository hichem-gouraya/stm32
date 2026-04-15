/*
 * acs712.c
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */
#include "acs712.h"

float ACS712_GetCurrent(ADC_HandleTypeDef *hadc) {
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC for ACS712 (Channel 4)
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    float current = 0;

    // Take 300 samples for averaging
    for(int i = 0; i < 300; i++){
        HAL_ADC_Start(hadc);
        while(HAL_ADC_PollForConversion(hadc, 1000) != HAL_OK);
        current += HAL_ADC_GetValue(hadc);
        HAL_ADC_Stop(hadc);
    }

    // Math conversions
    current /= 300.0f;
    current = (current / 4096.0f) * 3.49f * 2.0f;
    current = ((current - 2.645f) / 0.185f);
    //if (current < 0 ) current =0 ;

    return current;
}

