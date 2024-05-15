#include "stm32f10x.h"
#include "PWM.h"
#include "Motor.h"

/**
 * CH1 - PA0 - 蓝 - AIN1 - 左轮正转
 * CH2 - PA1 - 绿 - BIN2 - 右轮正转
 * CH3 - PA2 - 黄 - AIN2 - 左轮反转
 * CH4 - PA3 - 棕 - BIN1 - 右轮反转
 */

#define Left_Wheel_F(Duty) TIM2_PWM_Duty(1, (Duty))
#define Left_Wheel_B(Duty) TIM2_PWM_Duty(3, (Duty))
#define Right_Wheel_F(Duty) TIM2_PWM_Duty(2, (Duty))
#define Right_Wheel_B(Duty) TIM2_PWM_Duty(4, (Duty))

#define Motor_Enable() GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define Motor_Disable() GPIO_ResetBits(GPIOA, GPIO_Pin_4)

/**
 * @brief  初始化电机控制（默认禁用电机驱动板），初始化PWM输出
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

    TIM2_PWM_Init(719, 1999);  // PWM频率 = 72 000 000 / (719 + 1) / (1999 + 1) = 50Hz
}

/**
 * @brief  控制小车移动方向及速度
 * @param  CarState 小车电机状态。
 *     @arg 取值: Car_P 停止 | Car_F 前进 | Car_B 后退（当电机状态设为Car_P时将忽略参数Duty_L和Duty_R的值）
 * @param  Duty_L 小车左轮PWM占空比。
 *     @arg 取值: 0 - 100
 * @param  Duty_R 小车右轮PWM占空比。
 *     @arg 取值: 0 - 100
 * @retval 无
 */
void Car_Run(uint8_t CarState, uint8_t Duty_L, uint8_t Duty_R)
{
    if(CarState == Car_F)
    {
        Motor_Enable();
        Left_Wheel_B(0);
        Right_Wheel_B(0);
        Left_Wheel_F(Duty_L);
        Right_Wheel_F(Duty_R);
    }
    if(CarState == Car_B)
    {
        Motor_Enable();
        Left_Wheel_F(0);
        Right_Wheel_F(0);
        Left_Wheel_B(Duty_L);
        Right_Wheel_B(Duty_R);
    }
    if(CarState == Car_P)
    {
        Motor_Disable();
        Left_Wheel_F(0);
        Right_Wheel_F(0);
        Left_Wheel_B(0);
        Right_Wheel_B(0);
    }
}
