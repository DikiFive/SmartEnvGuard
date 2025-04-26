#ifndef __DHT11_H
#define __DHT11_H
#include "dk_C8T6.h"

/************************** DHT11 数据类型定义********************************/
typedef struct
{
    uint8_t humi_int;  // 湿度的整数部分
    uint8_t humi_deci; // 湿度的小数部分
    uint8_t temp_int;  // 温度的整数部分
    uint8_t temp_deci; // 温度的小数部分
    uint8_t check_sum; // 校验和

} DHT11_Data_TypeDef;

/************************** DHT11 连接引脚定义********************************/
#define DHT11_SCK_APBxClock_FUN RCC_APB2PeriphClockCmd
#define DHT11_GPIO_CLK          RCC_APB2Periph_GPIOB

#define DHT11_GPIO_PORT         GPIOB
#define DHT11_GPIO_PIN          GPIO_Pin_0

#define DHT11_L                 GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN)
#define DHT11_H                 GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

#define DHT11_IN()              GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN)

void DHT11_Init(void);
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);

#endif
