#ifndef __I2C_SOFTWARE_H
#define __I2C_SOFTWARE_H

#include "stdint.h"


#define APB2_GPIO RCC_APB2Periph_GPIOB // APB2外设
#define GPIOX GPIOB                    // GPIOB端口

#define SCL_Pin GPIO_Pin_8 // PB8 -> SCL
#define SDA_Pin GPIO_Pin_9 // PB9 -> SDA

                                                  
#define I2C_ACK 0
#define I2C_NO_ACK 1
#define OLED_R_SDA() GPIO_ReadInputDataBit(GPIOX, SDA_Pin)
#define OLED_W_SCL(x) GPIO_WriteBit(GPIOX, SCL_Pin, (BitAction)(x))
#define OLED_W_SDA(x) GPIO_WriteBit(GPIOX, SDA_Pin, (BitAction)(x))


void Sim_I2C_Init(void);            // 初始化模拟I2C引脚。
void Sim_I2C_Start(void);           // 模拟I2C起始信号。
void Sim_I2C_Stop(void);            // 模拟I2C停止信号。
uint8_t I2C_Wait_Ack(void);         // 等待从机应答信号。
void I2C_Send_Ack(uint8_t ack);     // 发送应答信号。
uint8_t I2C_Read_Byte(uint8_t ack); // I2C读取一个字节。
void I2C_Send_Byte(uint8_t Byte);   // I2C发送一个字节。

#endif /* __I2C_SOFTWARE_H */
