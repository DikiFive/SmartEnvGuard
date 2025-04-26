#include "DK_C8T6.h"

void Sys_Init(void)
{
    BT_Init();      // 初始化蓝牙
    LED_Init();     // 初始化LED灯
    LED_Sys_Init(); // 初始化LED灯
    Key_Init();     // 初始化按键
    // Motor_Init(); // 初始化电机
    // PWM_Init();   // 初始化PWM
    // Servo_Init(); // 初始化舵机
    OLED_Init(); // 初始化OLED屏幕
}