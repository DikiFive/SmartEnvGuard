#ifndef __BT_H
#define __BT_H

#include "dk_C8T6.h"

extern char BT_RxPacket[100];
extern uint8_t BT_RxFlag;

void BT_Init(void);
void BT_SendString(char *String);
void BT_SendDataPacket(uint16_t count, uint8_t uvLevel, DHT11_Data_TypeDef dht11_data);

#endif // __BT_H
