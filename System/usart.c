#include "usart.h"

/** 接收状态标志
 * bit15，接收完成标志（0x0a）
 * bit14，接收到0x0d
 * bit13~0，接收到的有效字节数
 */
uint16_t USART_RX_STA = 0;

/**
 * 接收缓冲数组，最大USART_REC_LEN个字节。
 * 在取完串口数据后，需要用 USART_RX_STA = 0 初始化串口接收标志
 */
uint8_t USART_RX_BUF[USART_REC_LEN];

void uart_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); // 使能USART1，GPIOA时钟

    // USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART1_RX   PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;        // 响应优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_InitStructure.USART_BaudRate = bound;                                     // 串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 收发模式

    USART_Init(USART1, &USART_InitStructure);      // 初始化串口1
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启串口接受中断
    USART_Cmd(USART1, ENABLE);                     // 使能串口1
}

void USART1_IRQHandler(void)
{
    uint8_t Res;
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART1); // 读取接收到的数据

        if ((USART_RX_STA & 0x8000) == 0) // 接收未完成
        {
            if (USART_RX_STA & 0x4000) // 接收到了0x0d
            {
                if (Res != 0x0a)
                    USART_RX_STA = 0; // 接收错误,重新开始
                else
                    USART_RX_STA |= 0x8000; // 接收完成
            }
            else // 还没收到0X0d
            {
                if (Res == 0x0d)
                    USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))
                        USART_RX_STA = 0; // 接收数据错误,重新开始接收
                }
            }
        }
    }
}

        // 串口收发数据模板1
        // if (USART_RX_STA & 0x8000)
        // {
        //     uint8_t len, t;
        //     len = USART_RX_STA & 0x3fff;
        //     for (t = 0; t < len; t++)
        //     {
        //         USART_SendData(USART1, USART_RX_BUF[t]);
        //         while (USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
        //     }

        //     USART_RX_STA = 0;
        // }

        // 串口收发数据模板2
        // if (USART_RX_STA & 0x8000)
        // {
        //     OLED_ShowHexNum(3, 1, USART_RX_BUF[0], 2, 8);
        //     OLED_ShowHexNum(3, 17, USART_RX_BUF[1], 2, 8);
        //     OLED_ShowHexNum(3, 33, USART_RX_BUF[2], 2, 8);
        //     USART_RX_STA = 0;
        // }
