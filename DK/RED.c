/**
 * @file     RED.c
 * @brief    红外传感器驱动程序
 * @details  实现红外传感器的中断检测功能：
 *          - PA7引脚配置为上拉输入
 *          - 使用外部中断检测下降沿
 *          - 通过标志位反映检测状态
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件

/** @brief 红外检测标志，1表示检测到障碍物 */
uint8_t RED_Flag = 0;

/**
 * @brief  红外传感器初始化
 * @details 完成以下配置：
 *         1. 使能相关时钟（GPIO和AFIO）
 *         2. 配置PA7为上拉输入
 *         3. 配置外部中断：
 *            - 映射PA7到EXTI7
 *            - 设置为下降沿触发
 *            - 配置NVIC中断优先级
 * @param  无
 * @return 无
 */
void RED_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  // AFIO时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;     // 上拉输入
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;        // PA7
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置外部中断*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); // PA7映射到EXTI7

    /*EXTI配置*/
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line7;           // 选择EXTI7线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // 使能中断
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;  // 中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 双边沿触发
    EXTI_Init(&EXTI_InitStructure);

    /*NVIC配置*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 配置为分组2

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  获取红外检测状态
 * @return uint8_t 检测结果：
 *         - 1：检测到障碍物
 *         - 0：未检测到障碍物
 */
uint8_t RED_Get(void)
{
    return RED_Flag;
}

/**
 * @brief  EXTI9_5中断服务函数
 * @details 响应红外传感器的外部中断：
 *         1. 判断是否为EXTI7的中断
 *         2. 清除中断标志位
 *         3. 延时一小段时间进行去抖
 *         4. 根据当前引脚电平更新检测标志
 * @note   此函数会被硬件自动调用
 */
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line7) == SET) {
        /*清除中断标志位*/
        EXTI_ClearITPendingBit(EXTI_Line7);

        // 直接更新标志，消抖由定时器处理
        RED_Flag = (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0) ? 1 : 0;
    }
}
