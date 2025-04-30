/**
 * @file     PWM.c
 * @brief    PWM驱动程序
 * @details  使用TIM3和TIM4实现PWM输出：
 *          - TIM3_CH1：用于直流电机速度控制
 *          - TIM4_CH3：用于舵机角度控制
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief  PWM初始化（用于直流电机）
 * @details 配置TIM3_CH1为PWM输出模式：
 *         1. 使能GPIO和定时器时钟
 *         2. 配置GPIO为复用推挽输出
 *         3. 配置定时器基本参数：
 *            - 72MHz / 36 = 2MHz 计数频率
 *            - 2MHz / 100 = 20KHz PWM频率
 *         4. 配置PWM模式和输出极性
 * @param  无
 * @return 无
 */
void PWM_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  // 开启TIM3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;         // PA6（TIM3_CH1）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /*配置时钟源*/
    TIM_InternalClockConfig(TIM3); // 使用内部时钟源

    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;       // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period            = 100 - 1;            // ARR值（周期）
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 36 - 1;             // PSC值（预分频）
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  // 重复计数器值
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    /*输出比较初始化*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);                      // 结构体初始化
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;      // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;  // 输出极性为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;// 使能输出
    TIM_OCInitStructure.TIM_Pulse       = 0;                     // 初始占空比为0
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);

    /*使能定时器*/
    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief  设置TIM3通道1的PWM占空比（电机控制）
 * @param  Compare 占空比值，范围：0~100
 *         - 0: 输出一直为低电平
 *         - 100: 输出一直为高电平
 * @return 无
 */
void PWM_SetCompare1(uint16_t Compare)
{
    TIM_SetCompare1(TIM3, Compare);
}

/**
 * @brief  设置TIM3通道2的PWM占空比
 * @param  Compare 占空比值，范围：0~100
 *         - 0: 输出一直为低电平
 *         - 100: 输出一直为高电平
 * @return 无
 */
void PWM_SetCompare2(uint16_t Compare)
{
    TIM_SetCompare2(TIM3, Compare);
}

/**
 * @brief  设置TIM3通道3的PWM占空比
 * @param  Compare 占空比值，范围：0~100
 *         - 0: 输出一直为低电平
 *         - 100: 输出一直为高电平
 * @return 无
 */
void PWM_SetCompare3(uint16_t Compare)
{
    TIM_SetCompare3(TIM3, Compare);
}
