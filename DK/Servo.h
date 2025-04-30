/**
 * @file     Servo.h
 * @brief    舵机驱动程序头文件
 * @details  声明舵机控制相关的函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __SERVO_H
#define __SERVO_H

/**
 * @brief  舵机初始化
 * @details 配置TIM2_CH2为PWM输出：
 *         - 周期：20ms（50Hz）
 *         - 脉宽：0.5ms~2.5ms
 * @param  无
 * @return 无
 */
void Servo_Init(void);

/**
 * @brief  设置舵机角度
 * @param  Angle 目标角度，范围：0~180度
 * @return 无
 */
void Servo_SetAngle(float Angle);

#endif /* __SERVO_H */
