/**
 * @file     Motor.h
 * @brief    直流电机驱动程序头文件
 * @details  声明电机控制相关的函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __MOTOR_H
#define __MOTOR_H

/**
 * @brief  初始化直流电机
 * @details 配置GPIO和PWM：
 *         - PA4/PA5：方向控制引脚
 *         - TIM3_CH1：PWM输出引脚
 * @param  无
 * @return 无
 */
void Motor_Init(void);

/**
 * @brief  设置电机速度
 * @details 通过PWM占空比和方向控制实现速度调节：
 *         - 使用PA4和PA5控制方向
 *         - 使用PWM控制速度
 * @param  Speed 速度值，范围-100~100
 *         - -100~-1：反转，速度随绝对值增大而增大
 *         - 0：停止
 *         - 1~100：正转，速度随数值增大而增大
 * @return 无
 */
void Motor_SetSpeed(int8_t Speed);

#endif /* __MOTOR_H */
