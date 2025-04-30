/**
 * @file     SD12.h
 * @brief    SD12紫外线传感器驱动程序头文件
 * @details  声明SD12传感器相关的功能接口：
 *          - 初始化函数
 *          - ADC采样函数
 *          - 强度转换函数
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __SD12_H
#define __SD12_H

#include <stdint.h>

/**
 * @brief 紫外线强度等级定义
 */
#define UV_LEVEL_MAX 11  /**< 最大强度等级 */
#define UV_LEVEL_MIN 0   /**< 最小强度等级 */

/**
 * @brief  SD12传感器初始化
 * @details 配置ADC采样相关的GPIO和参数
 * @param  无
 * @return 无
 */
void SD12_Init(void);

/**
 * @brief  获取ADC采样值
 * @details 执行多次采样并取平均值
 * @param  nSample 采样次数，建议值：10
 * @return uint16_t ADC转换结果，范围：0~4095
 */
uint16_t SD12_GetADCValue(uint8_t nSample);

/**
 * @brief  获取紫外线强度等级
 * @details 将ADC值映射到0~11级
 * @param  adValue ADC采样值，范围：0~4095
 * @return uint8_t 紫外线强度等级：
 *         - 0：无紫外线
 *         - 1~10：正常强度范围
 *         - 11：最大强度
 */
uint8_t SD12_GetIntensity(uint16_t adValue);

#endif /* __SD12_H */
