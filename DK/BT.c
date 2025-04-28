/**
 * @file     BT.c
 * @brief    蓝牙通信模块驱动程序
 * @details  实现基于USART2的蓝牙通信功能，包括：
 *          - 蓝牙串口初始化
 *          - 数据发送功能
 *          - 数据接收及解析功能
 *          - 状态监控及错误处理
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#include "BT.h"

/** @brief 蓝牙接收缓冲区大小为4字节 */
uint8_t BT_RxBuffer[4];
/** @brief 蓝牙接收完成标志，1表示接收到完整数据包 */
uint8_t BT_RxFlag = 0;
/** @brief 解析后的数据包结构体 */
BT_Packet_t BT_Packet;

/**
 * @brief  蓝牙模块初始化
 * @details 完成以下配置：
 *         1. 初始化USART2引脚（PA2-TX, PA3-RX）
 *         2. 配置串口参数（9600波特率，8位数据，1位停止，无校验）
 *         3. 启用接收中断
 *         4. 配置NVIC中断优先级
 * @param  无
 * @return 无
 */
void BT_Init(void)
{
    /* 开启时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // USART2在APB1总线
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA时钟

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; // PA2作为TX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3; // PA3作为RX
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART初始化 */
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate            = 9600;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStructure);

    /* 中断配置 */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&NVIC_InitStructure);

    /* USART使能 */
    USART_Cmd(USART2, ENABLE);
}

/**
 * @brief  发送字符串到蓝牙模块
 * @param  String 要发送的以'\0'结尾的字符串
 * @return 无
 */
void BT_SendString(char *String)
{
    while (*String) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *String++);
    }
}

/**
 * @brief  发送传感器数据包到蓝牙模块
 * @details 数据包格式：
 *         - 帧头(1字节): 0xA5
 *         - 计数值(1字节)
 *         - UV等级(1字节)
 *         - 湿度(4字节float)
 *         - 温度(4字节float)
 *         - 校验和(1字节)
 *         - 帧尾(1字节): 0x5A
 * @param  count   计数值
 * @param  uvLevel 紫外线等级(0-11)
 * @param  humi    湿度值(浮点数)
 * @param  temp    温度值(浮点数)
 * @return 无
 */
void BT_SendDataPacket(uint8_t count, uint8_t uvLevel, float humi, float temp)
{
    uint8_t packet[13]; // 数据包长度增加
    uint8_t checksum = 0;
    uint8_t *pFloat;

    packet[0] = 0xA5;    // 帧头
    packet[1] = count;   // count
    packet[2] = uvLevel; // uvLevel

    // 湿度数据
    pFloat    = (uint8_t *)&humi;
    packet[3] = pFloat[0];
    packet[4] = pFloat[1];
    packet[5] = pFloat[2];
    packet[6] = pFloat[3];

    // 温度数据
    pFloat     = (uint8_t *)&temp;
    packet[7]  = pFloat[0];
    packet[8]  = pFloat[1];
    packet[9]  = pFloat[2];
    packet[10] = pFloat[3];

    // 计算校验和
    for (int i = 1; i < 11; i++) {
        checksum += packet[i];
    }
    packet[11] = checksum; // 校验和
    packet[12] = 0x5A;     // 帧尾

    // 发送数据包
    for (int i = 0; i < 13; i++) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, packet[i]);
    }
}

/**
 * @brief  USART2中断服务函数
 * @details 实现以下功能：
 *         1. 接收数据状态机:
 *            - 状态0：等待帧头(0xA5)
 *            - 状态1：接收数据内容
 *            - 状态2：接收包尾
 *         2. 数据包组装和存储
 *         3. 接收完成标志设置
 * @note   本函数为中断服务函数，由硬件自动调用
 * @param  无
 * @return 无
 */
void USART2_IRQHandler(void)
{
    // 静态变量，用于记录接收状态
    static uint8_t RxState = 0;
    // 静态变量，用于存储接收到的数据包
    static uint8_t RxPacket[8];
    // 静态变量，用于记录接收位置
    static uint8_t RxPos = 0;

    // 检查USART2的接收数据寄存器非空中断状态是否设置
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        // 读取接收到的数据
        uint8_t RxData = USART_ReceiveData(USART2);

        // 根据接收状态机处理数据
        if (RxState == 0) {
            // 状态0：等待帧头0xA5
            if (RxData == 0xA5 && BT_RxFlag == 0) {
                RxState = 1;
                RxPos = 0;
                RxPacket[RxPos++] = RxData; // 存储帧头
            }
        } else if (RxState == 1) {
            // 状态1：接收数据包内容
            RxPacket[RxPos++] = RxData;
            if (RxPos >= 8) { // 已接收完整数据包
                // 将数据包复制到接收缓冲区
                memcpy(BT_RxBuffer, RxPacket, 8);
                // 设置接收标志
                BT_RxFlag = 1;
                RxState = 0; // 重置状态机
            }
        }

        // 清除接收数据寄存器非空中断标志
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

/**
 * @brief  解析接收到的蓝牙数据包
 * @details 完成以下操作：
 *         1. 校验数据包格式（帧头0xA5，帧尾0x5A）
 *         2. 计算并验证校验和
 *         3. 解析各个标志位到结构体中
 * @return 解析结果：
 *         - 0  : 解析成功
 *         - -1 : 校验和错误
 *         - -2 : 帧头帧尾错误
 */
int8_t BT_ParsePacket(void)
{
    uint8_t checksum = 0;

    // 检查帧头和帧尾
    if (BT_RxBuffer[0] != 0xA5 || BT_RxBuffer[3] != 0x5A) {
        return -2;
    }

    // 计算校验和(5个标志位之和)
    checksum = BT_RxBuffer[1];

    // 验证校验和
    if (BT_RxBuffer[2] != checksum) {
        return -1;
    }

    // 解析数据到结构体
    BT_Packet.header = BT_RxBuffer[0];
    BT_Packet.flags = BT_RxBuffer[1];
    BT_Packet.checksum = BT_RxBuffer[2];
    BT_Packet.footer = BT_RxBuffer[3];

    return 0;
}
