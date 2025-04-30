/**
 * @file     PWM.h
 * @brief    PWM驱动程序头文件
 * @details  声明PWM相关的控制函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __PWM_H
#define __PWM_H

/**
 * @brief  PWM初始化
 * @details 配置TIM3为PWM输出模式：
 *         - TIM3_CH1(PA6)：电机控制
 *         - 频率：20KHz
 *         - 占空比范围：0~100%
 * @param  无
 * @return 无
 */
void PWM_Init(void);

/**
 * @brief  设置TIM3通道1的PWM占空比
 * @details 用于电机速度控制
 * @param  Compare 占空比值，范围0~100：
 *         - 0：停止
 *         - 1~99：速度由小到大
 *         - 100：最大速度
 * @return 无
 */
void PWM_SetCompare1(uint16_t Compare);

/**
 * @brief  设置TIM3通道2的PWM占空比
 * @param  Compare 占空比值，范围0~100
 * @return 无
 */
void PWM_SetCompare2(uint16_t Compare);

/**
 * @brief  设置TIM3通道3的PWM占空比
 * @param  Compare 占空比值，范围0~100
 * @return 无
 */
void PWM_SetCompare3(uint16_t Compare);

#endif /* __PWM_H */
