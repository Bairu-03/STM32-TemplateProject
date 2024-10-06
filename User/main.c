#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
#include "USART.h"

int main(void)
{
    OLED_Init();
    UART_init(115200);

    OLED_ShowString(1, 1, " UART OLED TEST ", 8);
    OLED_ShowString(3, 1, "================", 8);

    Delay_ms(1000);

    /* 设置OLED反显 */
    OLED_SetDisplayMode(NEGATIVE_MODE);

    /* 绘制进度条框 */
    OLED_SetCursor(4, 8);
    OLED_WriteData(0xFF);
    OLED_SetCursor(5, 8);
    OLED_WriteData(0xFF);
    OLED_SetCursor(4, 120);
    OLED_WriteData(0xFF);
    OLED_SetCursor(5, 120);
    OLED_WriteData(0xFF);
    for(int j = 0; j < 111; j++)
    {
        OLED_SetCursor(4, 9 + j);
        OLED_WriteData(0x01);
        OLED_SetCursor(5, 9 + j);
        OLED_WriteData(0x80);
    }

    Delay_ms(500);

    /* 模拟进度条加载 */
    int i;
    for (i = 2; i < 111; i++)
    {
        OLED_SetCursor(4, 8 + i);
        OLED_WriteData(0xFD);
        OLED_SetCursor(5, 8 + i);
        OLED_WriteData(0xBF);
        Delay_ms(i - i / 2 + 1);
    }
    OLED_ShowString(5, 1, "     FINISH     ", 8);
    Delay_ms(1000);
    OLED_ClearLine(5, 6);

    /* 第一行文字右上滚动显示 */
    OLED_Scroll_VH(1, 16, ScrH_ON, ScrVR, 1, 2, 1, 128, 1, OLED_ScrSpeed5);

    while (1)
    {
        // 串口收发测试
        if (get_UART_RecStatus())
        {
            uint8_t t;
            OLED_ClearLine(5, 6);
            for (t = 0; t < get_UART_RecLength(); t++)
            {
                UART_SendData(USART_RX_BUF[t]);
                OLED_ShowChar(5, (t * 8 + 1), USART_RX_BUF[t], 8);
            }
            Reset_UART_RecStatus();
        }
    }
}
