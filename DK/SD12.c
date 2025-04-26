#include "SD12.h"
#include "Delay.h" // 需要Delay函数用于延时

/**
 * 函    数：SD12紫外线传感器模块初始化
 * 参    数：无
 * 返 回 值：无
 */
void SD12_Init(void)
{
    AD_Init();
}

/**
 * 函    数：获取SD12 AD转换的平均值
 * 参    数：num 采集次数
 * 返 回 值：AD转换的平均值
 */
uint16_t SD12_GetADCValue(uint16_t num)
{
    uint32_t sum = 0;
    uint16_t i;
    for (i = 0; i < num; i++) {
        sum += AD_GetValue(SD12_ADC_CHANNEL);
        Delay_ms(1); // 适当延时
    }
    return sum / num;
}

/**
 * 函    数：获取紫外线强度等级
 * 参    数：value AD值
 * 返 回 值：紫外线强度等级 (0-11)
 */
uint8_t SD12_GetIntensity(uint16_t value)
{
    uint8_t ret = 0;
    if (value < 227) // 紫外线强度0级
    {
        ret = 0;
    }
    if (value >= 227 && value < 318) // 紫外线强度1级
    {
        ret = 1;
    }
    if (value >= 318 && value < 408) // 紫外线强度2级
    {
        ret = 2;
    }
    if (value >= 408 && value < 503) // 紫外线强度3级
    {
        ret = 3;
    }
    if (value >= 503 && value < 606) // 紫外线强度4级
    {
        ret = 4;
    }
    if (value >= 606 && value < 696) // 紫外线强度5级
    {
        ret = 5;
    }
    if (value >= 696 && value < 795) // 紫外线强度6级
    {
        ret = 6;
    }

    if (value >= 795 && value < 881) // 紫外线强度7级
    {
        ret = 7;
    }
    if (value >= 881 && value < 976) // 紫外线强度8级
    {
        ret = 8;
    }
    if (value >= 976 && value < 1079) // 紫外线强度9级
    {
        ret = 9;
    }
    if (value >= 1079 && value < 1170) // 紫外线强度10级
    {
        ret = 10;
    }
    if (value >= 1170) // 紫外线强度11级
    {
        ret = 11;
    }
    return ret;
}
