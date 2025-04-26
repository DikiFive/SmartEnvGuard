#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_ROW_NUM 4
#define KEY_COL_NUM 4

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
