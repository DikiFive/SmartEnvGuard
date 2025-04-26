#include "BT.h"
#include "stm32f10x.h"
#include <stdio.h>

char BT_RxPacket[100]; // 蓝牙接收数据包
uint8_t BT_RxFlag = 0; // 蓝牙接收标志

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
    USART_InitStructure.USART_BaudRate            = 115200;
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

void BT_SendString(char *String)
{
    while (*String) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *String++);
    }
}

void BT_SendDataPacket(uint16_t count, uint8_t uvLevel, DHT11_Data_TypeDef dht11_data)
{
    uint8_t packet[10];
    uint8_t checksum = 0;

    packet[0] = 0xA5; // 帧头
    packet[1] = (count >> 8) & 0xFF; // count 高字节
    packet[2] = count & 0xFF; // count 低字节
    packet[3] = uvLevel; // uvLevel
    packet[4] = dht11_data.humi_int; // 湿度整数部分
    packet[5] = dht11_data.humi_deci; // 湿度小数部分
    packet[6] = dht11_data.temp_int; // 温度整数部分
    packet[7] = dht11_data.temp_deci; // 温度小数部分

    // 计算校验和
    for (int i = 1; i < 8; i++) {
        checksum += packet[i];
    }
    packet[8] = checksum; // 校验和
    packet[9] = 0x5A; // 帧尾

    // 发送数据包
    for (int i = 0; i < 10; i++) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, packet[i]);
    }
}

/**
 * @brief USART2中断服务例程
 *
 * 本函数用于处理USART2的中断请求，主要负责串口接收数据的处理
 * 它通过检查接收数据寄存器非空中断标志来决定是否读取数据
 * 接收到的数据将根据特定协议进行处理，以组装成完整的数据包
 */
void USART2_IRQHandler(void)
{
    // 静态变量，用于记录接收状态
    static uint8_t RxState = 0;
    // 静态变量，用于指向接收到的数据包中的下一个字节位置
    static uint8_t pRxPacket = 0;

    // 检查USART2的接收数据寄存器非空中断状态是否设置
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET) {
        // 读取接收到的数据
        uint8_t RxData = USART_ReceiveData(USART2);

        // 回显功能：将接收到的数据立即发送回去
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, RxData);

        // 根据接收状态机处理数据
        if (RxState == 0) {
            // 状态0：等待起始字符'@'
            if (RxData == '@' && BT_RxFlag == 0) {
                // 接收到起始字符，初始化接收状态和数据包指针
                RxState   = 1;
                pRxPacket = 0;
            }
        } else if (RxState == 1) {
            // 状态1：接收数据包内容
            if (RxData == '\r') {
                // 数据包内容结束，进入状态2
                RxState = 2;
            } else {
                // 继续接收数据包内容
                BT_RxPacket[pRxPacket++] = RxData;
            }
        } else if (RxState == 2) {
            // 状态2：等待数据包结束字符'\n'
            if (RxData == '\n') {
                // 数据包接收完成，终止数据包，重置接收状态，设置接收标志
                RxState                = 0;
                BT_RxPacket[pRxPacket] = '\0';
                BT_RxFlag              = 1;
            }
        }

        // 清除接收数据寄存器非空中断标志
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}
