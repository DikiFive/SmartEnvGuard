#ifndef __BT_H
#define __BT_H

#include "dk_C8T6.h"
#include "DHT11.h"
#include <stdint.h>

typedef struct {
    uint8_t header;   // 帧头 0xA5
    uint8_t flags;    // 5个标志位
    uint8_t checksum; // 校验位
    uint8_t footer;   // 帧尾 0x5A
} BT_Packet_t;

extern uint8_t BT_RxBuffer[4]; // 接收缓冲区
extern BT_Packet_t BT_Packet;  // 解析后的数据包
extern uint8_t BT_RxFlag;

void BT_Init(void);
void BT_SendString(char *String);
void BT_SendDataPacket(uint8_t count, uint8_t uvLevel, float humi, float temp);
int8_t BT_ParsePacket(void);

#endif // __BT_H
