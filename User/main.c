#include "DK_C8T6.h"

int main(void)
{
    Sys_Init(); // 系统初始化

    // 初始显示
    OLED_UpdateDisplay(0, 1, 0, 0, 0, 0, 0, 0, 0);

    while (1) {
        ProcessSystemTasks(); // 处理系统主要任务
        Delay_ms(1);          // 短暂延时，避免CPU占用过高
    }
}
