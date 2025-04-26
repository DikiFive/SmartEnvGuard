#include "DK_C8T6.h"

void Sys_Init(void)
{
    BT_Init();          // 初始化蓝牙
    Buzzer_Init();      // 初始化蜂鸣器
    CountSensor_Init(); // 初始化计数传感器
    Fan_Init();         // 初始化风扇
    LED_Sys_Init();     // 初始化LED灯
    Key_Init();         // 初始化按键
    Motor_Init();       // 初始化电机
    SD12_Init();        // 初始化SD12紫外线传感器
    DHT11_Init();       // 初始化DHT11传感器
    Servo_Init();       // 初始化舵机
    OLED_Init();        // 初始化OLED屏幕
}