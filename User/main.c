#include "DK_C8T6.h"
#include "BT.h"

// 存储上一次显示的值，用于比较是否需要更新
static struct {
    int keyValue;
    uint8_t dht11_status;
    uint8_t humi_int;
    uint8_t humi_deci;
    uint8_t temp_int;
    uint8_t temp_deci;
    uint8_t uvLevel;
    uint8_t redValue;
    int8_t bt_status;
    uint8_t initialized;
} last_display = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void OLED_UpdateDisplay(int keyValue, uint8_t dht11_status,
    uint8_t humi_int, uint8_t humi_deci,
    uint8_t temp_int, uint8_t temp_deci,
    uint8_t uvLevel, uint8_t redValue, int8_t bt_status)
{
    // 首次显示时初始化所有内容
    if (!last_display.initialized) {
        OLED_ShowString(1, 1, "K:");
        OLED_ShowString(1, 8, "D:");
        OLED_ShowString(2, 1, "hm:");
        OLED_ShowString(3, 1, "T:");
        OLED_ShowString(3, 5, ".");
        OLED_ShowString(3, 7, "C UV:");
        OLED_ShowString(4, 1, "R:");
        OLED_ShowString(4, 4, "B:");
        OLED_ShowString(4, 8, "M:");

        last_display.initialized = 1;
    }

    // 更新按键值（如果改变）
    if (last_display.keyValue != keyValue) {
        OLED_ShowNum(1, 3, keyValue, 2);
        last_display.keyValue = keyValue;
    }

    // 更新DHT11状态（只在成功时显示OK，避免频繁闪烁）
    static uint8_t err_count = 0;
    if (dht11_status == 0) {
        // 成功时立即显示OK
        if (last_display.dht11_status != 0) {
            OLED_ShowString(1, 10, "OK ");
            last_display.dht11_status = 0;
        }
        err_count = 0;
    } else {
        // 错误时累计3次才显示ERR
        err_count++;
        if (err_count >= 3 && last_display.dht11_status == 0) {
            OLED_ShowString(1, 10, "ERR");
            last_display.dht11_status = 1;
        }
    }

    // 更新湿度（如果改变）
    float humi = humi_int + humi_deci / 10.0f;
    int humi_display = (int)(humi * 10);  // 保留一位小数
    static int last_humi_display = -1;

    if (last_humi_display != humi_display) {
        OLED_ShowNum(2, 4, humi_display/10, 2);     // 整数部分
        OLED_ShowString(2, 6, ".");                 // 小数点
        OLED_ShowNum(2, 7, humi_display%10, 1);    // 小数部分
        OLED_ShowString(2, 8, "%");                // 百分号
        last_humi_display = humi_display;
    }

    // 更新温度（如果改变）
    if (last_display.temp_int != temp_int) {
        OLED_ShowNum(3, 3, temp_int, 2);
        last_display.temp_int = temp_int;
    }
    if (last_display.temp_deci != temp_deci) {
        OLED_ShowNum(3, 6, temp_deci, 1);
        last_display.temp_deci = temp_deci;
    }

    // 更新紫外线等级（如果改变）
    if (last_display.uvLevel != uvLevel) {
        OLED_ShowNum(3, 12, uvLevel, 2);
        last_display.uvLevel = uvLevel;
    }

    // 更新红外值（如果改变）
    if (last_display.redValue != redValue) {
        OLED_ShowNum(4, 3, redValue, 1);  // 只显示0或1
        last_display.redValue = redValue;
    }

    // 更新蓝牙状态（如果改变）
    static uint8_t last_bt_rx = 0;
    if (BT_RxFlag == 1 && !last_bt_rx) {
        OLED_ShowString(4, 6, "R");  // 显示接收标志
    } else if (BT_RxFlag == 0 && last_bt_rx) {
        OLED_ShowString(4, 6, " ");  // 清除接收标志
    }
    last_bt_rx = BT_RxFlag;

    // 更新Mode标志（如果改变）
    static uint8_t last_mode = 0xFF;
    uint8_t current_mode = (BT_Packet.flags >> 4) & 0x01;
    if (last_mode != current_mode) {
        OLED_ShowNum(4, 10, current_mode, 1);
        last_mode = current_mode;
    }
}

int main(void)
{
    DHT11_Data_TypeDef DHT11_Data;
    int keyValue    = 0;    // 初始化按键值为0，用于存储上一次的按键值
    uint8_t RED     = 0;    /// 红外传感器的计数值
    uint8_t uvLevel = 0;    // 紫外线强度等级
    float humi      = 0.0f; // 湿度值
    float temp      = 0.0f; // 温度值
    uint8_t dht11_status = 1; // DHT11状态，0表示成功
    int8_t bt_status = 0;     // 蓝牙状态，0表示成功

    Sys_Init(); // 系统初始化

    // 初始显示
    OLED_UpdateDisplay(keyValue, dht11_status, 0, 0, 0, 0, 0, 0, bt_status);

    while (1) {
        int currentKeyValue = Key_GetNum(); // 获取当前按键值

        if (currentKeyValue != 0) {     // 如果检测到按键按下（键值不为0）
            keyValue = currentKeyValue; // 更新存储的按键值

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
        }

        /*调用DHT11_Read_TempAndHumidity读取温湿度*/
        dht11_status = (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS) ? 0 : 1;

        // 读取SD12紫外线传感器数据
        uint16_t uvValue = SD12_GetADCValue(10); // 采集10次取平均值
        uvLevel = SD12_GetIntensity(uvValue);

        RED = RED_Get(); // 获取红外传感器的值
        humi = (float)DHT11_Data.humi_int + (float)DHT11_Data.humi_deci / 10.0;
        temp = (float)DHT11_Data.temp_int + (float)DHT11_Data.temp_deci / 10.0;
        BT_SendDataPacket(RED, uvLevel, humi, temp); // 发送数据包

        // 检查并处理蓝牙接收数据包
        if (BT_RxFlag == 1) {
            BT_RxFlag = 0; // 清除接收标志

            // 解析数据包并更新蓝牙状态
            bt_status = BT_ParsePacket();
            if (bt_status == 0) {

                // 从flags字节中提取各个标志位
                uint8_t uv_flag    = (BT_Packet.flags >> 0) & 0x01;
                uint8_t servo_flag = (BT_Packet.flags >> 1) & 0x01;
                uint8_t fan_flag   = (BT_Packet.flags >> 2) & 0x01;
                uint8_t motor_flag = (BT_Packet.flags >> 3) & 0x01;
                uint8_t mode_flag  = (BT_Packet.flags >> 4) & 0x01;

                // 根据标志控制设备
                if (uv_flag)
                    LED_Sys_ON();
                else
                    LED_Sys_OFF();
                if (servo_flag)
                    Servo_SetAngle(90);
                else
                    Servo_SetAngle(0);
                if (fan_flag)
                    Fan_ON();
                else
                    Fan_OFF();
                if (motor_flag)
                    Motor_SetSpeed(50);
                else
                    Motor_SetSpeed(0);
            }
        }

        // 更新显示
        OLED_UpdateDisplay(keyValue, dht11_status,
            DHT11_Data.humi_int, DHT11_Data.humi_deci,
            DHT11_Data.temp_int, DHT11_Data.temp_deci,
            uvLevel, RED, bt_status);

        Delay_ms(10); // 延时10ms
    }
}
