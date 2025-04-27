#ifndef __DK_C8T6_H
#define __DK_C8T6_H

#include <stdint.h>    // 引入标准整数类型定义[2,5](@ref)
#include "stm32f10x.h" // Device header

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

// 定义传感器数据结构体
typedef struct {
    uint8_t dht11_status;
    uint8_t humi_int;
    uint8_t humi_deci;
    uint8_t temp_int;
    uint8_t temp_deci;
    uint8_t uvLevel;
    uint8_t redValue;
} SensorData_t;

// 定义按键处理结果结构体
typedef struct {
    int keyValue;
    uint8_t changed; // 标记按键值是否发生改变
} KeyStatus_t;

// 定义蓝牙处理结果结构体
typedef struct {
    int8_t status;      // 蓝牙操作状态：0成功，其他失败
    uint8_t uv_flag;    // UV灯控制标志
    uint8_t servo_flag; // 舵机控制标志
    uint8_t fan_flag;   // 风扇控制标志
    uint8_t motor_flag; // 电机控制标志
    uint8_t mode_flag;  // 模式控制标志
} BTStatus_t;

// 系统初始化函数
void Sys_Init(void);

// 获取所有传感器数据
SensorData_t GetAllSensorData(void);

// 处理按键输入
KeyStatus_t HandleKeyPress(int currentKeyValue);

// 处理蓝牙数据包
BTStatus_t HandleBluetooth(void);

// OLED显示更新
void OLED_UpdateDisplay(int keyValue, uint8_t dht11_status,
                        uint8_t humi_int, uint8_t humi_deci,
                        uint8_t temp_int, uint8_t temp_deci,
                        uint8_t uvLevel, uint8_t redValue, int8_t bt_status);

#endif
