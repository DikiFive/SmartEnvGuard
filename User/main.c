#include "DK_C8T6.h"

int main(void)
{
    Sys_Init(); // 系统初始化
    while (1) {

        int keyValue = Key_GetNum();     // 获取按键值并存储
        OLED_ShowNum(1, 1, keyValue, 2); // 显示按键值
        if (keyValue == 9) {             // 如果按键值为9
            LED1_Turn();
        }
    }
}
