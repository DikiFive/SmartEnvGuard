/**
 * @file     DK_C8T6.c
 * @brief    STM32F103C8T6主控制程序
 * @details  实现了系统的核心功能：
 *          1. 系统初始化
 *          2. 工作模式管理（手动/自动/蓝牙）
 *          3. 传感器数据采集
 *          4. 按键处理
 *          5. 蓝牙通信
 *          6. OLED显示
 * @author   [作者]
 * @date     [日期]
 * @version  v1.0
 */

#include "DK_C8T6.h"

/**
 * @brief 系统工作模式，默认为手动模式
 */
SystemMode_t currentMode = MODE_MANUAL;

/**
 * @brief 保存上次有效的传感器数据
 * @note  用于传感器读取失败时保持上次的有效数据
 */
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

/**
 * @brief  系统初始化
 * @details 完成所有外设的初始化配置：
 *         - 通信接口（蓝牙、串口）
 *         - 传感器（DHT11、SD12等）
 *         - 执行器（电机、舵机等）
 *         - 人机交互（按键、LED、OLED等）
 *         - 定时器
 * @param  无
 * @return 无
 */
void Sys_Init(void)
{
    BT_Init();      // 初始化蓝牙
    Buzzer_Init();  // 初始化蜂鸣器
    Fan_Init();     // 初始化风扇
    LED_Sys_Init(); // 初始化LED灯
    UV_Init();      // 初始化紫外线灯
    Key_Init();     // 初始化按键
    Motor_Init();   // 初始化电机
    RED_Init();     // 初始化红外传感器
    SD12_Init();    // 初始化SD12紫外线传感器
    DHT11_Init();   // 初始化DHT11传感器
    Servo_Init();   // 初始化舵机
    OLED_Init();    // 初始化OLED屏幕
    Timer_Init();   // 初始化定时器
}

/**
 * @brief  获取所有传感器数据
 * @details 读取并整合所有传感器数据：
 *         1. DHT11温湿度数据
 *         2. SD12紫外线数据
 *         3. 红外传感器数据
 * @note   当DHT11读取失败时，会使用上次的有效数据
 * @return 包含所有传感器数据的结构体
 */
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

/**
 * @brief  处理按键事件
 * @details 按键功能：
 *         1. 模式切换（按键16）
 *            - 手动模式：可直接控制各个设备
 *            - 自动模式：系统自动控制
 *            - 蓝牙模式：通过蓝牙控制
 *         2. 设备控制（在手动模式下）
 *            - 按键1/2：蜂鸣器开关
 *            - 按键3/4：风扇开关
 *            - 按键5/6：紫外线灯开关
 *            - 按键7/8/9：电机控制
 *            - 按键10/11/12：舵机角度控制
 * @param  currentKeyValue 当前按键值
 * @return KeyStatus_t 按键状态：
 *         - keyValue：当前按键值
 *         - changed：按键值是否改变
 */
KeyStatus_t HandleKeyPress(int currentKeyValue)
{
    // 静态变量，保存上一次的按键值
    static int lastKeyValue = 0;
    // 初始化状态结构体，初始时按键值设为上次的按键值，更改标志设为0
    KeyStatus_t status = {lastKeyValue, 0};

    // 当当前按键值不为0时，表示有按键按下
    if (currentKeyValue != 0) {
        // 更新状态结构体的按键值和更改标志
        status.keyValue = currentKeyValue;
        status.changed  = 1;
        lastKeyValue    = currentKeyValue;

        // 处理模式切换（按键16）
        if (currentKeyValue == 16) {
            // 在三种模式间循环切换：手动->自动->蓝牙->手动
            switch (currentMode) {
                case MODE_MANUAL:
                    currentMode = MODE_AUTO;
                    break;
                case MODE_AUTO:
                    // 从自动模式切换出去时，关闭所有设备
                    UV_OFF();
                    Buzzer_OFF();
                    Motor_SetSpeed(0);
                    currentMode = MODE_BT;
                    break;
                case MODE_BT:
                    currentMode = MODE_MANUAL;
                    // 切换到手动模式时，关闭所有设备
                    UV_OFF();
                    Buzzer_OFF();
                    Motor_SetSpeed(0);
                    break;
            }
            return status;
        }

        // 在手动模式下处理其他按键
        if (currentMode == MODE_MANUAL) {
            // 根据不同的按键值执行相应的操作
            switch (currentKeyValue) {
                case 1: // 蜂鸣器 ON
                    Buzzer_ON();
                    break;
                case 2: // 蜂鸣器 OFF
                    Buzzer_OFF();
                    break;
                case 3: // 风扇 ON
                    Fan_ON();
                    break;
                case 4: // 风扇 OFF
                    Fan_OFF();
                    break;
                case 5: // 紫外线灯 ON
                    UV_ON();
                    break;
                case 6: // 紫外线灯 OFF
                    UV_OFF();
                    break;
                case 7: // 电机正转
                    Motor_SetSpeed(20);
                    break;
                case 8: // 电机反转
                    Motor_SetSpeed(-20);
                    break;
                case 9: // 电机停止
                    Motor_SetSpeed(0);
                    break;
                case 10: // 舵机 0°
                    Servo_SetAngle(0);
                    break;
                case 11: // 舵机 90°
                    Servo_SetAngle(90);
                    break;
                case 12: // 舵机 180°
                    Servo_SetAngle(180);
                    break;
            }
        }
    }

    // 返回按键状态
    return status;
}

/**
 * @brief  处理蓝牙通信
 * @details 完成以下功能：
 *         1. 接收并解析蓝牙数据包
 *         2. 根据数据包内容控制设备：
 *            - UV灯控制
 *            - 舵机控制
 *            - 风扇控制
 *            - 电机控制
 * @note   仅在蓝牙模式下处理数据包
 * @return BTStatus_t 蓝牙状态结构体
 */
BTStatus_t HandleBluetooth(void)
{
    BTStatus_t btStatus = {0}; // 初始化为0

    if (BT_RxFlag == 1 && currentMode == MODE_BT) { // 只在蓝牙模式下处理数据包
        BT_RxFlag       = 0;                        // 清除接收标志
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

/**
 * @brief  更新OLED显示内容
 * @details 显示以下信息：
 *         1. 按键值
 *         2. DHT11状态和数据
 *         3. 温湿度值
 *         4. 紫外线等级
 *         5. 红外传感器状态
 *         6. 蓝牙通信状态
 *         7. 当前工作模式
 * @note   使用状态缓存，仅在数据变化时更新显示
 * @param  keyValue    按键值
 * @param  dht11_status DHT11状态
 * @param  humi_int    湿度整数部分
 * @param  humi_deci   湿度小数部分
 * @param  temp_int    温度整数部分
 * @param  temp_deci   温度小数部分
 * @param  uvLevel     紫外线等级
 * @param  redValue    红外传感器值
 * @param  bt_status   蓝牙状态
 * @return 无
 */
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
    if (last_mode != currentMode) {
        OLED_ShowNum(4, 10, currentMode, 1);
        last_mode = currentMode;
    }
}
