#ifndef __BT_H
#define __BT_H

#include "dk_C8T6.h"

extern char BT_RxPacket[100];
extern uint8_t BT_RxFlag;

void BT_Init(void);
void BT_SendString(char *String);

#endif // __BT_H
