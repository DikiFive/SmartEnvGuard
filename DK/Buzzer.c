/**
 * @file     Buzzer.c
 * @brief    蜂鸣器驱动程序
 * @details  实现蜂鸣器的基本控制功能，包括：
 *          - 初始化
 *          - 开启/关闭控制
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "Buzzer.h"

/**
 * @brief  蜂鸣器初始化
 * @details 完成以下配置：
 *         1. 使能GPIO端口时钟
 *         2. 配置蜂鸣器控制引脚为推挽输出
 *         3. 设置默认状态为关闭
 * @param  无
 * @return 无
 */
void Buzzer_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(Buzzer_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = Buzzer_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStructure);

    // 默认关闭蜂鸣器
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);
}

/**
 * @brief  打开蜂鸣器
 * @details 将控制引脚设置为低电平，使蜂鸣器发声
 * @param  无
 * @return 无
 */
void Buzzer_ON(void)
{
    GPIO_ResetBits(Buzzer_GPIO_Port, Buzzer_Pin); // 低电平触发
}

/**
 * @brief  关闭蜂鸣器
 * @details 将控制引脚设置为高电平，使蜂鸣器停止发声
 * @param  无
 * @return 无
 */
void Buzzer_OFF(void)
{
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin); // 高电平关闭
}
