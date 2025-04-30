/**
 * @file     OLED.h
 * @brief    OLED显示屏驱动程序头文件
 * @details  声明OLED显示相关的函数接口，包括：
 *          - 初始化函数
 *          - 显示控制函数
 *          - 字符显示函数
 *          - 数字显示函数
 * @note     适用于0.96寸OLED显示屏（128x64分辨率）
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __OLED_H
#define __OLED_H

/**
 * @brief  OLED初始化
 * @details 包括I2C接口初始化和显示参数配置
 * @param  无
 * @return 无
 */
void OLED_Init(void);

/**
 * @brief  清空显示
 * @details 将显存中所有数据清零
 * @param  无
 * @return 无
 */
void OLED_Clear(void);

/**
 * @brief  显示一个字符
 * @param  Line 行号，范围1~4
 * @param  Column 列号，范围1~16
 * @param  Char 要显示的字符（ASCII可见字符）
 * @return 无
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);

/**
 * @brief  显示字符串
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  String 要显示的字符串，以'\0'结尾
 * @return 无
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

/**
 * @brief  显示无符号整数
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~4294967295
 * @param  Length 显示长度，范围1~10
 * @return 无
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief  显示有符号整数
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围-2147483648~2147483647
 * @param  Length 显示长度（不含符号），范围1~10
 * @return 无
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

/**
 * @brief  显示16进制数
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~0xFFFFFFFF
 * @param  Length 显示长度，范围1~8
 * @return 无
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

/**
 * @brief  显示2进制数
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~65535
 * @param  Length 显示长度，范围1~16
 * @return 无
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif /* __OLED_H */
