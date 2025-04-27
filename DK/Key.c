#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "dk_C8T6.h"

// 列线引脚定义 (PB12-PB15)
#define KEY_COL1_PIN GPIO_Pin_15
#define KEY_COL2_PIN GPIO_Pin_14
#define KEY_COL3_PIN GPIO_Pin_13
#define KEY_COL4_PIN GPIO_Pin_12

// 行线引脚定义 (PA8-PA11)
#define KEY_ROW1_PIN GPIO_Pin_11
#define KEY_ROW2_PIN GPIO_Pin_10
#define KEY_ROW3_PIN GPIO_Pin_9
#define KEY_ROW4_PIN GPIO_Pin_8

/**
 * 函    数：按键初始化
 * 参    数：无
 * 返 回 值：无
 */
void Key_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    /*行线初始化(推挽输出)*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN | KEY_ROW4_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*列线初始化(上拉输入)*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin  = KEY_COL1_PIN | KEY_COL2_PIN | KEY_COL3_PIN | KEY_COL4_PIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*初始状态：所有行线置高*/
    GPIO_SetBits(GPIOA, KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN | KEY_ROW4_PIN);
}

/**
 * 函    数：按键获取键码
 * 参    数：无
 * 返 回 值：按下按键的键码值，范围：1~16对应矩阵键盘，0代表没有按键按下
 * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
 */
uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    uint8_t row, col;
    uint16_t rowPins[4] = {KEY_ROW1_PIN, KEY_ROW2_PIN, KEY_ROW3_PIN, KEY_ROW4_PIN};
    uint16_t colPins[4] = {KEY_COL1_PIN, KEY_COL2_PIN, KEY_COL3_PIN, KEY_COL4_PIN};

    /* 扫描每一行 */
    for (row = 0; row < 4; row++) {
        /* 当前行置低，其他行置高 */
        // 设置当前行为低电平，其他行为高电平
        GPIO_WriteBit(GPIOA, rowPins[0], (row == 0) ? Bit_RESET : Bit_SET);
        GPIO_WriteBit(GPIOA, rowPins[1], (row == 1) ? Bit_RESET : Bit_SET);
        GPIO_WriteBit(GPIOA, rowPins[2], (row == 2) ? Bit_RESET : Bit_SET);
        GPIO_WriteBit(GPIOA, rowPins[3], (row == 3) ? Bit_RESET : Bit_SET);

        /* 检查每一列 */
        for (col = 0; col < 4; col++) {
            if (GPIO_ReadInputDataBit(GPIOB, colPins[col]) == 0) // 检测到按键按下
            {
                // 在这里添加调试显示代码
                // OLED_ShowNum(2, 1, row, 1); // 显示行号
                // OLED_ShowNum(2, 3, col, 1); // 显示列号
                // OLED_ShowNum(3, 1, 88, 2);  // 显示一个固定数字，表示进入了按键检测分支

                Delay_ms(20);                                            // 延时消抖
                while (GPIO_ReadInputDataBit(GPIOB, colPins[col]) == 0); // 等待按键释放
                Delay_ms(20);                                            // 延时消抖
                KeyNum = (3 - col) * 4 + (3 - row) + 1;                  // 计算键值(1-16)
                break;
            }
        }

        /* 恢复所有行线为高 */
        GPIO_SetBits(GPIOA, KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN | KEY_ROW4_PIN);

        if (KeyNum != 0) break; // 已检测到按键，退出扫描
    }

    return KeyNum;
}
