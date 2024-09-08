/**
 * @file    OLED.h
 * @version v1.3.1
 * @author  Bairu
 * @date    2024年8月17日 22:17:45
 * @brief   STM32 OLED屏幕驱动程序头文件，包含参数宏定义及OLED.c内所有函数声明
 */

#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"

/**
 * @note  指定OLED屏幕的驱动芯片，不同芯片的垂直滚动显示相关指令不同。
 */
#define OLED_SSD1315
// #define OLED_SSD1306

#if (defined(OLED_SSD1315) && defined(OLED_SSD1306)) || (!defined(OLED_SSD1315) && !defined(OLED_SSD1306))
#error "Need to specify a unique OLED driver chip!"
#endif

/**********************参数宏定义************************/

#define ScrL 0x27
#define ScrR 0x26
#define ScrVL 0x2A
#define ScrVR 0x29
#define ScrH_OFF 0x00
#define ScrH_ON 0x01

#define POSITIVE_MODE 0xA6
#define NEGATIVE_MODE 0xA7

#define T_ON 0xA5
#define T_OFF 0xA4

/*******************************************************/

/************************函数声明************************/

void OLED_WriteCommand(uint8_t Command);                                        // 向OLED屏发送指令。
void OLED_WriteData(uint8_t Data);                                              // 向OLED屏发送数据。
void OLED_SetCursor(uint8_t Line, uint8_t Column);                              // 设置屏幕显示起始坐标。
void OLED_Display_Off(void);                                                    // 关闭OLED屏幕显示。
void OLED_Display_On(void);                                                     // 打开OLED屏幕显示。
void OLED_SetContrast(uint8_t contrast);                                        // 设置OLED屏幕对比度。
void OLED_SetDisplayMode(uint8_t mode);                                         // 设置OLED屏幕显示模式。
void OLED_Test(uint8_t cmd);                                                    // OLED屏幕测试。
void OLED_Clear(void);                                                          // 清空OLED屏幕显示。
void OLED_ClearLine(uint8_t LineS, uint8_t LineE);                              // 清空OLED屏幕指定行。
void OLED_Scroll_H(uint8_t ScrLR, uint8_t LineS, uint8_t LineE, uint8_t Level); // 设置OLED屏幕连续水平滚动。

#ifdef OLED_SSD1315
void OLED_Scroll_VH(uint8_t PixLineS, uint8_t PixLineNum,
                    uint8_t Horizontal, uint8_t ScrVLR,
                    uint8_t LineS, uint8_t LineE,
                    uint8_t ColumnS, uint8_t ColumnE,
                    uint8_t Offset, uint8_t Level);                               // 设置OLED屏幕连续垂直+水平滚动。
#elif defined(OLED_SSD1306)
void OLED_Scroll_VH(uint8_t ScrVLR, uint8_t LineS, uint8_t LineE,
                    uint8_t PixLineS, uint8_t PixLineNum,
                    uint8_t Offset, uint8_t Level); // 设置OLED屏幕连续垂直+水平滚动。
#endif

void OLED_Stop_Scroll(void);  // 停止OLED屏幕连续水平滚动。
void OLED_Start_Scroll(void); // 启用OLED屏幕连续水平滚动。
uint32_t OLED_Pow(uint32_t X, uint32_t Y);

void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size);                                // 在指定位置显示一个字符。
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t Size);                             // 在指定位置显示一个字符串。
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);             // 在指定位置显示一个无符号数。
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint8_t Size);        // 在指定位置显示一个有符号数。
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size); // 在指定位置显示一个浮点数。
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);          // 在指定位置显示一个十六进制数。
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size);          // 在指定位置显示一个二进制数。
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num);                                                // 在指定位置显示一个汉字。
void OLED_DrawBMP(uint8_t LineS, uint8_t LineE, uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[]);           // 在指定位置显示一个BMP图片。

void OLED_Init(void); // 初始化OLED屏幕。

/*******************************************************/

#endif
