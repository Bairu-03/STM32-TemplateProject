#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "usart.h"

int main(void)
{
    OLED_Init();
    UART_init(115200);

    OLED_ShowString(1, 1, "UART OLED T", 8);

    while (1)
    {
        // 串口收发测试
        if (get_UART_RecStatus())
        {
            uint8_t t;
            OLED_Clear();
            for (t = 0; t < get_UART_RecLength(); t++)
            {
                UART_SendData(USART_RX_BUF[t]);

                /* 1A 2B 3C 4D 5E */
                /* 1  25 49 73 97 */
                OLED_ShowHexNum(1, (t * 24 + 1), USART_RX_BUF[t], 2,8);
            }
            Reset_UART_RecStatus();
        }
    }
}
