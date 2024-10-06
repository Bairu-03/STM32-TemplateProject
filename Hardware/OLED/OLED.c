/**
  *****************************************************************************
  * @file    OLED.c
  * @version v1.3.2
  * @author  Bairu
  * @date    2024年10月07日 00:30:17
  * @brief   STM32 OLED屏幕驱动程序(SSD1306或SSD1315, I2C)
  *****************************************************************************
  * @copyright (c) 2024 Bairu. All Rights Reserved.
  *
  * Distributed under MIT license.
  * See file LICENSE for detail or copy at https://opensource.org/licenses/MIT
  *****************************************************************************
  */

#include "stm32f10x.h"
#include "OLED.h"
#include "OLED_Font.h"
#include "I2C_Software.h"

/**
 * @brief  向OLED屏发送指令。
 * @param  Command 要写入的指令。
 * @retval 无
 */
void OLED_WriteCommand(uint8_t Command)
{
    Sim_I2C_Start();
    I2C_Send_Byte(0x78); // 从机地址
    I2C_Send_Byte(0x00); // 写命令
    I2C_Send_Byte(Command);
    Sim_I2C_Stop();
}

/**
 * @brief  向OLED屏发送数据。
 * @param  Data 要写入的数据。
 * @retval 无
 */
void OLED_WriteData(uint8_t Data)
{
    Sim_I2C_Start();
    I2C_Send_Byte(0x78); // 从机地址
    I2C_Send_Byte(0x40); // 写数据
    I2C_Send_Byte(Data);
    Sim_I2C_Stop();
}

/**
 * @brief  软延时。
 * @param  无
 * @retval 无
 */
void OLED_delay(void)
{
    uint32_t i, j;
    for (i = 0; i < 2000; i++) // 上电延时
    {
        for (j = 0; j < 2000; j++)
            ;
    }
}

/**
 * @brief  设置屏幕显示起始坐标。
 * @param  Line 行（页）地址，以左上角为原点，向下方向的坐标。
 *     @arg 取值: 0 - 7
 * @param  Column 列地址，以左上角为原点，向右方向的坐标。
 *     @arg 取值: 0 - 127
 * @retval 无
 */
void OLED_SetCursor(uint8_t Line, uint8_t Column)
{
    OLED_WriteCommand(0xB0 | Line);                   // 设置行地址位置
    OLED_WriteCommand(0x10 | ((Column & 0xF0) >> 4)); // 设置列地址位置高4位
    OLED_WriteCommand(0x00 | (Column & 0x0F));        // 设置列地址位置低4位
}

/**
 * @brief  OLED熄屏。
 * @param  无
 * @retval 无
 */
void OLED_Display_Off(void)
{
    OLED_WriteCommand(0XAE); // 熄屏
    OLED_WriteCommand(0X8D); // 设置OLED电荷泵
    OLED_WriteCommand(0X10); // 失能，关
}

/**
 * @brief  OLED亮屏。
 * @param  无
 * @retval 无
 */
void OLED_Display_On(void)
{
    OLED_WriteCommand(0X8D); // 设置OLED电荷泵
    OLED_WriteCommand(0X14); // 使能，开
    OLED_WriteCommand(0XAF); // 亮屏
}

/**
 * @brief  设置屏幕对比度。
 * @param  contrast 对比度值。
 *     @arg 有效取值: 0x00 - 0xFF
 * @retval 无
 */
void OLED_SetContrast(uint8_t contrast)
{
    OLED_WriteCommand(0x81); // 设置对比度
    OLED_WriteCommand(contrast);
}

/**
 * @brief  设置OLED显示模式 - 正显/反显。
 * @param  mode 显示模式。
 *     @arg 有效取值:
 *      - \b POSITIVE_MODE : 正显（1 亮 0 灭）
 *      - \b NEGATIVE_MODE : 反显（0 亮 1 灭）
 * @retval 无
 */
void OLED_SetDisplayMode(OLED_DisplayMode mode)
{
    OLED_WriteCommand((uint8_t)mode);
}

/**
 * @brief  OLED测试模式/正常模式。
 *      注：切换到测试模式不会破坏原有数据，且此模式仍可传输显示数据，待退出测试模式后会正常显示。
 * @param  cmd 指定模式。
 *     @arg 有效取值:
 *      - \b TEST_ON : 测试模式（屏幕全亮，忽略显示数据）
 *      - \b TEST_OFF : 正常模式
 * @retval 无
 */
void OLED_Test(OLED_TestMode mode)
{
    OLED_WriteCommand((uint8_t)mode);
}

/**
 * @brief  OLED清屏。
 * @param  无
 * @retval 无
 */
void OLED_Clear(void)
{
    uint8_t i, j;
    for (j = 0; j < 8; j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
 * @brief  OLED清指定行。
 * @param  LineS 起始行行号。
 *     @arg 有效取值: 1 - 8
 * @param  LineE 结尾行行号。
 *      注意：LineE必须大于等于LineS。
 *     @arg 有效取值: 1 - 8
 * @retval 无
 */
void OLED_ClearLine(uint8_t LineS, uint8_t LineE)
{
    uint8_t i, j;
    for (j = (LineS - 1); j <= (LineE - 1); j++)
    {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++)
        {
            OLED_WriteData(0x00);
        }
    }
}

/**
 * @brief  设置屏幕内容连续水平滚动。
 *      注意：调用本函数后，若要更新显示数据必须先停止滚动，数据全部传输完成后再启动滚动，否则极易显示乱码。
 *      推荐顺序：调用OLED_Stop_Scroll -> 传输显示数据 -> 调用OLED_Start_Scroll
 * @param  ScrLR 滚动方向。
 *     @arg 有效取值:
 *      - \b ScrL : 向左滚动
 *      - \b ScrR : 向右滚动
 * @param  LineS 滚动行范围: 起始行行号。
 *     @arg 有效取值: 1 - 8
 * @param  LineE 滚动行范围: 结尾行行号。
 *      注意：LineE必须大于等于LineS。
 *     @arg 有效取值: 1 - 8
 * @param  Speed 滚动速度。
 *     @arg 有效取值(由慢到快):
 *      - \b OLED_ScrSpeed1
 *      - \b OLED_ScrSpeed2
 *      - \b OLED_ScrSpeed3
 *      - \b OLED_ScrSpeed4
 *      - \b OLED_ScrSpeed5
 *      - \b OLED_ScrSpeed6
 *      - \b OLED_ScrSpeed7
 *      - \b OLED_ScrSpeed8
 * @retval 无
 */
void OLED_Scroll_H(OLED_ScrHorDir ScrLR, uint8_t LineS, uint8_t LineE, OLED_ScrSpeed Speed)
{
    OLED_WriteCommand(0x2E); // 关闭滚动
    OLED_delay();
    OLED_WriteCommand((uint8_t)ScrLR); // 水平滚动方向
    OLED_WriteCommand(0x00);           // 空字节，固定0x00
    OLED_WriteCommand(LineS - 1);      // 水平滚动起始行
    OLED_WriteCommand((uint8_t)Speed); // 滚动速度（间隔帧数）
    OLED_WriteCommand(LineE - 1);      // 水平滚动终止行
    OLED_WriteCommand(0x00);           // 空字节，固定0x00
    OLED_WriteCommand(0xFF);           // 空字节，固定0xFF
    OLED_WriteCommand(0x2F);           // 开启滚动
}

#ifdef OLED_SSD1315
/**
 * @brief  设置屏幕内容连续垂直和水平滚动。
 *      注意：调用本函数后，若要更新显示数据必须先停止滚动，数据全部传输完成后再启动滚动，否则极易显示乱码。
 *      推荐顺序：调用OLED_Stop_Scroll -> 传输显示数据 -> 调用OLED_Start_Scroll
 * @param  PixLineS 垂直滚动起始像素行。
 *     @arg 有效取值: 1 - 64
 * @param  PixLineNum 执行垂直滚动的像素行数。
 *     @arg 有效取值: 1 - 64
 * @param  Horizontal 设置是否启用水平方向滚动。
 *     @arg 有效取值:
 *      - \b ScrH_ON : 启用水平滚动
 *      - \b ScrH_OFF : 禁用水平滚动
 * @param  ScrVLR 水平滚动方向，仅在启用水平方向滚动时有效。
 *     @arg 有效取值:
 *      - \b ScrVL : 垂直+向左滚动
 *      - \b ScrVR : 垂直+向右滚动
 * @param  LineS 水平滚动起始行，仅在启用水平方向滚动时有效。
 *     @arg 有效取值: 1 - 8
 * @param  LineE 水平滚动终止行，仅在启用水平方向滚动时有效。
 *     @arg 有效取值: 1 - 8
 * @param  ColumnS 水平滚动起始列，仅在启用水平方向滚动时有效。
 *     @arg 有效取值: 1 - 128
 * @param  ColumnE 水平滚动终止列，仅在启用水平方向滚动时有效。
 *     @arg 有效取值: 1 - 128
 * @param  Offset 每次垂直向上滚动的偏移量。
 *     @arg 有效取值: 0 - 63
 * @param  Speed 滚动速度。
 *     @arg 有效取值(由慢到快):
 *      - \b OLED_ScrSpeed1
 *      - \b OLED_ScrSpeed2
 *      - \b OLED_ScrSpeed3
 *      - \b OLED_ScrSpeed4
 *      - \b OLED_ScrSpeed5
 *      - \b OLED_ScrSpeed6
 *      - \b OLED_ScrSpeed7
 *      - \b OLED_ScrSpeed8
 * @retval 无
 */
void OLED_Scroll_VH(uint8_t PixLineS, uint8_t PixLineNum,
                    OLED_ScrVerHorMode Horizontal, OLED_ScrVerHorDir ScrVLR,
                    uint8_t LineS, uint8_t LineE,
                    uint8_t ColumnS, uint8_t ColumnE,
                    uint8_t Offset, OLED_ScrSpeed Speed)
{
    OLED_WriteCommand(0x2E); // 关闭滚动
    OLED_delay();
    OLED_WriteCommand(0xA3);                // 启用部分区域水平+垂直滚动
    OLED_WriteCommand(PixLineS - 1);        // 垂直滚动起始像素行
    OLED_WriteCommand(PixLineNum - 1);      // 执行垂直滚动的像素行数
    OLED_WriteCommand((uint8_t)ScrVLR);     // 滚动方向
    OLED_WriteCommand((uint8_t)Horizontal); // 水平滚动开关
    OLED_WriteCommand(LineS - 1);           // 水平滚动起始行
    OLED_WriteCommand((uint8_t)Speed);      // 滚动速度（间隔帧数）
    OLED_WriteCommand(LineE - 1);           // 水平滚动终止行
    OLED_WriteCommand(Offset);              // 垂直向上滚动偏移量，0x00 - 0x3F
    OLED_WriteCommand(ColumnS - 1);         // 水平滚动起始列
    OLED_WriteCommand(ColumnE - 1);         // 水平滚动终止列
    OLED_WriteCommand(0x2F);                // 开启滚动
}
#elif defined(OLED_SSD1306)
/**
 * @brief  设置屏幕内容连续垂直和水平滚动。要实现仅垂直滚动，将LineS和LineE都设为1即可。
 *      注意：调用本函数后，若要更新显示数据必须先停止滚动，数据全部传输完成后再启动滚动，否则极易显示乱码。
 *      推荐顺序：调用OLED_Stop_Scroll -> 传输显示数据 -> 调用OLED_Start_Scroll
 * @param  ScrVLR 水平滚动方向。
 *     @arg 有效取值:
 *      - \b ScrVL : 垂直+向左滚动
 *      - \b ScrVR : 垂直+向右滚动
 * @param  LineS 水平滚动起始行。
 *     @arg 有效取值: 1 - 8
 * @param  LineE 水平滚动终止行。
 *     @arg 有效取值: 1 - 8
 * @param  PixLineS 垂直滚动起始像素行。
 *     @arg 有效取值: 1 - 64
 * @param  PixLineNum 执行垂直滚动的像素行数。
 *     @arg 有效取值: 1 - 64
 * @param  Offset 每次垂直向上滚动的偏移量。
 *     @arg 有效取值: 0 - 63
 * @param  Speed 滚动速度。
 *     @arg 有效取值(由慢到快):
 *      - \b OLED_ScrSpeed1
 *      - \b OLED_ScrSpeed2
 *      - \b OLED_ScrSpeed3
 *      - \b OLED_ScrSpeed4
 *      - \b OLED_ScrSpeed5
 *      - \b OLED_ScrSpeed6
 *      - \b OLED_ScrSpeed7
 *      - \b OLED_ScrSpeed8
 * @retval 无
 */
void OLED_Scroll_VH(OLED_ScrVerHorDir ScrVLR, uint8_t LineS, uint8_t LineE,
                    uint8_t PixLineS, uint8_t PixLineNum,
                    uint8_t Offset, OLED_ScrSpeed Speed)
{
    OLED_WriteCommand(0x2E); // 关闭滚动
    OLED_delay();
    OLED_WriteCommand((uint8_t)ScrVLR); // 滚动方向
    OLED_WriteCommand(0x00);            // 空字节，固定0x00
    OLED_WriteCommand(LineS - 1);       // 水平滚动起始行
    OLED_WriteCommand((uint8_t)Speed);  // 滚动时间间隔
    OLED_WriteCommand(LineE - 1);       // 水平滚动终止行
    OLED_WriteCommand(Offset);          // 垂直滚动偏移量，0x00 - 0x3F
    OLED_WriteCommand(0xA3);            // 启用部分区域水平+垂直滚动
    OLED_WriteCommand(PixLineS - 1);    // 垂直滚动起始像素行
    OLED_WriteCommand(PixLineNum - 1);  // 执行垂直滚动的像素行数
    OLED_WriteCommand(0x2F);            // 开启滚动
}
#endif

/**
 * @brief  OLED停止屏幕滚动。
 * @param  无
 * @retval 无
 */
void OLED_Stop_Scroll(void)
{
    OLED_WriteCommand(0x2E); // 关闭滚动
}

/**
 * @brief  OLED启动屏幕滚动。
 * @param  无
 * @retval 无
 */
void OLED_Start_Scroll(void)
{
    OLED_WriteCommand(0x2F); // 开启滚动
}

/**
 * @brief  计算x^y。
 * @param  X 无符号整形数。
 * @param  Y 无符号整形数。
 * @retval Result X的Y次方
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    return Result;
}

/**
 * @brief  OLED初始化。
 *      PB9 - SDA | PB8 - SCL
 * @param  无
 * @retval 无
 */
void OLED_Init(void)
{
    OLED_delay();

    Sim_I2C_Init(); // 端口初始化

    OLED_WriteCommand(0xAE); // 关闭显示

    OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F);

    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00);

    OLED_WriteCommand(0x40); // 设置显示开始行

    OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // 设置对比度控制
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6); // 设置正常/倒转显示

    OLED_WriteCommand(0x8D); // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // OLED清屏
}

/**
 * @brief  OLED显示一个字符。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Char 要显示的一个字符。
 *     @arg 取值: ASCII可见字符
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowChar(uint8_t Line, uint8_t Column, int8_t Char, uint8_t Size)
{
    uint8_t i;
    if (Size == 8) // 字符大小8x16
    {
        OLED_SetCursor(Line - 1, Column - 1); // 设置光标位置在上半部分
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i]); // 显示上半部分内容
        }
        OLED_SetCursor((Line - 1) + 1, Column - 1); // 设置光标位置在下半部分
        for (i = 0; i < 8; i++)
        {
            OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]); // 显示下半部分内容
        }
    }
    else // 字符大小6x8
    {
        OLED_SetCursor(Line - 1, Column - 1);
        for (i = 0; i < 6; i++)
        {
            OLED_WriteData(OLED_F6x8[Char - ' '][i]);
        }
    }
}

/**
 * @brief  OLED显示字符串。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  String 要显示的字符串。
 *     @arg 取值: ASCII可见字符
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String, uint8_t Size)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, String[i], Size);
    }
}

/**
 * @brief  OLED显示数字（十进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 4294967295
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', Size);
    }
}

/**
 * @brief  OLED显示数字（十进制，带符号数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: -2147483648 - +2147483647
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length, uint8_t Size)
{
    uint32_t Number1;
    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+', Size);
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-', Size);
        Number1 = -Number;
    }
    OLED_ShowNum(Line, Column + Size, Number1, Length, Size);
}

/**
 * @brief  OLED显示有符号浮点数
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Num 要显示的数字。
 *     @arg 取值: -3.4028235E38 - +3.4028235E38
 * @param  Intlen 要显示的整数位数。
 *     @arg 取值: 1 - 10
 * @param  Declen 要显示的小数位数。
 *     @arg 取值: 1 - 10
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowFloat(uint8_t Line, uint8_t Column, float Num, uint8_t Intlen, uint8_t Declen, uint8_t Size)
{
    uint8_t p;
    unsigned long m;

    OLED_ShowSignedNum(Line, Column, (int32_t)Num, Intlen, Size);

    if (Declen > 0)
        OLED_ShowChar(Line, Column + Size * (Intlen + 1), '.', Size);

    if (Num < 0)
        Num = -Num;
    for (p = 2, m = 10; p <= Declen + 1; p++, m *= 10)
    {
        OLED_ShowNum(Line, Column + Size * (Intlen + p), (unsigned long)((Num - (uint32_t)Num) * m) % 10, 1, Size);
    }
}

/**
 * @brief  OLED显示数字（十六进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 0xFFFFFFFF
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 8
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + Size * i, SingleNumber + '0', Size);
        }
        else
        {
            OLED_ShowChar(Line, Column + Size * i, SingleNumber - 10 + 'A', Size);
        }
    }
}

/**
 * @brief  OLED显示数字（二进制，正数）。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Number 要显示的数字。
 *     @arg 取值: 0 - 1111 1111 1111 1111
 * @param  Length 要显示数字的长度。
 *     @arg 取值: 1 - 16
 * @param  Size 字符大小。
 *     @arg 取值(宽x高): 6（6x8）、8（8x16）
 * @retval 无
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Size)
{
    uint8_t i;
    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + Size * i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', Size);
    }
}

/**
 * @brief  OLED显示汉字。
 * @param  Line 起始行位置。
 *     @arg 取值: 1 - 8
 * @param  Column 起始列位置。
 *     @arg 取值: 1 - 128
 * @param  Num 要显示的汉字在字库数组中的索引号（下标号）。
 * @retval 无
 */
void OLED_ShowCN(uint8_t Line, uint8_t Column, uint8_t Num)
{
    uint8_t i;
    uint8_t wide = 16; // 字宽

    OLED_SetCursor(Line - 1, Column - 1); // 参数1:把光标设置在第几行. 参数2:把光标设置在第几列
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i]); // 显示上半部分内容
    }
    OLED_SetCursor((Line - 1) + 1, Column - 1);
    for (i = 0; i < wide; i++)
    {
        OLED_WriteData(OLED_HzK[Num][i + wide]); // 显示下半部分内容
    }
}

/**
 * @brief  OLED显示图片。
 * @param  LineS 绘制图片的起始行位置。
 *     @arg 取值: 1 - 8
 * @param  LineE 绘制图片的终止行位置。
 *     @arg 取值: 1 - 8
 * @param  ColumnS 绘制图片的起始列位置。
 *     @arg 取值: 1 - 128
 * @param  ColumnE 绘制图片的终止列位置。
 *     @arg 取值: 1 - 128
 * @param  BMP 图片模数组。
 * @retval 无
 */
void OLED_DrawBMP(uint8_t LineS, uint8_t LineE, uint8_t ColumnS, uint8_t ColumnE, uint8_t BMP[])
{
    uint32_t j = 0;
    uint8_t x, y;

    if ((LineE - 1) % 8 == 0)
        y = (LineE - 1) / 8;
    else
        y = (LineE - 1) / 8 + 1;
    for (y = (LineS - 1); y <= (LineE - 1); y++)
    {
        OLED_SetCursor(y, (ColumnS - 1));
        for (x = (ColumnS - 1); x <= (ColumnE - 1); x++)
        {
            OLED_WriteData(BMP[j++]);
        }
    }
}
