/**
 * @file     BT.h
 * @brief    蓝牙通信模块驱动程序头文件
 * @details  定义了蓝牙通信相关的数据结构、宏和函数接口
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#ifndef __BT_H
#define __BT_H

#include "dk_C8T6.h"
#include "DHT11.h"
#include <stdint.h>

/**
 * @brief 蓝牙数据包结构体
 * @details 定义了蓝牙通信协议的数据包格式：
 *         - 帧头(0xA5)
 *         - 标志位字节
 *         - 校验和
 *         - 帧尾(0x5A)
 */
typedef struct {
    uint8_t header;   /**< 帧头，固定为0xA5 */
    uint8_t flags;    /**< 5个标志位，用于控制不同功能 */
    uint8_t checksum; /**< 校验和字节 */
    uint8_t footer;   /**< 帧尾，固定为0x5A */
} BT_Packet_t;

/** @brief 接收缓冲区，用于存储接收到的数据 */
extern uint8_t BT_RxBuffer[4];
/** @brief 解析后的数据包结构体 */
extern BT_Packet_t BT_Packet;
/** @brief 接收完成标志，1表示接收到完整数据包 */
extern uint8_t BT_RxFlag;

/**
 * @brief  初始化蓝牙模块
 * @return 无
 */
void BT_Init(void);

/**
 * @brief  发送字符串到蓝牙模块
 * @param  String 要发送的字符串指针
 * @return 无
 */
void BT_SendString(char *String);

/**
 * @brief  发送数据包到蓝牙模块
 * @param  count   计数值
 * @param  uvLevel 紫外线等级(0-11)
 * @param  humi    湿度值(浮点数)
 * @param  temp    温度值(浮点数)
 * @return 无
 */
void BT_SendDataPacket(uint8_t count, uint8_t uvLevel, float humi, float temp);

/**
 * @brief  解析接收到的蓝牙数据包
 * @return 解析结果：
 *         - 0  : 解析成功
 *         - -1 : 校验和错误
 *         - -2 : 帧头帧尾错误
 */
int8_t BT_ParsePacket(void);

#endif // __BT_H
