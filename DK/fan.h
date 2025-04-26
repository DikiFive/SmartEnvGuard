#ifndef __FAN_H
#define __FAN_H

#include "dk_C8T6.h"

// 定义风扇连接的GPIO引脚
#define Fan_Pin       GPIO_Pin_15
#define Fan_GPIO_Port GPIOC
#define Fan_GPIO_CLK  RCC_APB2Periph_GPIOC

// 函数原型
void Fan_Init(void);
void Fan_ON(void);
void Fan_OFF(void);

#endif
// __FAN_H
