/**
 * @file     fan.h
 * @brief    风扇驱动程序头文件
 * @details  定义了风扇控制的：
 *          - 引脚配置
 *          - 控制函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __FAN_H
#define __FAN_H

#include "dk_C8T6.h"

/**
 * @brief 风扇硬件连接定义
 * @note  修改这些宏定义可以改变风扇的连接引脚
 */
#define Fan_Pin       GPIO_Pin_15         /**< 风扇控制引脚 */
#define Fan_GPIO_Port GPIOC               /**< 风扇所在的GPIO端口 */
#define Fan_GPIO_CLK  RCC_APB2Periph_GPIOC /**< 风扇GPIO端口时钟 */

/**
 * @brief  初始化风扇
 * @details 配置GPIO并设置初始状态
 * @param  无
 * @return 无
 */
void Fan_Init(void);

/**
 * @brief  打开风扇
 * @details 驱动风扇开始转动
 * @param  无
 * @return 无
 */
void Fan_ON(void);

/**
 * @brief  关闭风扇
 * @details 停止风扇转动
 * @param  无
 * @return 无
 */
void Fan_OFF(void);

#endif /* __FAN_H */
