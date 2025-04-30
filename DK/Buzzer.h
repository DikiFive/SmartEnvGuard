/**
 * @file     Buzzer.h
 * @brief    蜂鸣器驱动程序头文件
 * @details  定义了蜂鸣器控制相关的宏和函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __BUZZER_H
#define __BUZZER_H

#include "dk_C8T6.h"

/**
 * @brief 蜂鸣器硬件连接定义
 * @note  修改这些宏定义可以改变蜂鸣器的连接引脚
 */
#define Buzzer_Pin       GPIO_Pin_14      /**< 蜂鸣器控制引脚 */
#define Buzzer_GPIO_Port GPIOC            /**< 蜂鸣器所在的GPIO端口 */
#define Buzzer_GPIO_CLK  RCC_APB2Periph_GPIOC /**< 蜂鸣器GPIO端口时钟 */

/**
 * @brief  初始化蜂鸣器
 * @details 配置GPIO和默认状态
 * @param  无
 * @return 无
 */
void Buzzer_Init(void);

/**
 * @brief  打开蜂鸣器
 * @details 使蜂鸣器发声
 * @param  无
 * @return 无
 */
void Buzzer_ON(void);

/**
 * @brief  关闭蜂鸣器
 * @details 使蜂鸣器停止发声
 * @param  无
 * @return 无
 */
void Buzzer_OFF(void);

#endif // __BUZZER_H
