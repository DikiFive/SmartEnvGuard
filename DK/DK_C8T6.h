/**
 * @file     DK_C8T6.h
 * @brief    STM32F103C8T6主控制程序头文件
 * @details  定义了系统的：
 *          - 工作模式
 *          - 数据结构
 *          - 功能函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __DK_C8T6_H
#define __DK_C8T6_H

#include <stdint.h>    // 标准整数类型定义
#include "stm32f10x.h" // STM32F10x外设库头文件

// 包含所有外设驱动头文件
#include "AD.h"
#include "BT.h"
#include "Buzzer.h"
#include "Delay.h"
#include "DHT11.h"
#include "fan.h"
#include "Key.h"
#include "LED.h"
#include "Motor.h"
#include "OLED.h"
#include "PWM.h"
#include "RED.h"
#include "SD12.h"
#include "Serial.h"
#include "Servo.h"
#include "Timer.h"

/**
 * @brief 系统工作模式枚举
 */
typedef enum {
    MODE_MANUAL = 0, /**< 手动模式：通过按键直接控制设备 */
    MODE_AUTO   = 1, /**< 自动模式：系统自动控制设备（温湿度+红外） */
    MODE_CYCLE  = 2, /**< 循环模式：15秒周期循环控制设备 */
    MODE_BT     = 3  /**< 蓝牙模式：通过蓝牙控制设备 */
} SystemMode_t;

/**
 * @brief 声明为外部变量，以便其他文件访问
 */
extern SystemMode_t currentMode;

/**
 * @brief 传感器数据结构体
 * @details 包含所有传感器的测量数据
 */
typedef struct {
    uint8_t dht11_status; /**< DHT11状态：0-正常，1-错误 */
    uint8_t humi_int;     /**< 湿度整数部分 */
    uint8_t humi_deci;    /**< 湿度小数部分 */
    uint8_t temp_int;     /**< 温度整数部分 */
    uint8_t temp_deci;    /**< 温度小数部分 */
    uint8_t uvLevel;      /**< 紫外线等级(0-11) */
    uint8_t redValue;     /**< 红外传感器值(0-1) */
} SensorData_t;

/**
 * @brief 按键处理结果结构体
 */
typedef struct {
    int keyValue;    /**< 当前按键值 */
    uint8_t changed; /**< 按键值是否改变标志 */
} KeyStatus_t;

/**
 * @brief 蓝牙处理结果结构体
 */
typedef struct {
    int8_t status;      /**< 蓝牙操作状态：0成功，其他失败 */
    uint8_t uv_flag;    /**< UV灯控制标志 */
    uint8_t servo_flag; /**< 舵机控制标志 */
    uint8_t fan_flag;   /**< 风扇控制标志 */
    uint8_t motor_flag; /**< 电机控制标志 */
    uint8_t mode_flag;  /**< 模式控制标志 */
} BTStatus_t;

/**
 * @brief  系统初始化
 * @return 无
 */
void Sys_Init(void);

/**
 * @brief  获取所有传感器数据
 * @return SensorData_t 传感器数据结构体
 */
SensorData_t GetAllSensorData(void);

/**
 * @brief  处理按键输入
 * @param  currentKeyValue 当前按键值
 * @return KeyStatus_t 按键处理结果
 */
KeyStatus_t HandleKeyPress(int currentKeyValue);

/**
 * @brief  处理蓝牙数据
 * @return BTStatus_t 蓝牙处理结果
 */
BTStatus_t HandleBluetooth(void);

/**
 * @brief  更新OLED显示
 * @param  keyValue     按键值
 * @param  dht11_status DHT11状态
 * @param  humi_int     湿度整数
 * @param  humi_deci    湿度小数
 * @param  temp_int     温度整数
 * @param  temp_deci    温度小数
 * @param  uvLevel      紫外线等级
 * @param  redValue     红外值
 * @param  bt_status    蓝牙状态
 * @return 无
 */
void OLED_UpdateDisplay(int keyValue, uint8_t dht11_status,
                        uint8_t humi_int, uint8_t humi_deci,
                        uint8_t temp_int, uint8_t temp_deci,
                        uint8_t uvLevel, uint8_t redValue, int8_t bt_status);

/**
 * @brief  处理系统主要任务
 * @details 包含以下功能：
 *         1. 按键输入和传感器数据的处理（100ms周期）
 *         2. 自动模式和循环模式的控制逻辑
 *         3. 蓝牙数据的处理
 *         4. OLED显示的更新
 * @param  无
 * @return 无
 */
void ProcessSystemTasks(void);

#endif /* __DK_C8T6_H */
