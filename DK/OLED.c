/**
 * @file     OLED.c
 * @brief    OLED显示屏驱动程序（I2C通信）
 * @details  实现0.96寸OLED显示功能：
 *          - I2C通信协议（软件模拟）
 *          - 显示字符、字符串
 *          - 显示数字（整数、小数）
 *          - 显示清屏等基本功能
 * @note     OLED分辨率：128x64
 *          通信接口：I2C（SCL:PB8, SDA:PB9）
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h"  // STM32F10x外设库头文件
#include "OLED_Font.h"  // OLED字体库
#include "dk_C8T6.h"    // 项目主头文件

/**
 * @brief OLED I2C通信引脚定义和控制宏
 */
#define OLED_W_SCL(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, (BitAction)(x)) /**< SCL引脚控制 */
#define OLED_W_SDA(x) GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(x)) /**< SDA引脚控制 */

/**
 * @brief  OLED I2C通信引脚初始化
 * @details 配置SCL和SDA引脚为开漏输出模式
 * @param  无
 * @return 无
 */
void OLED_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief  I2C起始信号
 * @details 时序：
 *         1. SDA和SCL都为高
 *         2. SDA拉低，产生下降沿
 *         3. SCL拉低，结束起始信号
 * @param  无
 * @return 无
 */
void OLED_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

/**
 * @brief  I2C停止信号
 * @details 时序：
 *         1. SCL为低，SDA为低
 *         2. SCL拉高
 *         3. SDA拉高，产生上升沿
 * @param  无
 * @return 无
 */
void OLED_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief  I2C发送一个字节
 * @details 时序：
 *         1. 循环8次，依次发送每一位
 *         2. 每位数据按MSB先行发送
 *         3. 在SCL高电平期间保持数据稳定
 *         4. 忽略从机应答信号
 * @param  Byte 要发送的字节数据
 * @return 无
 */
void OLED_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        OLED_W_SDA(!!(Byte & (0x80 >> i)));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }
    OLED_W_SCL(1); // 额外的一个时钟，不处理应答信号
    OLED_W_SCL(0);
}

/**
 * @brief  向OLED写入命令
 * @details 发送步骤：
 *         1. 发送起始信号
 *         2. 发送从机地址(0x78)和写命令(0x00)
 *         3. 发送命令数据
 *         4. 发送停止信号
 * @param  Command 要写入的命令字节
 * @return 无
 */
void OLED_WriteCommand(uint8_t Command)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78); // 从机地址
    OLED_I2C_SendByte(0x00); // 写命令
    OLED_I2C_SendByte(Command);
    OLED_I2C_Stop();
}

/**
 * @brief  向OLED写入数据
 * @details 发送步骤：
 *         1. 发送起始信号
 *         2. 发送从机地址(0x78)和写数据(0x40)
 *         3. 发送显示数据
 *         4. 发送停止信号
 * @param  Data 要写入的数据字节
 * @return 无
 */
void OLED_WriteData(uint8_t Data)
{
    OLED_I2C_Start();
    OLED_I2C_SendByte(0x78); // 从机地址
    OLED_I2C_SendByte(0x40); // 写数据
    OLED_I2C_SendByte(Data);
    OLED_I2C_Stop();
}

/**
 * @brief  设置OLED显示光标位置
 * @details 通过发送命令设置页地址和列地址：
 *         - 页地址：0xB0 | Y
 *         - 列地址高4位：0x10 | (X >> 4)
 *         - 列地址低4位：0x00 | (X & 0x0F)
 * @param  Y 页地址，范围0~7，对应8页，每页8行像素
 * @param  X 列地址，范围0~127，对应128列像素
 * @return 无
 */
void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(0xB0 | Y);                 // 设置Y位置
    OLED_WriteCommand(0x10 | ((X & 0xF0) >> 4)); // 设置X位置高4位
    OLED_WriteCommand(0x00 | (X & 0x0F));        // 设置X位置低4位
}

/**
 * @brief  清空OLED显示
 * @details 将所有显存数据清零：
 *         1. 遍历所有8页
 *         2. 每页写入128个0x00
 * @param  无
 * @return 无
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteData(0x00);
        }
    }
}

/**
 * @brief  在指定位置显示一个字符
 * @details 显示过程：
 *         1. 计算显示位置（行和列的实际坐标）
 *         2. 从字库中取出字模数据
 *         3. 分两次写入（上下各8行像素）
 * @param  Line 行号，范围1~4（每行16像素高）
 * @param  Column 列号，范围1~16（每个字符8像素宽）
 * @param  Char 要显示的字符，范围：ASCII可见字符
 * @return 无
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8); // 设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]); // 显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8); // 设置光标位置在下半部分
    for (i = 0; i < 8; i++) {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]); // 显示下半部分内容
    }
}

/**
 * @brief  显示字符串
 * @details 依次显示字符串中的每个字符，直到遇到\0结束
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  String 要显示的字符串，以\0结尾
 * @return 无
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
 * @brief  计算X的Y次方
 * @details 用于进制转换时的数值计算
 * @param  X 底数
 * @param  Y 指数
 * @return uint32_t X^Y的结果
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  显示无符号整数
 * @details 将数字转换为指定长度的字符串显示：
 *         1. 根据指定长度从高位到低位依次转换
 *         2. 不足指定长度时前面补0
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~4294967295
 * @param  Length 指定显示长度，范围1~10
 * @return 无
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  显示有符号整数
 * @details 显示过程：
 *         1. 判断正负，显示符号
 *         2. 将数字转换为无符号数显示
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围-2147483648~2147483647
 * @param  Length 指定显示长度，范围1~10（不包括符号位）
 * @return 无
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  显示十六进制数
 * @details 将数字转换为16进制格式显示：
 *         1. 每4位二进制转换为1位16进制
 *         2. 数字范围0~F用0-9和A-F表示
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~0xFFFFFFFF
 * @param  Length 指定显示长度，范围1~8
 * @return 无
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        } else {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief  显示二进制数
 * @details 将数字的每一位直接转换为字符'0'或'1'显示
 * @param  Line 起始行号，范围1~4
 * @param  Column 起始列号，范围1~16
 * @param  Number 要显示的数字，范围0~65535
 * @param  Length 指定显示长度，范围1~16
 * @return 无
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}

/**
 * @brief  OLED显示屏初始化
 * @details 完成以下配置：
 *         1. 初始化I2C接口
 *         2. 发送显示配置命令序列：
 *            - 关显示
 *            - 设置时钟分频
 *            - 设置多路复用率
 *            - 设置显示偏移
 *            - 设置显示开始行
 *            - 设置充电泵
 *            - 设置内存寻址模式
 *            - 设置列地址范围
 *            - 设置页地址范围
 *            - 设置COM引脚扫描方向
 *            - 设置显示对比度
 *            - 设置预充电周期
 *            - 设置COM引脚硬件配置
 *            - 开显示
 * @param  无
 * @return 无
 */
void OLED_Init(void)
{
    uint32_t i, j;

    for (i = 0; i < 1000; i++) // 上电延时
    {
        for (j = 0; j < 1000; j++);
    }

    OLED_I2C_Init(); // 端口初始化

    OLED_WriteCommand(0xAE); // 关闭显示

    OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // 设置显示开始行

    OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6); // 设置正常/倒转显示

    OLED_WriteCommand(0x8D); // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // OLED清屏
}
