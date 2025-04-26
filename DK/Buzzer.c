#include "Buzzer.h"

// 初始化蜂鸣器
void Buzzer_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能 GPIO 时钟
    RCC_APB2PeriphClockCmd(Buzzer_GPIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = Buzzer_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStructure);

    // 默认关闭蜂鸣器
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);
}

// 打开蜂鸣器
void Buzzer_ON(void) {
    GPIO_ResetBits(Buzzer_GPIO_Port, Buzzer_Pin); // 低电平触发
}

// 关闭蜂鸣器
void Buzzer_OFF(void) {
    GPIO_SetBits(Buzzer_GPIO_Port, Buzzer_Pin);  // 高电平关闭
}
