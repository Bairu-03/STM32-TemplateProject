#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "Motor.h"
#include "InfTrack.h"
#include "usart.h"

int main(void)
{
    OLED_Init();
    Motor_PWM_Init();
    InfTracker_Init();
    uart_init(115200);
    OLED_ShowString(1, 1, "UART Track Test", 8);

    uint8_t def = 0;
    while (1)
    {
        if (USART_RX_STA & 0x8000)
        {
            def = USART_RX_BUF[0];
            OLED_ShowNum(3, 1, def, 2, 8);
            USART_RX_STA = 0;
        }
        if (def == 50) // 101 直行前进
            Car_Run(Car_F, 30, 30);
        else if ((def > 50) && (def < 70)) // 011 左转
            Car_Run(Car_F, 0, 30);
        else if ((def < 50) && (def > 30)) // 110 右转
            Car_Run(Car_F, 30, 0);
        else
            Car_Run(Car_P, 0, 0);
    }
}
