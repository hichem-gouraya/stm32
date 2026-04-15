/*
 * mq8.c
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */
#include "mq8.h"
#include <math.h> // Needed for the pow() function

float MQ8_GetPPM(ADC_HandleTypeDef *hadc) {
    ADC_ChannelConfTypeDef sConfig = {0};

    // Configure ADC for MQ-8 (Channel 0)
    sConfig.Channel = ADC_CHANNEL_0;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    float hydrogen = 0;

    // Take 300 samples for averaging
    for(int i = 0; i < 300; i++){
        HAL_ADC_Start(hadc);
        while(HAL_ADC_PollForConversion(hadc, 1000) != HAL_OK);
        hydrogen += HAL_ADC_GetValue(hadc);
        HAL_ADC_Stop(hadc);
    }

    // Math conversions
    hydrogen /= 300.0f;
    hydrogen = (hydrogen / 4096.0f) * 3.49f;
    hydrogen *= 2.0f;
    hydrogen = 4.7f * (5.0f / hydrogen - 1.0f);

    float a = 18.5f;
    float b = -1.515f;

    return pow((hydrogen / 2.5f) / a, b);
}

