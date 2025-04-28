/**
 * @file     Serial.h
 * @brief    串口通信驱动程序头文件
 * @details  声明串口通信相关的函数接口：
 *          - 初始化函数
 *          - 数据发送函数
 *          - printf重定向函数
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdint.h>

/**
 * @brief  串口初始化
 * @details 配置USART1：
 *         - PA9（TX）和PA10（RX）
 *         - 115200波特率
 *         - 8位数据位
 *         - 1位停止位
 *         - 无校验
 *         - 使能接收中断
 * @param  无
 * @return 无
 */
void Serial_Init(void);

/**
 * @brief  发送一个字节
 * @param  Byte 要发送的字节数据
 * @return 无
 */
void Serial_SendByte(uint8_t Byte);

/**
 * @brief  发送字符串
 * @param  String 要发送的字符串指针，以'\0'结尾
 * @return 无
 */
void Serial_SendString(char *String);

/**
 * @brief  发送数组
 * @param  Array 要发送的数组指针
 * @param  Length 数组长度
 * @return 无
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length);

#endif /* __SERIAL_H */
