#ifndef __MOTOR_H
#define __MOTOR_H

#define Car_P ((uint8_t)0)
#define Car_F ((uint8_t)1)
#define Car_B ((uint8_t)2)

void Motor_PWM_Init(void);
void Car_Run(uint8_t CarState, uint8_t Duty_L, uint8_t Duty_R);

#endif
