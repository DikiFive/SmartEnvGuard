/**
 * @file     DHT11.c
 * @brief    DHT11温湿度传感器驱动程序
 * @details  实现DHT11的通信协议和数据读取功能。DHT11使用单总线通信，
 *          需要严格的时序控制。完整的通信过程包括：
 *          1. 主机发送起始信号
 *          2. DHT11响应
 *          3. DHT11连续发送40位数据
 *          4. 数据校验
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "dht11.h"

/* 延时函数配置，可根据需要切换不同的延时实现方式 */

#define DHT11_DELAY_US(us) Delay_us(us)
#define DHT11_DELAY_MS(ms) Delay_ms(ms)

/**
 * @brief  DHT11引脚配置函数（内部使用）
 */
static void DHT11_GPIO_Config(void);

/**
 * @brief  将DHT11数据引脚配置为上拉输入模式（内部使用）
 */
static void DHT11_Mode_IPU(void);

/**
 * @brief  将DHT11数据引脚配置为推挽输出模式（内部使用）
 */
static void DHT11_Mode_Out_PP(void);

/**
 * @brief  从DHT11读取一个字节（内部使用）
 * @return 读取到的字节数据
 * @note   按照DHT11的通信协议，每bit数据的时间编码：
 *         - 26-28us的高电平表示数据'0'
 *         - 70us的高电平表示数据'1'
 */
static uint8_t DHT11_ReadByte(void);

/**
 * @brief  DHT11初始化
 * @details 完成以下配置：
 *         1. 配置DHT11的数据引脚
 *         2. 设置引脚为推挽输出模式
 *         3. 发送初始电平
 * @param  无
 * @return 无
 */
void DHT11_Init(void)
{
    DHT11_GPIO_Config();

    DHT11_H; // 拉高GPIOB10
}

/**
 * @brief  配置DHT11用到的GPIO引脚
 * @details 配置过程：
 *         1. 使能GPIO时钟
 *         2. 配置GPIO为推挽输出模式
 *         3. 设置GPIO速度为50MHz
 * @param  无
 * @return 无
 */
static void DHT11_GPIO_Config(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启DHT11_Dout_GPIO_PORT的外设时钟*/
    DHT11_SCK_APBxClock_FUN(DHT11_GPIO_CLK, ENABLE);

    /*选择要控制的DHT11_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  将DHT11数据线配置为上拉输入模式
 * @details 用于接收DHT11发送的数据时，
 *         需要将数据线切换为输入模式
 * @param  无
 * @return 无
 */
static void DHT11_Mode_IPU(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的DHT11_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;

    /*设置引脚模式为浮空输入模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;

    /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  将DHT11数据线配置为推挽输出模式
 * @details 用于主机发送起始信号时，
 *         需要将数据线切换为输出模式
 * @param  无
 * @return 无
 */
static void DHT11_Mode_Out_PP(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*选择要控制的DHT11_Dout_GPIO_PORT引脚*/
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;

    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*调用库函数，初始化DHT11_Dout_GPIO_PORT*/
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  从DHT11读取一个字节
 * @details 按照DHT11的通信协议读取8位数据：
 *         1. 每位数据以50us低电平开始
 *         2. 接着是26-28us或70us的高电平，表示0或1
 *         3. 按照MSB（最高位）先行的顺序接收
 * @return 读取到的字节数据
 */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, temp = 0;

    for (i = 0; i < 8; i++) {
        /*每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/
        while (DHT11_IN() == Bit_RESET);

        /*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
         *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时
         */
        DHT11_DELAY_US(40); // 延时x us 这个延时需要大于数据0持续的时间即可

        if (DHT11_IN() == Bit_SET) /* x us后仍为高电平表示数据“1” */
        {
            /* 等待数据1的高电平结束 */
            while (DHT11_IN() == Bit_SET);

            temp |= (uint8_t)(0x01 << (7 - i)); // 把第7-i位置1，MSB先行
        } else                                  // x us后为低电平表示数据“0”
        {
            temp &= (uint8_t)~(0x01 << (7 - i)); // 把第7-i位置0，MSB先行
        }
    }

    return temp;
}

/**
 * @brief  从DHT11读取温湿度数据
 * @details 完整的数据传输过程：
 *         1. 主机发送起始信号（拉低18ms，再拉高30us）
 *         2. 等待DHT11响应（低80us，高80us）
 *         3. 接收40位数据：
 *            - 8位湿度整数
 *            - 8位湿度小数
 *            - 8位温度整数
 *            - 8位温度小数
 *            - 8位校验和
 * @param  DHT11_Data 存储读取到的温湿度数据
 * @return 操作结果：
 *         - SUCCESS: 读取成功
 *         - ERROR: 读取失败
 */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{
    /*输出模式*/
    DHT11_Mode_Out_PP();
    /*主机拉低*/
    DHT11_L;
    /*延时18ms*/
    DHT11_DELAY_MS(18);

    /*总线拉高 主机延时30us*/
    DHT11_H;

    DHT11_DELAY_US(30); // 延时30us

    /*主机设为输入 判断从机响应信号*/
    DHT11_Mode_IPU();

    /*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/
    if (DHT11_IN() == Bit_RESET) {
        /*轮询直到从机发出 的80us 低电平 响应信号结束*/
        while (DHT11_IN() == Bit_RESET);

        /*轮询直到从机发出的 80us 高电平 标置信号结束*/
        while (DHT11_IN() == Bit_SET);

        /*开始接收数据*/
        DHT11_Data->humi_int = DHT11_ReadByte();

        DHT11_Data->humi_deci = DHT11_ReadByte();

        DHT11_Data->temp_int = DHT11_ReadByte();

        DHT11_Data->temp_deci = DHT11_ReadByte();

        DHT11_Data->check_sum = DHT11_ReadByte();

        /*读取结束，引脚改为输出模式*/
        DHT11_Mode_Out_PP();
        /*主机拉高*/
        DHT11_H;

        /*检查读取的数据是否正确*/
        if (DHT11_Data->check_sum == DHT11_Data->humi_int + DHT11_Data->humi_deci + DHT11_Data->temp_int + DHT11_Data->temp_deci)
            return SUCCESS;
        else
            return ERROR;
    }

    else
        return ERROR;
}
