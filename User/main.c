#include "DK_C8T6.h"

int main(void)
{
    BTStatus_t btStatus   = {0};    // 初始化蓝牙状态
    KeyStatus_t keyStatus = {0, 0}; // 初始化按键状态

    Sys_Init(); // 系统初始化

    // 初始显示
    OLED_UpdateDisplay(0, 1, 0, 0, 0, 0, 0, 0, btStatus.status);

    while (1) {
        // 处理按键输入
        keyStatus = HandleKeyPress(Key_GetNum());

        // 获取所有传感器数据
        SensorData_t sensorData = GetAllSensorData();

        // 计算温湿度浮点数值并发送蓝牙数据包
        float humi = (float)sensorData.humi_int + (float)sensorData.humi_deci / 10.0;
        float temp = (float)sensorData.temp_int + (float)sensorData.temp_deci / 10.0;
        BT_SendDataPacket(sensorData.redValue, sensorData.uvLevel, humi, temp);

        // 处理蓝牙数据
        btStatus = HandleBluetooth();

        // 更新显示
        OLED_UpdateDisplay(keyStatus.keyValue, sensorData.dht11_status,
                           sensorData.humi_int, sensorData.humi_deci,
                           sensorData.temp_int, sensorData.temp_deci,
                           sensorData.uvLevel, sensorData.redValue, btStatus.status);

        Delay_ms(10); // 延时10ms
    }
}
