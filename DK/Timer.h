/**
 * @file     Timer.h
 * @brief    定时器驱动程序头文件
 * @details  声明定时器相关的：
 *          - 全局变量
 *          - 功能函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

/**
 * @brief 定时器相关变量声明
 */
extern uint32_t TimingDelay;          /**< 软件延时计数器 */
extern uint32_t system_runtime_s;     /**< 系统运行时间（秒） */
extern uint32_t ms_count;             /**< 毫秒计数器 */
extern uint32_t uv_timer_ms;          /**< 红外触发UV灯计时器 */
extern uint8_t uv_infrared_active;    /**< 红外触发UV灯工作标志 */
extern uint32_t cycle_timer_ms;       /**< 循环模式计时器 */
extern uint8_t cycle_state;           /**< 循环模式状态 */
extern uint8_t update_flag;           /**< 定时更新标志 */

/**
 * @brief  定时器初始化
 * @details 配置TIM4为1ms定时中断
 * @param  无
 * @return 无
 */
void Timer_Init(void);

/**
 * @brief  设置延时时间
 * @param  nTime 延时时长（毫秒）
 * @return 无
 */
void TimingDelay_Set(uint32_t nTime);

/**
 * @brief  获取当前延时计数值
 * @return uint32_t 当前延时计数值
 */
uint32_t TimingDelay_Get(void);

/**
 * @brief  等待延时结束
 * @details 阻塞等待，直到延时计数为0
 * @return 无
 */
void TimingDelay_WaitForEnd(void);

#endif /* __TIMER_H */
