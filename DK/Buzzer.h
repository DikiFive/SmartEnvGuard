#ifndef __BUZZER_H
#define __BUZZER_H

#include "dk_C8T6.h"

// 定义蜂鸣器连接的GPIO引脚
#define Buzzer_Pin       GPIO_Pin_14
#define Buzzer_GPIO_Port GPIOC
#define Buzzer_GPIO_CLK  RCC_APB2Periph_GPIOC

// 函数原型
void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);

#endif
// __BUZZER_H
