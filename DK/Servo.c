/**
 * @file     Servo.c
 * @brief    舵机驱动程序
 * @details  实现舵机的角度控制功能：
 *          - 角度范围：0~180度
 *          - PWM周期：20ms
 *          - 脉宽范围：0.5ms~2.5ms
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "PWM.h"       // PWM驱动头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief  舵机初始化
 * @details 配置TIM2_CH2为PWM输出模式：
 *         1. 使能GPIO和定时器时钟
 *         2. 配置PA1为复用推挽输出
 *         3. 配置定时器基本参数：
 *            - 72MHz / 72 = 1MHz 计数频率
 *            - 1MHz / 20000 = 50Hz PWM频率
 *         4. 配置PWM模式和输出极性
 * @param  无
 * @return 无
 */
void Servo_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // 开启TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 将PA1引脚初始化为复用推挽输出
                                           // 受外设控制的引脚，均需要配置为复用模式

    /*配置时钟源*/
    TIM_InternalClockConfig(TIM2); // 选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟

    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;                    // 定义结构体变量
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;       // 时钟分频，选择不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up; // 计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period            = 20000 - 1;          // 计数周期，即ARR的值
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;             // 预分频器，即PSC的值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  // 重复计数器，高级定时器才会用到
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);                   // 配置TIM2的时基单元

    /*输出比较初始化*/
    TIM_OCInitTypeDef TIM_OCInitStructure;                        // 定义结构体变量
    TIM_OCStructInit(&TIM_OCInitStructure);                       // 结构体初始化，给结构体所有成员默认值
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;        // 输出比较模式，选择PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;    // 输出极性，选择为高
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse       = 0;                      // 初始的CCR值
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);                      // 配置TIM2的输出比较通道2

    /*TIM使能*/
    TIM_Cmd(TIM2, ENABLE); // 使能TIM2，定时器开始运行
}

/**
 * @brief  舵机设置角度
 * @details 将角度值转换为对应的PWM占空比：
 *         - 0度对应0.5ms脉宽（CCR=500）
 *         - 180度对应2.5ms脉宽（CCR=2500）
 * @param  Angle 要设置的角度，范围：0~180度
 * @return 无
 */
void Servo_SetAngle(float Angle)
{
    // 角度线性映射到脉宽：0~180° -> 500~2500
    TIM_SetCompare2(TIM2, Angle / 180 * 2000 + 500);
}
