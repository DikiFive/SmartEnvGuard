/**
 * @file     fan.c
 * @brief    风扇驱动程序
 * @details  实现风扇的初始化和基本控制功能：
 *          - GPIO配置
 *          - 开关控制
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "fan.h"

/**
 * @brief  风扇初始化
 * @details 完成以下配置：
 *         1. 使能GPIO端口时钟
 *         2. 配置控制引脚为推挽输出
 *         3. 设置默认状态为关闭
 * @param  无
 * @return 无
 */
void Fan_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(Fan_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = Fan_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Fan_GPIO_Port, &GPIO_InitStructure);

    // 默认关闭风扇
    GPIO_SetBits(Fan_GPIO_Port, Fan_Pin); // Updated to use Fan_Pin
}

/**
 * @brief  打开风扇
 * @details 将控制引脚设置为低电平，驱动风扇转动
 * @note   使用低电平触发
 * @param  无
 * @return 无
 */
void Fan_ON(void)
{
    GPIO_ResetBits(Fan_GPIO_Port, Fan_Pin); // 低电平触发
}

/**
 * @brief  关闭风扇
 * @details 将控制引脚设置为高电平，停止风扇转动
 * @note   使用高电平关闭
 * @param  无
 * @return 无
 */
void Fan_OFF(void)
{
    GPIO_SetBits(Fan_GPIO_Port, Fan_Pin); // 高电平关闭
}
