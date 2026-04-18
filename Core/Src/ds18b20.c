/*
 * ds18b20.c
 *
 *  Created on: Apr 15, 2026
 *      Author: windows
 */
#include "ds18b20.h"

static TIM_HandleTypeDef *ds18b20_timer;

// Internal Microsecond Delay
static void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(ds18b20_timer, 0);
    while (__HAL_TIM_GET_COUNTER(ds18b20_timer) < us);
}

// Internal GPIO Helpers
static void DQ_Low(void) {
    GPIO_InitTypeDef g = {0};
    g.Pin   = DS18B20_PIN;
    g.Mode  = GPIO_MODE_OUTPUT_OD;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DS18B20_PORT, &g);
    HAL_GPIO_WritePin(DS18B20_PORT, DS18B20_PIN, GPIO_PIN_RESET);
}

static void DQ_Release(void) {
    GPIO_InitTypeDef g = {0};
    g.Pin  = DS18B20_PIN;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DS18B20_PORT, &g);
}

static uint8_t DQ_Read(void) {
    return HAL_GPIO_ReadPin(DS18B20_PORT, DS18B20_PIN);
}

// 1-Wire Core Logic
static uint8_t OW_Reset(void) {
    DQ_Low();    delay_us(500);
    DQ_Release(); delay_us(70);
    uint8_t present = !DQ_Read();
    delay_us(410);
    return present;
}

static void OW_WriteByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        DQ_Low();
        if (byte & 0x01) { delay_us(5);  DQ_Release(); delay_us(65); }
        else             { delay_us(65); DQ_Release(); delay_us(5);  }
        byte >>= 1;
    }
}

static uint8_t OW_ReadByte(void) {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        DQ_Low();    delay_us(3);
        DQ_Release(); delay_us(10);
        if (DQ_Read()) byte |= (1 << i);
        delay_us(57);
    }
    return byte;
}

// Public Functions
void DS18B20_Init(TIM_HandleTypeDef *htim) {
    ds18b20_timer = htim;
    HAL_TIM_Base_Start(ds18b20_timer);

    // Set to 11-bit resolution
    OW_Reset();
    OW_WriteByte(0xCC);
    OW_WriteByte(0x4E);
    OW_WriteByte(0x00);
    OW_WriteByte(0x00);
    OW_WriteByte(0x5F);
}

float DS18B20_GetTemp(void) {
    OW_Reset();
    OW_WriteByte(0xCC);
    OW_WriteByte(0x44);
    HAL_Delay(380);

    OW_Reset();
    OW_WriteByte(0xCC);
    OW_WriteByte(0xBE);

    uint8_t lsb = OW_ReadByte();
    uint8_t msb = OW_ReadByte();

    int16_t raw = (int16_t)((msb << 8) | lsb);
    raw &= ~0x01;
    return raw * 0.125f;
}
