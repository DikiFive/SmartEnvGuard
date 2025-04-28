#include "stm32f10x.h" // Device header
#include "dk_C8T6.h"
#include "fan.h"   // 风扇控制
#include "LED.h"   // LED和紫外线灯控制
#include "Servo.h" // 舵机控制

// 系统运行时间计数器（全局可访问）
uint32_t system_runtime_s = 0;

// 温湿度阈值定义
#define TEMP_THRESHOLD 31 // 温度阈值（31°C）
#define HUMI_THRESHOLD 61 // 湿度阈值（61%）

extern SensorData_t last_valid_sensor_data;
extern uint8_t RED_Flag;

uint32_t TimingDelay = 0;

// 定义全局变量供主循环访问
uint32_t ms_count = 0;           // 毫秒计数器，用于更新运行时间
uint32_t uv_timer_ms = 0;        // 红外触发UV灯计时器
uint8_t uv_infrared_active = 0;  // 红外触发UV灯工作标志
uint32_t cycle_timer_ms = 0;     // 循环模式计时器
uint8_t cycle_state = 0;         // 循环模式状态
uint8_t update_flag = 0;         // 定时更新标志，用于主循环中处理

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

        // 更新系统运行时间
        ms_count++;
        if (ms_count >= 1000) {  // 每1000ms（1秒）
            system_runtime_s++;   // 增加一秒
            ms_count = 0;         // 重置毫秒计数器
        }

        /*在此添加其他需要1ms定时执行的任务*/

        // 仅在中断中更新计时器和标志
        if (currentMode == MODE_AUTO) {
            if (uv_infrared_active) {
                uv_timer_ms++;
            }
            update_flag = 1;  // 设置更新标志
        }
        else if (currentMode == MODE_CYCLE) {
            cycle_timer_ms++;
            if (cycle_timer_ms >= 15000) {
                cycle_timer_ms = 0;
            }
            update_flag = 1;  // 设置更新标志
        }
        else {
            uv_timer_ms = 0;
            cycle_timer_ms = 0;
        }

        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
