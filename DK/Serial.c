/**
 * @file     Serial.c
 * @brief    串口通信驱动程序
 * @details  实现串口通信功能：
 *          - USART1配置（PA9/PA10）
 *          - 数据收发
 *          - 中断接收
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件
#include <stdio.h>     // 用于printf重定向

/**
 * @brief  串口1初始化
 * @details 完成以下配置：
 *         1. 使能GPIO和USART时钟
 *         2. 配置GPIO引脚：
 *            - PA9（TX）：复用推挽输出
 *            - PA10（RX）：浮空输入
 *         3. 配置USART1参数：
 *            - 波特率：115200
 *            - 8位数据位
 *            - 1位停止位
 *            - 无校验
 *         4. 配置接收中断
 * @param  无
 * @return 无
 */
void Serial_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // USART1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA时钟

    /*GPIO配置*/
    GPIO_InitTypeDef GPIO_InitStructure;
    // PA9(TX)配置为复用推挽输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA10(RX)配置为浮空输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*USART配置*/
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;                  // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;   // 8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;       // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;          // 无校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 收发模式
    USART_Init(USART1, &USART_InitStructure);

    /*中断配置*/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 使能接收中断

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 配置NVIC分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);

    /*使能USART*/
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  发送一个字节
 * @param  Byte 要发送的字节
 * @return 无
 */
void Serial_SendByte(uint8_t Byte)
{
    USART_SendData(USART1, Byte);                         // 发送数据
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 等待发送完成
}

/**
 * @brief  发送字符串
 * @param  String 要发送的字符串指针
 * @return 无
 */
void Serial_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        Serial_SendByte(String[i]);
    }
}

/**
 * @brief  发送数组
 * @param  Array 数组指针
 * @param  Length 数组长度
 * @return 无
 */
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        Serial_SendByte(Array[i]);
    }
}

/**
 * @brief  printf函数重定向
 * @details 重定向后可以直接使用printf函数通过串口输出
 */
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);
    return ch;
}

/**
 * @brief  USART1中断服务函数
 * @details 处理接收到的数据
 * @note   此函数会被硬件自动调用
 */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
        uint8_t RxData = USART_ReceiveData(USART1);  // 读取接收到的数据
        Serial_SendByte(RxData);                      // 回显数据（用于测试）
        USART_ClearITPendingBit(USART1, USART_IT_RXNE); // 清除中断标志位
    }
}
