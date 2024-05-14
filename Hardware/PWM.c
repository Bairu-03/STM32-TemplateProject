#include "stm32f10x.h"

uint16_t T2_ARR; // 保存定时器自动装载值，用于TIM2_PWM_Duty函数计算PWM占空比

/**
 * @brief  TIM2定时器PWM初始化，PWM频率 = 72MHz / (psc+1) / (arr+1)
 * @param  psc 目标时钟预分频系数 - 1。
 *     @arg 取值: 0 - 65535
 * @param  arr 目标自动装载值 - 1。
 *     @arg 取值: 0 - 65535
 * @retval 无
 */
void TIM2_PWM_Init(uint16_t psc, uint16_t arr)
{
    T2_ARR = arr;

    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 使能定时器TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    // 使能PWM输出GPIO口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       // TIM2_CH1
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;       // TIM2_CH2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;       // TIM2_CH3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;       // TIM2_CH4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化GPIO

    TIM_TimeBaseStructure.TIM_Period = arr;    // 自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; // 时钟预分频系数
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             // 初始化TIM2

    // 初始化TIM2_CH1的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // 设置PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                            //
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 输出极性为高
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);                      // 初始化TIM4_CH1

    // 初始化TIM2_CH2的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    // 初始化TIM2_CH3的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // 初始化TIM2_CH4的PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    // 使能4个通道的预装载寄存器
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); // OC1
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); // OC2
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); // OC3
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); // OC4
    TIM_ARRPreloadConfig(TIM2, ENABLE);               // 使能重装寄存器

    TIM_Cmd(TIM2, ENABLE); // 使能定时器TIM2，准备工作
}

/**
 * @brief  控制TIM2各通道PWM占空比
 * @param  CHx 选择PWM输出通道。
 *     @arg 取值: 1 - PA0 | 2 - PA1 | 3 - PA2 | 4 - PA3
 * @param  Duty PWM占空比。
 *     @arg 取值: 0 - 100
 * @retval
 */
void TIM2_PWM_Duty(uint8_t CHx, uint8_t Duty)
{
    if (CHx == 1)
        TIM_SetCompare1(TIM2, ((T2_ARR + 1) * Duty) / 100);
    if (CHx == 2)
        TIM_SetCompare2(TIM2, ((T2_ARR + 1) * Duty) / 100);
    if (CHx == 3)
        TIM_SetCompare3(TIM2, ((T2_ARR + 1) * Duty) / 100);
    if (CHx == 4)
        TIM_SetCompare4(TIM2, ((T2_ARR + 1) * Duty) / 100);
}

/*
    TIM2_PWM_Init(719, 1999);  // PWM频率 = 72 000 000 / (719+1) / (1999+1) = 50Hz
    TIM_SetCompare1(TIM2, ((1999+1) * 10) / 100);  // 占空比10%
    TIM_SetCompare2(TIM2, ((1999+1) * 20) / 100); // 占空比20%
    TIM_SetCompare3(TIM2, ((1999+1) * 50) / 100); // 占空比50%
    TIM_SetCompare4(TIM2, ((1999+1) * 80) / 100); // 占空比80%
*/
