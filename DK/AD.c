/**
 * @file     AD.c
 * @brief    STM32F10x ADC驱动程序
 * @details  实现了ADC的初始化和数据读取功能
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x头文件
#include "dk_C8T6.h"   // 项目主设备头文件

/**
 * @brief    AD初始化函数
 * @details  完成ADC1的初始化配置，包括：
 *          1. 配置ADC时钟为72MHz/6=12MHz
 *          2. 配置PA0为模拟输入模式
 *          3. 配置ADC1工作参数
 *          4. 执行ADC校准
 * @param    无
 * @return   无
 */
void AD_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 开启ADC1的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    /*设置ADC时钟*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 选择时钟6分频，ADCCLK = 72MHz / 6 = 12MHz

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA0引脚初始化为模拟输入

    /*不在此处配置规则组序列，而是在每次AD转换前配置，这样可以灵活更改AD转换的通道*/

    /*ADC初始化*/
    ADC_InitTypeDef ADC_InitStructure;                                    // 定义结构体变量
    ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;      // 模式，选择独立模式，即单独使用ADC1
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;       // 数据对齐，选择右对齐
    ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None; // 外部触发，使用软件触发，不需要外部触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                   // 连续转换，失能，每转换一次规则组序列后停止
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;                   // 扫描模式，失能，只转换规则组的序列1这一个位置
    ADC_InitStructure.ADC_NbrOfChannel       = 1;                         // 通道数，为1，仅在扫描模式下，才需要指定大于1的数，在非扫描模式下，只能是1
    ADC_Init(ADC1, &ADC_InitStructure);                                   // 将结构体变量交给ADC_Init，配置ADC1

    /*ADC使能*/
    ADC_Cmd(ADC1, ENABLE); // 使能ADC1，ADC开始运行

    /*ADC校准*/
    ADC_ResetCalibration(ADC1); // 固定流程，内部有电路会自动执行校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/**
 * @brief    获取AD转换的值
 * @details  完成以下操作：
 *          1. 配置ADC规则组的转换通道和采样时间
 *          2. 触发一次AD转换
 *          3. 等待转换完成
 *          4. 返回转换结果
 * @param    ADC_Channel 指定AD转换的通道，范围：ADC_Channel_x，其中x可以是0/1/2/3
 * @return   AD转换的结果，范围：0~4095
 */
uint16_t AD_GetValue(uint8_t ADC_Channel)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5); // 在每次转换前，根据函数形参灵活更改规则组的通道1
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                                   // 软件触发AD转换一次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);                   // 等待EOC标志位，即等待AD转换结束
    return ADC_GetConversionValue(ADC1);                                      // 读数据寄存器，得到AD转换的结果
}
