#include "stm32f10x.h" // Device header
#include "dk_C8T6.h"
#include "fan.h" // Include fan header
#include "LED.h" // Assuming UV_ON/OFF is related to LED control, or include UV header if separate

uint32_t TimingDelay = 0;

// 自动模式定时器相关变量
static uint32_t auto_mode_timer_ms = 0;
static uint8_t auto_mode_state     = 0; // 0: OFF, 1: ON

void Timer_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode       = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period            = 1000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler         = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);

    /*使能定时器更新中断*/
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_Init(&NVIC_InitStructure);

    /*使能定时器*/
    TIM_Cmd(TIM4, ENABLE);
}

void Timer_SetDelay(uint32_t nTime)
{
    TimingDelay = nTime;
}

uint32_t Timer_GetDelay(void)
{
    return TimingDelay;
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET) {
        /*延时计数器处理*/
        if (TimingDelay > 0) {
            TimingDelay--;
        }

        /*在此添加其他需要1ms定时执行的任务*/

        // 自动模式控制逻辑
        if (currentMode == MODE_AUTO) {
            auto_mode_timer_ms++; // 1ms计数

            if (auto_mode_timer_ms >= 15000) { // 15秒周期 (5秒工作 + 10秒间隔)
                auto_mode_timer_ms = 0;
            }

            if (auto_mode_timer_ms < 5000) { // 前5秒：开启风扇和紫外线灯
                if (auto_mode_state == 0) {
                    Fan_ON();
                    UV_ON(); // Assuming UV_ON is defined elsewhere, possibly in LED.h or a separate UV.h
                    auto_mode_state = 1;
                }
            } else { // 后10秒：关闭风扇和紫外线灯
                if (auto_mode_state == 1) {
                    Fan_OFF();
                    UV_OFF(); // Assuming UV_OFF is defined elsewhere
                    auto_mode_state = 0;
                }
            }
        } else {
            // 如果不在自动模式，重置自动模式定时器和状态
            auto_mode_timer_ms = 0;
            auto_mode_state    = 0;
            // 退出自动模式时，DK_C8T6.c中的HandleKeyPress会关闭设备，这里不需要重复关闭
        }

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
