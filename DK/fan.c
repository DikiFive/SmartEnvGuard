#include "fan.h"

// 初始化风扇
void Fan_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(Fan_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = Fan_Pin;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Fan_GPIO_Port, &GPIO_InitStructure);

    // 默认关闭风扇
    GPIO_SetBits(Fan_GPIO_Port, Fan_Pin); // Updated to use Fan_Pin
}

// 打开风扇
void Fan_ON(void)
{
    GPIO_ResetBits(Fan_GPIO_Port, Fan_Pin); // 低电平触发
}

// 关闭风扇
void Fan_OFF(void)
{
    GPIO_SetBits(Fan_GPIO_Port, Fan_Pin); // 高电平关闭
}
