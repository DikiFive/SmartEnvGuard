/**
 * @file     SD12.c
 * @brief    SD12紫外线传感器驱动程序
 * @details  实现SD12紫外线强度检测功能：
 *          - ADC采样（PA0）
 *          - 紫外线强度分级（0-11级）
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief  SD12传感器初始化
 * @details 完成以下配置：
 *         1. 使能GPIO时钟
 *         2. 配置PA0为模拟输入模式
 *         3. 配置ADC采样参数：
 *            - 12位分辨率
 *            - 单次转换模式
 *            - 软件触发
 * @param  无
 * @return 无
 */
void SD12_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // ADC时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;      // 模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          // PA0
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*ADC配置*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;            // 独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;       // 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;          // 单次转换
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;               // 非扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 1;                     // 1个转换通道
    ADC_Init(ADC1, &ADC_InitStructure);

    /*ADC使能*/
    ADC_Cmd(ADC1, ENABLE);

    /*ADC校准*/
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
 * @brief  获取ADC采样值
 * @details 执行一次ADC转换并获取结果：
 *         1. 启动ADC转换
 *         2. 等待转换完成
 *         3. 读取转换结果
 * @param  nSample 采样次数（用于平均）
 * @return uint16_t ADC转换结果，范围0~4095
 */
uint16_t SD12_GetADCValue(uint8_t nSample)
{
    uint32_t sum = 0;

    for (uint8_t i = 0; i < nSample; i++) {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);           // 启动转换
        while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 等待转换完成
        sum += ADC_GetConversionValue(ADC1);              // 读取转换结果
    }

    return sum / nSample; // 返回平均值
}

/**
 * @brief  将ADC值转换为紫外线强度等级
 * @details 根据转换结果进行分级：
 *         - ADC值范围：0~4095
 *         - UV等级范围：0~11
 *         - 转换公式：level = ADC值 / 372
 * @param  adValue ADC转换值，范围0~4095
 * @return uint8_t 紫外线强度等级，范围0~11
 */
uint8_t SD12_GetIntensity(uint16_t adValue)
{
    uint8_t level = adValue / 372;  // 4095/11 ≈ 372
    return (level > 11) ? 11 : level; // 限制最大值为11
}
