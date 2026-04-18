#include "dht22.h"

static TIM_HandleTypeDef *dht_timer;

static void delay_us(uint16_t us) {
    __HAL_TIM_SET_COUNTER(dht_timer, 0);
    while (__HAL_TIM_GET_COUNTER(dht_timer) < us);
}

static void Set_Pin(uint8_t mode) {
    GPIO_InitTypeDef g = {0};
    g.Pin = DHT22_PIN;
    g.Speed = GPIO_SPEED_FREQ_HIGH;
    if(mode) { // Output
        g.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(DHT22_PORT, &g);
    } else {   // Input
        g.Mode = GPIO_MODE_INPUT;
        g.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DHT22_PORT, &g);
    }
}

void DHT22_Init(TIM_HandleTypeDef *htim) {
    dht_timer = htim;
    HAL_TIM_Base_Start(dht_timer);
}

float DHT22_GetHumidity(void) {
    uint8_t data[5] = {0};

    // 1. Start Signal
    Set_Pin(1);
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 0);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT22_PORT, DHT22_PIN, 1);
    delay_us(30);
    Set_Pin(0);

    // 2. Wait for Response (Skipping the 80us Low/High handshake)
    uint16_t timeout = 0;
    while(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) && timeout < 100) { delay_us(1); timeout++; }
    while(!HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) && timeout < 200) { delay_us(1); timeout++; }
    while(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN) && timeout < 300) { delay_us(1); timeout++; }

    // 3. Capture 40 bits
    for (int i = 0; i < 40; i++) {
        while(!HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)); // Wait for pulse to start
        delay_us(40); // Sample in the middle of a "1" pulse

        if (HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN))
            data[i/8] |= (1 << (7 - (i % 8)));

        while(HAL_GPIO_ReadPin(DHT22_PORT, DHT22_PIN)); // Wait for pulse to end
    }

    // 4. Verify Checksum and return Humidity
    if ((uint8_t)(data[0] + data[1] + data[2] + data[3]) == data[4]) {
        return (float)((data[0] << 8) | data[1]) / 10.0f;
    }

    return -1.0f; // Return -1 if read failed
}
