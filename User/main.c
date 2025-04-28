#include "DK_C8T6.h"

// 温湿度阈值定义
#define TEMP_THRESHOLD 31 // 温度阈值（31°C）
#define HUMI_THRESHOLD 61 // 湿度阈值（61%）

// 声明外部变量
extern SystemMode_t currentMode;

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

        // 计算温湿度浮点数值
        float humi = (float)sensorData.humi_int + (float)sensorData.humi_deci / 10.0;
        float temp = (float)sensorData.temp_int + (float)sensorData.temp_deci / 10.0;
        BT_SendDataPacket(sensorData.redValue, sensorData.uvLevel, humi, temp);

        // 温湿度控制逻辑 (仅在手动模式下执行)
        if (currentMode == MODE_MANUAL) {
            if (sensorData.dht11_status == 0) { // 仅在DHT11读取成功时执行
                if (temp > TEMP_THRESHOLD && humi > HUMI_THRESHOLD) {
                    Fan_ON(); // 开启风扇
                    // UV_ON(); // 在手动模式下，紫外线灯由按键控制，不受温湿度影响
                } else {
                    // 在手动模式下且不满足温湿度条件时，关闭风扇
                    Fan_OFF(); // 关闭风扇
                    // UV_OFF();  // 在手动模式下，紫外线灯由按键控制，不受温湿度影响
                }
            }
             // 在手动模式下，紫外线灯的开关完全由按键控制，不受温湿度影响
        }
        // 注意：自动模式下的风扇和紫外线灯控制在TIM4中断中实现

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
