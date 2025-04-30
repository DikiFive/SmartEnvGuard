/**
 * @file     Timer.c
 * @brief    定时器驱动程序
 * @details  实现系统定时功能：
 *          - 1ms基本定时
 *          - 系统运行时间计数
 *          - 软件延时功能
 *          - 各种模式的定时控制
 * @author   DikiFive
 * @date     2025-04-30
 * @version  v1.0
 */

#include "stm32f10x.h" // STM32F10x外设库头文件
#include "dk_C8T6.h"   // 项目主头文件

/**
 * @brief 系统定时相关变量
 */
uint32_t TimingDelay    = 0;  /**< 软件延时计数器 */
uint32_t system_runtime_s = 0; /**< 系统运行时间（秒） */
uint32_t ms_count       = 0;  /**< 毫秒计数器 */
uint32_t uv_timer_ms    = 0;  /**< 红外触发UV灯计时器 */
uint8_t uv_infrared_active = 0; /**< 红外触发UV灯工作标志 */
uint32_t cycle_timer_ms = 0;  /**< 循环模式计时器 */
uint8_t cycle_state    = 0;   /**< 循环模式状态 */
uint8_t update_flag    = 0;   /**< 定时更新标志 */

/**
 * @brief  定时器初始化
 * @details 配置TIM4为1ms定时中断：
 *         1. 使能定时器时钟
 *         2. 配置定时器基本参数：
 *            - 72MHz / 72 = 1MHz 计数频率
 *            - 1MHz / 1000 = 1KHz 中断频率
 *         3. 配置NVIC中断优先级
 * @param  无
 * @return 无
 */
void Timer_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // 开启TIM4的时钟

    /*配置时钟源*/
    TIM_InternalClockConfig(TIM4); // 选择内部时钟

    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;       // 不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInitStructure.TIM_Period            = 1000 - 1;           // ARR值，1ms
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;             // PSC值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                  // 不重复计数
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    /*中断输出配置*/
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);                  // 清除更新中断标志位
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);             // 使能更新中断

    /*NVIC中断优先级配置*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;
    NVIC_Init(&NVIC_InitStructure);

    /*使能定时器*/
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @brief  定时器中断服务函数
 * @details 每1ms触发一次中断：
 *         1. 更新系统运行时间
 *         2. 处理软件延时计数
 *         3. 更新各种定时器计数值
 *         4. 设置定时更新标志
 * @note   此函数会被硬件自动调用
 */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update); // 清除中断标志位

        // 更新系统运行时间
        ms_count++;
        if (ms_count >= 1000) {
            ms_count = 0;
            system_runtime_s++;
        }

        // 软件延时更新
        if (TimingDelay > 0) {
            TimingDelay--;
        }

        // 更新UV灯定时器
        if (uv_infrared_active) {
            uv_timer_ms++;
        }

        // 更新循环模式定时器
        if (cycle_state) {
            cycle_timer_ms++;
            if (cycle_timer_ms >= 10000) {
                cycle_timer_ms = 0;
            }
        }

        // 设置定时更新标志
        update_flag = 1;
    }
}

/**
 * @brief  设置软件延时计数值
 * @details 用于非阻塞延时
 * @param  nTime 延时时间（毫秒）
 * @return 无
 */
void TimingDelay_Set(uint32_t nTime)
{
    TimingDelay = nTime;
}

/**
 * @brief  获取软件延时计数值
 * @return uint32_t 当前延时计数值
 */
uint32_t TimingDelay_Get(void)
{
    return TimingDelay;
}

/**
 * @brief  等待延时结束
 * @details 阻塞等待直到延时计数为0
 * @return 无
 */
void TimingDelay_WaitForEnd(void)
{
    while (TimingDelay != 0);
}
