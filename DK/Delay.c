/**
 * @file     Delay.c
 * @brief    系统延时功能实现
 * @details  基于SysTick定时器实现的延时函数，包括：
 *          - 微秒级延时
 *          - 毫秒级延时
 *          - 秒级延时
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h"
#include "dk_C8T6.h"

/**
 * @brief  微秒级延时函数
 * @details 利用SysTick定时器实现延时，计算原理：
 *         - STM32系统主频为72MHz
 *         - SysTick计数器每个计数需要 1/72 微秒
 *         - reload值 = 延时时间(us) * 72
 * @note   由于定时器最大值限制，延时范围：0~233015微秒
 * @param  xus 延时时长，单位：微秒，范围：0~233015
 * @return 无
 */
void Delay_us(uint32_t xus)
{
    SysTick->LOAD = 72 * xus;              // 设置定时器重装值
    SysTick->VAL  = 0x00;                  // 清空当前计数值
    SysTick->CTRL = 0x00000005;            // 设置时钟源为HCLK，启动定时器
    while (!(SysTick->CTRL & 0x00010000)); // 等待计数到0
    SysTick->CTRL = 0x00000004;            // 关闭定时器
}

/**
 * @brief  毫秒级延时函数
 * @details 通过调用微秒级延时函数实现，将毫秒转换为微秒
 * @param  xms 延时时长，单位：毫秒，范围：0~4294967295
 * @return 无
 */
void Delay_ms(uint32_t xms)
{
    while (xms--) {
        Delay_us(1000);
    }
}

/**
 * @brief  秒级延时函数
 * @details 通过调用毫秒级延时函数实现，将秒转换为毫秒
 * @param  xs 延时时长，单位：秒，范围：0~4294967295
 * @return 无
 */
void Delay_s(uint32_t xs)
{
    while (xs--) {
        Delay_ms(1000);
    }
}
