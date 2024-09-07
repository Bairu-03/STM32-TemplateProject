#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#define USART_REC_LEN 200 // 定义最大接收字节数 200

extern uint8_t USART_RX_BUF[USART_REC_LEN];

void UART_init(uint32_t bound);
void UART_SendData(uint16_t data);
uint8_t get_UART_RecStatus(void);
uint16_t get_UART_RecLength(void);
void Reset_UART_RecStatus(void);

#endif

// 串口收发数据模板
// UART_Init(115200);
// if (get_UART_RecStatus())
// {
//     uint8_t t;
//     for (t = 0; t < get_UART_RecLength(); t++)
//     {
//         UART_SendData(USART_RX_BUF[t]);
//     }
//     Reset_UART_RecStatus();
// }
