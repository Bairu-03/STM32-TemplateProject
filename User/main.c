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
    OLED_ShowString(1, 1, "Track Test", 8);

    while (1)
    {
        uint8_t InfTD;
        OLED_ShowBinNum(3, 1, Get_InfTdata(), 5, 8);
        InfTD = Get_InfTdata() & 0x0E;  // 保留红外寻迹数据中间三位
        if(InfTD == 0x0A)  // 101 直行前进
            Car_Run(Car_F, 30, 30);
        if(InfTD == 0x06)  // 011 左转
            Car_Run(Car_F, 0, 30);
        if(InfTD == 0x0C)  // 110 右转
            Car_Run(Car_F, 30, 0);
        if(InfTD == 0x08)  // 100 大右转
            Car_Run(Car_F, 50, 0);
        if(InfTD == 0x02)  // 001 大左转
            Car_Run(Car_F, 0, 50);
        if(InfTD == 0x00)  // 000 停
            Car_Run(Car_P, 0, 0);
    }
}
