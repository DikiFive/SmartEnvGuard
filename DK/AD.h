/**
 * @file     AD.h
 * @brief    STM32F10x ADC驱动程序头文件
 * @details  声明了ADC相关的函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __AD_H
#define __AD_H

#include "dk_C8T6.h"

/**
 * @brief    初始化ADC
 * @details  配置ADC的时钟、GPIO、工作模式等参数，并完成校准
 * @param    无
 * @return   无
 */
void AD_Init(void);

/**
 * @brief    获取AD转换值
 * @details  对指定通道进行一次AD转换并返回结果
 * @param    ADC_Channel ADC通道号，可选值为ADC_Channel_x (x=0,1,2,3)
 * @return   AD转换结果，范围0~4095
 */
uint16_t AD_GetValue(uint8_t ADC_Channel);

#endif
