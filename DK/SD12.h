#ifndef __SD12_H
#define __SD12_H

#include "dk_C8T6.h"
#include "AD.h"

// 定义SD12紫外线传感器连接的ADC通道
#define SD12_ADC_CHANNEL ADC_Channel_0

void SD12_Init(void);
uint16_t SD12_GetADCValue(uint16_t num);
uint8_t SD12_GetIntensity(uint16_t value);

#endif // __SD12_H
