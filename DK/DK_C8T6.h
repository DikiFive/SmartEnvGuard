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

void Sys_Init(void); // 系统初始化函数

#endif
