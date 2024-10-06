/**
  *****************************************************************************
  * @file    OLED.h
  * @version v1.3.2
  * @author  Bairu
  * @date    2024年10月07日 00:30:51
  * @brief   STM32 OLED屏幕驱动程序头文件，包含参数定义及OLED.c内所有函数声明
  *****************************************************************************
  * @copyright (c) 2024 Bairu. All Rights Reserved.
  *
  * Distributed under MIT license.
  * See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
  *****************************************************************************
  */

#ifndef __OLED_H
#define __OLED_H

#include "stdint.h"


/* 指定OLED驱动芯片型号 --------------------------------------------------------------*/
/**
 * @note 不同型号的芯片对垂直滚动显示功能的支持情况不同。
 *      根据OLED模块实际使用的芯片型号选择一个宏定义即可。
 */
#define OLED_SSD1315
// #define OLED_SSD1306

// 若没有正确指定OLED驱动芯片则触发编译错误
#if (defined(OLED_SSD1315) && defined(OLED_SSD1306)) || (!defined(OLED_SSD1315) && !defined(OLED_SSD1306))
#error "Need to specify a unique OLED driver chip! See OLED.h file."
#endif


/* 参数定义 ----------------------------------------------------------------------*/
// 屏幕测试模式开启/关闭
typedef enum
{
    TEST_ON = 0xA5, // OLED屏幕测试模式开启
    TEST_OFF = 0xA4 // OLED屏幕测试模式关闭
} OLED_TestMode;

// 屏幕显示模式
typedef enum
{
    POSITIVE_MODE = 0xA6, // OLED正显模式
    NEGATIVE_MODE = 0xA7  // OLED反显模式
} OLED_DisplayMode;

// 仅水平滚动模式中的滚动方向
typedef enum
{
    ScrL = 0x27, // 水平向左滚动
    ScrR = 0x26  // 水平向右滚动
} OLED_ScrHorDir;

// 水平+垂直滚动模式中水平滚动开关
typedef enum
{
    ScrH_ON = 0x01, // 水平+垂直滚动模式下启用水平滚动
    ScrH_OFF = 0x00 // 水平+垂直滚动模式下禁用水平滚动
} OLED_ScrVerHorMode;

// 水平+垂直滚动模式中的滚动方向
typedef enum
{
    ScrVL = 0x2A, // 垂直+向左滚动
    ScrVR = 0x29  // 垂直+向右滚动
} OLED_ScrVerHorDir;

// SSD13xx手册中规定的滚动速度指令（间隔多少帧滚动一次）
typedef enum
{
    OLED_ScrSpeed1 = 0x03, // 慢 - 256帧
    OLED_ScrSpeed2 = 0x02, //  | - 128帧
    OLED_ScrSpeed3 = 0x01, //  | - 64帧
    OLED_ScrSpeed4 = 0x06, //  | - 25帧
    OLED_ScrSpeed5 = 0x00, //  | - 5帧
    OLED_ScrSpeed6 = 0x05, //  | - 4帧
    OLED_ScrSpeed7 = 0x04, //  | - 3帧
    OLED_ScrSpeed8 = 0x07  // 快 - 2帧
} OLED_ScrSpeed;


/* 函数声明 ----------------------------------------------------------------------*/
void OLED_WriteCommand(uint8_t Command);           // 向OLED屏发送指令。
void OLED_WriteData(uint8_t Data);                 // 向OLED屏发送数据。
void OLED_SetCursor(uint8_t Line, uint8_t Column); // 设置屏幕显示起始坐标。
void OLED_Display_Off(void);                       // 关闭OLED屏幕显示。
void OLED_Display_On(void);                        // 打开OLED屏幕显示。
void OLED_SetContrast(uint8_t contrast);           // 设置OLED屏幕对比度。
void OLED_SetDisplayMode(OLED_DisplayMode mode);   // 设置OLED屏幕显示模式。
void OLED_Test(OLED_TestMode mode);                // OLED屏幕测试。
void OLED_Clear(void);                             // 清空OLED屏幕显示。
void OLED_ClearLine(uint8_t LineS, uint8_t LineE); // 清空OLED屏幕指定行。

void OLED_Scroll_H(OLED_ScrHorDir ScrLR,
                   uint8_t LineS, uint8_t LineE, OLED_ScrSpeed Speed); // 设置OLED屏幕连续水平滚动。

#ifdef OLED_SSD1315
void OLED_Scroll_VH(uint8_t PixLineS, uint8_t PixLineNum,
                    OLED_ScrVerHorMode Horizontal, OLED_ScrVerHorDir ScrVLR,
                    uint8_t LineS, uint8_t LineE,
                    uint8_t ColumnS, uint8_t ColumnE,
                    uint8_t Offset, OLED_ScrSpeed Speed); // 设置OLED屏幕连续垂直+水平滚动。
#elif defined(OLED_SSD1306)
void OLED_Scroll_VH(OLED_ScrVerHorDir ScrVLR, uint8_t LineS, uint8_t LineE,
                    uint8_t PixLineS, uint8_t PixLineNum,
                    uint8_t Offset, OLED_ScrSpeed Speed); // 设置OLED屏幕连续垂直+水平滚动。
#endif

void OLED_Stop_Scroll(void);  // 停止OLED屏幕连续水平滚动。
void OLED_Start_Scroll(void); // 启用OLED屏幕连续水平滚动。

uint32_t OLED_Pow(uint32_t X, uint32_t Y);

void OLED_ShowChar(uint8_t Line, uint8_t Column,
                   int8_t Char, uint8_t Size); // 在指定位置显示一个字符。

void OLED_ShowString(uint8_t Line, uint8_t Column,
                     char *String, uint8_t Size); // 在指定位置显示一个字符串。

void OLED_ShowNum(uint8_t Line, uint8_t Column,
                  uint32_t Number, uint8_t Length, uint8_t Size); // 在指定位置显示一个无符号数。

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column,
                        int32_t Number, uint8_t Length, uint8_t Size); // 在指定位置显示一个有符号数。

void OLED_ShowFloat(uint8_t Line, uint8_t Column,
                    float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size); // 在指定位置显示一个浮点数。

void OLED_ShowHexNum(uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length, uint8_t Size); // 在指定位置显示一个十六进制数。

void OLED_ShowBinNum(uint8_t Line, uint8_t Column,
                     uint32_t Number, uint8_t Length, uint8_t Size); // 在指定位置显示一个二进制数。

void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num); // 在指定位置显示一个汉字。

void OLED_DrawBMP(uint8_t LineS, uint8_t LineE,
                  uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[]); // 在指定位置显示一个BMP图片。

void OLED_Init(void); // 初始化OLED屏幕。

#endif /* __OLED_H */
