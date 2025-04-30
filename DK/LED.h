/**
 * @file     LED.h
 * @brief    LED驱动程序头文件
 * @details  声明了所有LED控制函数接口，包括：
 *          1. LED1/LED2：通用指示灯
 *          2. 系统LED：状态指示
 *          3. 紫外线灯：消毒功能
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __LED_H
#define __LED_H

/**
 * @brief  初始化LED1和LED2
 * @details 配置PA1和PA2为推挽输出模式
 * @param  无
 * @return 无
 */
void LED_Init(void);

/**
 * @brief  初始化系统LED
 * @details 配置PC13为推挽输出模式
 * @param  无
 * @return 无
 */
void LED_Sys_Init(void);

/**
 * @brief  系统LED控制函数组
 * @note   控制PC13引脚
 */
void LED_Sys_ON(void);   /**< 打开系统LED */
void LED_Sys_OFF(void);  /**< 关闭系统LED */
void LED_Sys_Turn(void); /**< 翻转系统LED状态 */

/**
 * @brief  LED1控制函数组
 * @note   控制PA1引脚
 */
void LED1_ON(void);   /**< 打开LED1 */
void LED1_OFF(void);  /**< 关闭LED1 */
void LED1_Turn(void); /**< 翻转LED1状态 */

/**
 * @brief  LED2控制函数组
 * @note   控制PA2引脚
 */
void LED2_ON(void);   /**< 打开LED2 */
void LED2_OFF(void);  /**< 关闭LED2 */
void LED2_Turn(void); /**< 翻转LED2状态 */

/**
 * @brief  紫外线灯控制函数组
 * @note   控制PA12引脚
 */
void UV_Init(void);  /**< 初始化紫外线灯 */
void UV_ON(void);    /**< 打开紫外线灯 */
void UV_OFF(void);   /**< 关闭紫外线灯 */
void UV_Turn(void);  /**< 翻转紫外线灯状态 */

#endif /* __LED_H */
