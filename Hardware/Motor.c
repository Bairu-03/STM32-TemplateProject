#include "stm32f10x.h"
#include "PWM.h"

/**
 * CH1 - PA0 - 蓝 - AIN1 - 左轮正转
 * CH2 - PA1 - 绿 - BIN2 - 右轮正转
 * CH3 - PA2 - 黄 - AIN2 - 左轮反转 
 * CH4 - PA3 - 棕 - BIN1 - 右轮反转
 */

/**
 * @brief  初始化电机控制（默认禁用驱动板），初始化PWM输出
 * @param  无
 * @retval 无
 */
void Motor_PWM_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, GPIO_Pin_4);

    TIM2_PWM_Init(719, 1999);
}
