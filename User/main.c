#include "DK_C8T6.h"

int main(void)
{
    Sys_Init();    // 系统初始化
    Buzzer_Init(); // 初始化蜂鸣器

    int keyValue = 0;                // 初始化按键值为0，用于存储上一次的按键值
    OLED_ShowNum(1, 1, keyValue, 2); // 初始显示0
    OLED_ShowNum(2, 1, keyValue, 16);
    OLED_ShowNum(3, 1, keyValue, 16);
    OLED_ShowNum(4, 1, keyValue, 16);

    while (1) {
        int currentKeyValue = Key_GetNum(); // 获取当前按键值

        if (currentKeyValue != 0) {           // 如果检测到按键按下（键值不为0）
            keyValue = currentKeyValue;       // 更新存储的按键值
            OLED_ShowNum(1, 1, keyValue, 16); // 显示新的按键值

            if (keyValue == 1) {        // 如果按键值为1
                Buzzer_ON();            // 打开蜂鸣器
            } else if (keyValue == 2) { // 如果按键值为2
                Buzzer_OFF();           // 关闭蜂鸣器
            } else if (keyValue == 9) { // 如果按键值为9
                LED_Sys_Turn();
                Motor_SetSpeed(20); // 设置电机速度为20
                Servo_SetAngle(90);
            } else if (keyValue == 10) { // 如果按键值为10
                LED_Sys_Turn();
                Motor_SetSpeed(-20);     // 设置电机速度为-20，反转
                Servo_SetAngle(0);       // 设置舵机角度为0
            } else if (keyValue == 11) { // 如果按键值为11
                LED_Sys_Turn();
                Motor_SetSpeed(0);       // 设置电机停止
                Servo_SetAngle(180);     // 设置舵机角度为180
            } else if (keyValue == 12) { // 如果按键值为12
                LED_Sys_Turn();
                Motor_SetSpeed(100); // 设置电机速度为100
                Servo_SetAngle(45);  // 设置舵机角度为45
            }
            // 如果currentKeyValue为0，则不更新OLED显示，保持上一次的keyValue
        }
    }
}
