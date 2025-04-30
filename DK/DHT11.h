/**
 * @file     DHT11.h
 * @brief    DHT11温湿度传感器驱动程序头文件
 * @details  定义了DHT11相关的：
 *          - 数据结构
 *          - 硬件连接配置
 *          - 功能函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __DHT11_H
#define __DHT11_H

#include "dk_C8T6.h"

/**
 * @brief   DHT11测量数据结构体
 * @details 存储从DHT11读取到的温湿度数据，
 *         湿度和温度都分为整数部分和小数部分
 */
typedef struct
{
    uint8_t humi_int;  /**< 湿度整数部分 */
    uint8_t humi_deci; /**< 湿度小数部分 */
    uint8_t temp_int;  /**< 温度整数部分 */
    uint8_t temp_deci; /**< 温度小数部分 */
    uint8_t check_sum; /**< 校验和，用于数据验证 */
} DHT11_Data_TypeDef;

/**
 * @brief DHT11硬件连接定义
 * @note  修改这些宏定义可以改变DHT11的连接引脚
 */
#define DHT11_SCK_APBxClock_FUN RCC_APB2PeriphClockCmd  /**< GPIO时钟使能函数 */
#define DHT11_GPIO_CLK          RCC_APB2Periph_GPIOB    /**< GPIO时钟 */
#define DHT11_GPIO_PORT         GPIOB                    /**< GPIO端口 */
#define DHT11_GPIO_PIN          GPIO_Pin_0               /**< GPIO引脚 */

/**
 * @brief DHT11输入输出控制宏
 * @note  用于控制DHT11数据线的电平和读取数据
 */
#define DHT11_L                 GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN) /**< 输出低电平 */
#define DHT11_H                 GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN)   /**< 输出高电平 */
#define DHT11_IN()             GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) /**< 读取引脚电平 */

/**
 * @brief  DHT11初始化函数
 * @return 无
 */
void DHT11_Init(void);

/**
 * @brief  读取DHT11的温湿度数据
 * @param  DHT11_Data 指向存储测量数据的结构体的指针
 * @return 操作结果
 *         - SUCCESS: 读取成功
 *         - ERROR: 读取失败
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data);

#endif /* __DHT11_H */
