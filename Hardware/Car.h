#ifndef __CAR_H
#define __CAR_H

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

#endif
