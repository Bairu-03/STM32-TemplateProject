/**
 ******************************************************************************
 * @file    OLED.h
 * @author  Blue_寻
 * @version V1.1.1
 * @date    2024-03-25 00:08:57
 * @brief   OLED屏幕驱动程序头文件，包含数据类型定义、宏定义及OLED.c内所有函数声明
 ******************************************************************************
 */

#ifndef __OLED_H
#define __OLED_H

/*********************数据类型定义***********************/

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed int int32_t;
typedef unsigned int uint32_t;

/*********************外设端口定义************************/

#define APB2_GPIO RCC_APB2Periph_GPIOB
#define GPIOX GPIOB

#define SCL_Pin GPIO_Pin_8 // PB8 -> SCL
#define SDA_Pin GPIO_Pin_9 // PB9 -> SDA

/**********************参数宏定义************************/

#define Line1 0x00
#define Line2 0x01
#define Line3 0x02
#define Line4 0x03
#define Line5 0x04
#define Line6 0x05
#define Line7 0x06
#define Line8 0x07

#define ScrL 0x27
#define ScrR 0x26

/**********************函数声明************************/

void Sim_I2C_Init(void);
void Sim_I2C_Start(void);
void Sim_I2C_Stop(void);
uint8_t I2C_Wait_Ack(void);
void I2C_Send_Ack(uint8_t ack);
uint8_t I2C_Read_Byte(uint8_t ack);
void I2C_Send_Byte(uint8_t Byte);

void OLED_WriteCommand(uint8_t Command);
void OLED_WriteData(uint8_t Data);
void OLED_SetCursor(uint8_t Line, uint8_t Column);
void OLED_Display_Off(void);
void OLED_Display_On(void);
void OLED_Clear(void);
void OLED_Scroll(uint8_t LineS, uint8_t LineE, uint8_t ScrLR, uint8_t Speed);
void OLED_Stop_Scroll(void);
uint32_t OLED_Pow(uint32_t X, uint32_t Y);
void OLED_Init(void);

void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t Size);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num);
void OLED_DrawBMP(uint8_t LineS, uint8_t LineE, uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[]);

#endif
