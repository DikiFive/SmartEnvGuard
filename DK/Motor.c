/**
 * @file     Motor.c
 * @brief    直流电机驱动程序
 * @details  实现电机的基本控制功能：
 *          - PWM调速
 *          - 正反转控制
 *          - 启停控制
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "PWM.h"       // PWM驱动头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief  直流电机初始化
 * @details 完成以下配置：
 *         1. 使能GPIO时钟
 *         2. 配置方向控制引脚（PA4和PA5）为推挽输出
 *         3. 初始化PWM（用于速度控制）
 * @note   使用TIM3的通道1输出PWM信号
 * @param  无
 * @return 无
 */
void Motor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA4和PA5引脚初始化为推挽输出

    PWM_Init(); // 初始化直流电机的底层PWM
}

/**
 * @brief  设置电机速度
 * @details 通过PWM和方向控制实现速度调节：
 *         - 正值：正转，PWM占空比对应速度值
 *         - 负值：反转，PWM占空比对应速度绝对值
 *         - 零值：停止
 * @param  Speed 设定速度，范围：-100~100
 *         - -100~-1：反转，速度随数值增大而增大
 *         - 0：停止
 *         - 1~100：正转，速度随数值增大而增大
 * @return 无
 */
void Motor_SetSpeed(int8_t Speed)
{
    if (Speed >= 0) // 如果设置正转的速度值
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);   // PA4置高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_5); // PA5置低电平，设置方向为正转
        PWM_SetCompare1(Speed);            // PWM设置为速度值
    } else                                 // 否则，即设置反转的速度值
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4); // PA4置低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_5);   // PA5置高电平，设置方向为反转
        PWM_SetCompare1(-Speed);           // PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
    }
}
