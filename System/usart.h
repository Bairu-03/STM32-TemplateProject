#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"

#define USART_REC_LEN 200 // 定义最大接收字节数 200

extern uint8_t USART_RX_BUF[USART_REC_LEN]; // 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern uint16_t USART_RX_STA;               // 接收状态标记

void uart_init(uint32_t bound);

#endif
