/**
 * @file     LED.c
 * @brief    LED驱动程序
 * @details  实现三组LED的控制功能：
 *          1. LED1/LED2（PA1/PA2）：通用指示灯
 *          2. 系统LED（PC13）：系统状态指示
 *          3. 紫外线灯（PA12）：杀菌消毒功能
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief  LED1和LED2初始化
 * @details 配置PA1和PA2为推挽输出，并设置默认状态为关闭
 * @param  无
 * @return 无
 */
void LED_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA1和PA2引脚初始化为推挽输出

    /*设置GPIO初始化后的默认电平*/
    GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_2); // 设置PA1和PA2引脚为高电平
}

/**
 * @brief  系统LED初始化（PC13）
 * @details 配置PC13为推挽输出，并设置默认状态为关闭
 * @param  无
 * @return 无
 */
void LED_Sys_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 开启GPIOC的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure); // 将PC13引脚初始化为推挽输出

    /*设置GPIO初始化后的默认电平*/
    GPIO_SetBits(GPIOC, GPIO_Pin_13); // 设置PC13引脚为高电平
}

/**
 * @brief  打开系统LED
 * @details 将PC13设置为低电平，点亮LED
 * @param  无
 * @return 无
 */
void LED_Sys_ON(void)
{
    GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 设置PC13引脚为低电平
}

/**
 * @brief  关闭系统LED
 * @details 将PC13设置为高电平，熄灭LED
 * @param  无
 * @return 无
 */
void LED_Sys_OFF(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13); // 设置PC13引脚为高电平
}

/**
 * @brief  翻转系统LED状态
 * @details 如果LED当前是亮的则熄灭，如果是熄灭的则点亮
 * @param  无
 * @return 无
 */
void LED_Sys_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == 0) // 获取输出寄存器的状态，如果当前引脚输出低电平
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13); // 则设置PC13引脚为高电平
    } else                                // 否则，即当前引脚输出高电平
    {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 则设置PC13引脚为低电平
    }
}

/**
 * @brief  打开LED1
 * @details 将PA1设置为低电平，点亮LED1
 * @param  无
 * @return 无
 */
void LED1_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 设置PA1引脚为低电平
}

/**
 * @brief  关闭LED1
 * @details 将PA1设置为高电平，熄灭LED1
 * @param  无
 * @return 无
 */
void LED1_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1); // 设置PA1引脚为高电平
}

/**
 * @brief  翻转LED1状态
 * @details 如果LED1当前是亮的则熄灭，如果是熄灭的则点亮
 * @param  无
 * @return 无
 */
void LED1_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) // 获取输出寄存器的状态，如果当前引脚输出低电平
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_1); // 则设置PA1引脚为高电平
    } else                               // 否则，即当前引脚输出高电平
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1); // 则设置PA1引脚为低电平
    }
}

/**
 * @brief  打开LED2
 * @details 将PA2设置为低电平，点亮LED2
 * @param  无
 * @return 无
 */
void LED2_ON(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_2); // 设置PA2引脚为低电平
}

/**
 * @brief  关闭LED2
 * @details 将PA2设置为高电平，熄灭LED2
 * @param  无
 * @return 无
 */
void LED2_OFF(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_2); // 设置PA2引脚为高电平
}

/**
 * @brief  翻转LED2状态
 * @details 如果LED2当前是亮的则熄灭，如果是熄灭的则点亮
 * @param  无
 * @return 无
 */
void LED2_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_2) == 0) // 获取输出寄存器的状态，如果当前引脚输出低电平
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_2); // 则设置PA2引脚为高电平
    } else                               // 否则，即当前引脚输出高电平
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_2); // 则设置PA2引脚为低电平
    }
}

/**
 * @brief  紫外线灯初始化
 * @details 配置PA12为推挽输出，并设置默认状态为关闭
 * @note   紫外线灯用于杀菌消毒，使用时需注意安全
 * @param  无
 * @return 无
 */
void UV_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA12引脚初始化为推挽输出

    /*设置GPIO初始化后的默认电平*/
    GPIO_ResetBits(GPIOA, GPIO_Pin_12); // 设置PA12引脚为低电平（熄灭）
}

/**
 * @brief  打开紫外线灯
 * @details 将PA12设置为高电平，点亮紫外线灯
 * @param  无
 * @return 无
 */
void UV_ON(void)
{
    GPIO_SetBits(GPIOA, GPIO_Pin_12); // 设置PA12引脚为高电平（点亮）
}

/**
 * @brief  关闭紫外线灯
 * @details 将PA12设置为低电平，关闭紫外线灯
 * @param  无
 * @return 无
 */
void UV_OFF(void)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_12); // 设置PA12引脚为低电平（熄灭）
}

/**
 * @brief  翻转紫外线灯状态
 * @details 如果紫外线灯当前是开启的则关闭，如果是关闭的则开启
 * @param  无
 * @return 无
 */
void UV_Turn(void)
{
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12) == 0) // 获取输出寄存器的状态，如果当前引脚输出低电平
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_12); // 则设置PA12引脚为高电平
    } else                                // 否则，即当前引脚输出高电平
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_12); // 则设置PA12引脚为低电平
    }
}
