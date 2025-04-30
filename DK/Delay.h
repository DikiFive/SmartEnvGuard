/**
 * @file     Delay.h
 * @brief    系统延时功能头文件
 * @details  声明了系统延时相关的函数接口：
 *          - 微秒级延时
 *          - 毫秒级延时
 *          - 秒级延时
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __DELAY_H
#define __DELAY_H

/**
 * @brief  微秒级延时
 * @details 提供最高精度的延时功能，适用于短时间延时场景
 * @note   延时范围受限于SysTick定时器的最大计数值
 * @param  us 延时时长，单位：微秒，范围：0~233015
 * @return 无
 */
void Delay_us(uint32_t us);

/**
 * @brief  毫秒级延时
 * @details 提供毫秒级延时，适用于一般延时场景
 * @param  ms 延时时长，单位：毫秒，范围：0~4294967295
 * @return 无
 */
void Delay_ms(uint32_t ms);

/**
 * @brief  秒级延时
 * @details 提供秒级延时，适用于长时间延时场景
 * @param  s 延时时长，单位：秒，范围：0~4294967295
 * @return 无
 */
void Delay_s(uint32_t s);

#endif /* __DELAY_H */
