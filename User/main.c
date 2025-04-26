#include "DK_C8T6.h"

int main(void)
{
    Sys_Init();                      // 系统初始化
    int keyValue = 0;                // 初始化按键值为0，用于存储上一次的按键值
    OLED_ShowNum(1, 1, keyValue, 2); // 初始显示0

    while (1) {
        int currentKeyValue = Key_GetNum(); // 获取当前按键值

        if (currentKeyValue != 0) {          // 如果检测到按键按下（键值不为0）
            keyValue = currentKeyValue;      // 更新存储的按键值
            OLED_ShowNum(1, 1, keyValue, 2); // 显示新的按键值

            if (keyValue == 9) { // 如果按键值为9
                LED_Sys_Turn();
            }
        }
        // 如果currentKeyValue为0，则不更新OLED显示，保持上一次的keyValue
    }
}
