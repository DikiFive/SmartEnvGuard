
#include "DK_C8T6.h"

int main(void)
{
    DHT11_Data_TypeDef DHT11_Data;
    int keyValue    = 0;    // 初始化按键值为0，用于存储上一次的按键值
    uint8_t count   = 0;    /// 计数传感器的计数值
    uint8_t uvLevel = 0;    // 紫外线强度等级
    float humi      = 0.0f; // 湿度值
    float temp      = 0.0f; // 温度值

    Sys_Init(); // 系统初始化

    OLED_ShowNum(1, 1, keyValue, 2); // 初始显示0

    while (1) {
        int currentKeyValue = Key_GetNum(); // 获取当前按键值

        if (currentKeyValue != 0) {           // 如果检测到按键按下（键值不为0）
            keyValue = currentKeyValue;       // 更新存储的按键值
            OLED_ShowNum(1, 1, keyValue, 16); // 显示新的按键值

            if (keyValue == 1) {        // 如果按键值为1
                Buzzer_ON();            // 打开蜂鸣器
                Fan_ON();               //
            } else if (keyValue == 2) { // 如果按键值为2
                Buzzer_OFF();           // 关闭蜂鸣器
                Fan_OFF();              //
            } else if (keyValue == 9) { // 如果按键值为9
                LED_Sys_Turn();
                Motor_SetSpeed(20); // 设置电机速度为20
                Servo_SetAngle(90);
            } else if (keyValue == 10) { // 如果按键值为10
                LED_Sys_Turn();
                Motor_SetSpeed(-20);     // 设置电机速度为-20，反转
                Servo_SetAngle(0);       // 设置舵机角度为0
            } else if (keyValue == 11) { // 如果按键值为11
                LED_Sys_Turn();
                Motor_SetSpeed(0);       // 设置电机停止
                Servo_SetAngle(180);     // 设置舵机角度为180
            } else if (keyValue == 12) { // 如果按键值为12
                LED_Sys_Turn();
                Motor_SetSpeed(100); // 设置电机速度为100
                Servo_SetAngle(45);  // 设置舵机角度为45
            }
            // 如果currentKeyValue为0，则不更新OLED显示，保持上一次的keyValue
        }
        /*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
        if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS) {
            OLED_ShowString(1, 1, "DHT11 Success!");
            OLED_ShowNum(2, 1, DHT11_Data.humi_int, 2);
            OLED_ShowString(2, 3, ".");
            OLED_ShowNum(2, 4, DHT11_Data.humi_deci, 1);
            OLED_ShowString(2, 5, "%RH");
            OLED_ShowNum(3, 1, DHT11_Data.temp_int, 2);
            OLED_ShowString(3, 3, ".");
            OLED_ShowNum(3, 4, DHT11_Data.temp_deci, 1);
            OLED_ShowString(3, 5, "C");
        } else {
            OLED_ShowString(1, 1, "DHT11 ERROR!");
        }

        // 读取SD12紫外线传感器数据
        uint16_t uvValue = SD12_GetADCValue(10); // 采集10次取平均值
        uvLevel          = SD12_GetIntensity(uvValue);

        // 在OLED上显示紫外线强度等级
        OLED_ShowString(4, 1, "UV Level:");
        OLED_ShowNum(4, 10, uvLevel, 2);

        count = CountSensor_Get(); // 获取计数传感器的计数值
        OLED_ShowString(3, 7, "Count:");
        OLED_ShowNum(3, 13, count, 4);
        humi = (float)DHT11_Data.humi_int + (float)DHT11_Data.humi_deci / 10.0;
        temp = (float)DHT11_Data.temp_int + (float)DHT11_Data.temp_deci / 10.0;
        BT_SendDataPacket(count, uvLevel, humi, temp); // 发送数据包
        Delay_ms(10);                                  // 延时50ms
    }
}
