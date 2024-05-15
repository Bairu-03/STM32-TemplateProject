#ifndef __MOTOR_H
#define __MOTOR_H

#define Left_Wheel_F(Duty) TIM2_PWM_Duty(1, (Duty))
#define Left_Wheel_B(Duty) TIM2_PWM_Duty(3, (Duty))
#define Right_Wheel_F(Duty) TIM2_PWM_Duty(2, (Duty))
#define Right_Wheel_B(Duty) TIM2_PWM_Duty(4, (Duty))

#define Motor_Enable() GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define Motor_Disable() GPIO_ResetBits(GPIOA, GPIO_Pin_4)

void Motor_PWM_Init(void);

#endif
