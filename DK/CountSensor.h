#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

extern uint8_t CountSensor_Count; // 计数值

void CountSensor_Init(void);
uint8_t CountSensor_Get(void);

#endif // __COUNT_SENSOR_H