/**
 * @file     Timer.h
 * @brief    系统定时器驱动程序头文件
 * @details  声明定时器相关的函数接口：
 *          - 初始化函数
 *          - 延时控制函数
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

// 声明外部变量
extern uint32_t system_runtime_s;  // 系统运行时间(秒)
extern uint32_t ms_count;          // 毫秒计数器
extern uint32_t uv_timer_ms;       // 红外触发UV灯计时器
extern uint8_t uv_infrared_active; // 红外触发UV灯工作标志
extern uint32_t cycle_timer_ms;    // 循环模式计时器
extern uint8_t cycle_state;        // 循环模式状态
extern uint8_t update_flag;        // 定时更新标志

/**
 * @brief  定时器初始化
 * @details 配置TIM4为基本定时器：
 *         - 1ms中断周期
 *         - 中断优先级2-0
 * @param  无
 * @return 无
 */
void Timer_Init(void);

/**
 * @brief  设置延时时间
 * @details 设置延时计数器的初值，在定时器中断中递减
 * @param  nTime 延时时间，单位：毫秒（ms）
 * @return 无
 */
void Timer_SetDelay(uint32_t nTime);

/**
 * @brief  获取当前延时计数值
 * @details 读取延时计数器的当前值
 * @return uint32_t 当前延时计数值（ms）
 * @note   可用于判断延时是否结束：
 *         - 计数值为0表示延时结束
 *         - 计数值非0表示延时进行中
 */
uint32_t Timer_GetDelay(void);

#endif /* __TIMER_H */
