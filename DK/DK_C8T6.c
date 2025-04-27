#include "DK_C8T6.h"

// 静态变量用于保存上次有效的传感器数据
static SensorData_t last_valid_sensor_data = {0};

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

void Sys_Init(void)
{
    BT_Init();      // 初始化蓝牙
    Buzzer_Init();  // 初始化蜂鸣器
    Fan_Init();     // 初始化风扇
    LED_Sys_Init(); // 初始化LED灯
    Key_Init();     // 初始化按键
    Motor_Init();   // 初始化电机
    RED_Init();     // 初始化红外传感器
    SD12_Init();    // 初始化SD12紫外线传感器
    DHT11_Init();   // 初始化DHT11传感器
    Servo_Init();   // 初始化舵机
    OLED_Init();    // 初始化OLED屏幕
}

// 获取所有传感器数据
SensorData_t GetAllSensorData(void)
{
    SensorData_t data;
    DHT11_Data_TypeDef DHT11_Data;

    // 获取DHT11数据
    if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS) {
        data.dht11_status = 0;
        data.humi_int     = DHT11_Data.humi_int;
        data.humi_deci    = DHT11_Data.humi_deci;
        data.temp_int     = DHT11_Data.temp_int;
        data.temp_deci    = DHT11_Data.temp_deci;

        // 保存有效数据
        last_valid_sensor_data.humi_int  = data.humi_int;
        last_valid_sensor_data.humi_deci = data.humi_deci;
        last_valid_sensor_data.temp_int  = data.temp_int;
        last_valid_sensor_data.temp_deci = data.temp_deci;
    } else {
        data.dht11_status = 1;
        // 读取失败时使用上次的有效数据
        data.humi_int  = last_valid_sensor_data.humi_int;
        data.humi_deci = last_valid_sensor_data.humi_deci;
        data.temp_int  = last_valid_sensor_data.temp_int;
        data.temp_deci = last_valid_sensor_data.temp_deci;
    }

    // 获取SD12紫外线数据
    uint16_t uvValue = SD12_GetADCValue(10);
    data.uvLevel     = SD12_GetIntensity(uvValue);

    // 获取红外数据
    data.redValue = RED_Get();

    return data;
}

// 处理按键输入
KeyStatus_t HandleKeyPress(int currentKeyValue)
{
    static int lastKeyValue = 0;
    KeyStatus_t status      = {lastKeyValue, 0};

    if (currentKeyValue != 0) {
        status.keyValue = currentKeyValue;
        status.changed  = 1;
        lastKeyValue    = currentKeyValue;

        // 根据按键值控制设备
        switch (currentKeyValue) {
            case 1:
                Buzzer_ON();
                Fan_ON();
                break;
            case 2:
                Buzzer_OFF();
                Fan_OFF();
                break;
            case 9:
                LED_Sys_Turn();
                Motor_SetSpeed(20);
                Servo_SetAngle(90);
                break;
            case 10:
                LED_Sys_Turn();
                Motor_SetSpeed(-20);
                Servo_SetAngle(0);
                break;
            case 11:
                LED_Sys_Turn();
                Motor_SetSpeed(0);
                Servo_SetAngle(180);
                break;
            case 12:
                LED_Sys_Turn();
                Motor_SetSpeed(10
                );
                Servo_SetAngle(45);
                break;
        }
    }

    return status;
}

// 处理蓝牙数据包
BTStatus_t HandleBluetooth(void)
{
    BTStatus_t btStatus = {0}; // 初始化为0

    if (BT_RxFlag == 1) {
        BT_RxFlag       = 0; // 清除接收标志
        btStatus.status = BT_ParsePacket();

        if (btStatus.status == 0) {
            // 从flags字节中提取各个标志位
            btStatus.uv_flag    = (BT_Packet.flags >> 0) & 0x01;
            btStatus.servo_flag = (BT_Packet.flags >> 1) & 0x01;
            btStatus.fan_flag   = (BT_Packet.flags >> 2) & 0x01;
            btStatus.motor_flag = (BT_Packet.flags >> 3) & 0x01;
            btStatus.mode_flag  = (BT_Packet.flags >> 4) & 0x01;

            // 根据标志控制设备
            if (btStatus.uv_flag)
                LED_Sys_ON();
            else
                LED_Sys_OFF();

            if (btStatus.servo_flag)
                Servo_SetAngle(90);
            else
                Servo_SetAngle(0);

            if (btStatus.fan_flag)
                Fan_ON();
            else
                Fan_OFF();

            if (btStatus.motor_flag)
                Motor_SetSpeed(50);
            else
                Motor_SetSpeed(0);
        }
    }

    return btStatus;
}

// OLED显示更新
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
    float humi                   = humi_int + humi_deci / 10.0f;
    int humi_display             = (int)(humi * 10); // 保留一位小数
    static int last_humi_display = -1;

    if (last_humi_display != humi_display) {
        OLED_ShowNum(2, 4, humi_display / 10, 2); // 整数部分
        OLED_ShowString(2, 6, ".");               // 小数点
        OLED_ShowNum(2, 7, humi_display % 10, 1); // 小数部分
        OLED_ShowString(2, 8, "%");               // 百分号
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
        OLED_ShowNum(4, 3, redValue, 1); // 只显示0或1
        last_display.redValue = redValue;
    }

    // 更新蓝牙状态（如果改变）
    static uint8_t last_bt_rx = 0;
    if (BT_RxFlag == 1 && !last_bt_rx) {
        OLED_ShowString(4, 6, "R"); // 显示接收标志
    } else if (BT_RxFlag == 0 && last_bt_rx) {
        OLED_ShowString(4, 6, " "); // 清除接收标志
    }
    last_bt_rx = BT_RxFlag;

    // 更新Mode标志（如果改变）
    static uint8_t last_mode = 0xFF;
    uint8_t current_mode     = (BT_Packet.flags >> 4) & 0x01;
    if (last_mode != current_mode) {
        OLED_ShowNum(4, 10, current_mode, 1);
        last_mode = current_mode;
    }
}
