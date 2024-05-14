#include "stm32f10x.h"
#include "OLED.h"
#include "PWM.h"
#include "Car.h"

int main(void)
{
    OLED_Init();
    TIM2_PWM_Init(719, 1999);
    OLED_ShowString(1, 1, "PWM Test4", 8);

    Right_Wheel_B(80);
    Left_Wheel_F(40);

    while (1)
    {
    }
}
