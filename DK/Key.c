/**
 * @file     Key.c
 * @brief    4x4矩阵键盘驱动程序
 * @details  实现4x4矩阵键盘的扫描和按键检测功能：
 *          - 硬件连接：
 *            * 行线：PA8-PA11
 *            * 列线：PB12-PB15
 *          - 扫描原理：
 *            1. 行线轮流输出低电平
 *            2. 检测列线电平状态
 *            3. 根据行列组合确定按键
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "Delay.h"     // 延时函数
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief 列线引脚定义 (PB12-PB15)
 */
#define KEY_COL1_PIN GPIO_Pin_15  /**< 第1列，PB15 */
#define KEY_COL2_PIN GPIO_Pin_14
#define KEY_COL3_PIN GPIO_Pin_13
#define KEY_COL4_PIN GPIO_Pin_12

// 行线引脚定义 (PA8-PA11)
#define KEY_ROW1_PIN GPIO_Pin_11
#define KEY_ROW2_PIN GPIO_Pin_10
#define KEY_ROW3_PIN GPIO_Pin_9
#define KEY_ROW4_PIN GPIO_Pin_8

/**
 * @brief  矩阵键盘初始化
 * @details 完成以下配置：
 *         1. 使能GPIO时钟（GPIOA和GPIOB）
 *         2. 配置行线为推挽输出（PA8-PA11）
 *         3. 配置列线为上拉输入（PB12-PB15）
 *         4. 设置行线初始状态为高电平
 * @param  无
 * @return 无
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
 * @brief  获取按键键码
 * @details 通过扫描矩阵键盘获取按键值：
 *         1. 扫描方式：
 *            - 依次将每一行设为低电平
 *            - 检测所有列的状态
 *            - 根据当前行和检测到的列计算键码
 *         2. 键码对应关系：
 *            - 0：无按键按下
 *            - 1-16：对应矩阵键盘的16个按键
 *            - 按键布局：
 *              [1 ] [2 ] [3 ] [4 ]
 *              [5 ] [6 ] [7 ] [8 ]
 *              [9 ] [10] [11] [12]
 *              [13] [14] [15] [16]
 * @note   此函数为阻塞式操作，会等待按键释放
 * @param  无
 * @return uint8_t 按键键码（0-16）
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
