#include "stm32f10x.h"
#include "OLED.h"
#include "PWM.h"
#include "Motor.h"
#include "InfTrack.h"

int main(void)
{
    OLED_Init();
    Motor_PWM_Init();
    InfTracker_Init();
    OLED_ShowString(1, 1, "Test6", 8);

    Right_Wheel_B(50);
    Left_Wheel_F(50);

    while (1)
    {
        OLED_ShowBinNum(3, 1, Get_InfTdata(), 5, 8);
    }
}
