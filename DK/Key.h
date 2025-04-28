/**
 * @file     Key.h
 * @brief    4x4矩阵键盘驱动程序头文件
 * @details  定义了矩阵键盘相关的：
 *          - 硬件参数
 *          - 功能函数接口
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

/**
 * @brief 矩阵键盘参数定义
 */
#define KEY_ROW_NUM 4  /**< 行数 */
#define KEY_COL_NUM 4  /**< 列数 */

/**
 * @brief  矩阵键盘初始化函数
 * @details 配置GPIO引脚和默认状态：
 *         - 行线（PA8-PA11）配置为推挽输出
 *         - 列线（PB12-PB15）配置为上拉输入
 * @param  无
 * @return 无
 */
void Key_Init(void);

/**
 * @brief  获取按键键码
 * @details 按键编号定义：
 *         [1 ] [2 ] [3 ] [4 ]
 *         [5 ] [6 ] [7 ] [8 ]
 *         [9 ] [10] [11] [12]
 *         [13] [14] [15] [16]
 * @param  无
 * @return uint8_t 按键键码：
 *         - 0：无按键按下
 *         - 1-16：对应的按键编号
 */
uint8_t Key_GetNum(void);

#endif /* __KEY_H */
