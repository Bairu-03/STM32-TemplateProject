#include "stm32f10x.h"
#include "I2C_Software.h"

/**
 * @brief  模拟I2C信号IO口初始化。
 * @param  无
 * @retval 无
 */
void Sim_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(APB2_GPIO, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SCL_Pin;
    GPIO_Init(GPIOX, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = SDA_Pin;
    GPIO_Init(GPIOX, &GPIO_InitStructure);

    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief  模拟I2C起始信号。
 * @param  无
 * @retval 无
 */
void Sim_I2C_Start(void)
{
    OLED_W_SDA(1);
    OLED_W_SCL(1);
    OLED_W_SDA(0);
    OLED_W_SCL(0);
}

/**
 * @brief  模拟I2C停止信号。
 * @param  无
 * @retval 无
 */
void Sim_I2C_Stop(void)
{
    OLED_W_SDA(0);
    OLED_W_SCL(1);
    OLED_W_SDA(1);
}

/**
 * @brief  模拟I2C读取从机应答信号。
 * @param  无
 * @retval 从机应答状态，I2C_NO_ACK: 无应答，I2C_ACK: 应答。
 */
uint8_t I2C_Wait_Ack(void)
{
    uint8_t ack;
    OLED_W_SCL(0);
    OLED_W_SDA(1);
    OLED_W_SCL(1);

    if (OLED_R_SDA())
        ack = I2C_NO_ACK;
    else
        ack = I2C_ACK;

    OLED_W_SCL(0);
    return ack;
}

/**
 * @brief  模拟I2C主机发送应答信号。
 * @param  ack 决定主机是否发送应答信号。
 *     @arg 有效取值:
 *      - \b I2C_ACK : 发送应答信号
 *      - \b I2C_NO_ACK: 不发送应答信号
 * @retval 无
 */
void I2C_Send_Ack(uint8_t ack)
{
    OLED_W_SCL(0);

    if (ack == I2C_ACK)
        OLED_W_SDA(0);
    else
        OLED_W_SDA(1);

    OLED_W_SCL(1);
    OLED_W_SCL(0);
}

/**
 * @brief  I2C读取一个字节。
 * @param  ack 决定主机是否发送应答信号。
 *     @arg 有效取值:
 *      - \b I2C_ACK: 发送应答信号
 *      - \b I2C_NO_ACK: 不发送应答信号
 * @retval data 读取到的数据
 */
uint8_t I2C_Read_Byte(uint8_t ack)
{
    uint8_t data = 0;
    uint8_t i;
    OLED_W_SCL(0);
    OLED_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        OLED_W_SCL(1);
        data <<= 1;

        if (OLED_R_SDA())
            data |= 0x01;
        else
            data &= 0xFE;

        OLED_W_SCL(0);
    }
    I2C_Send_Ack(ack);
    return data;
}

/**
 * @brief  I2C发送一个字节。
 * @param  Byte  要发送的一个字节。
 * @retval 无
 */
void I2C_Send_Byte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        OLED_W_SDA(Byte & (0x80 >> i));
        OLED_W_SCL(1);
        OLED_W_SCL(0);
    }

    // while(I2C_Wait_Ack());    //等待从机应答信号

    OLED_W_SCL(1); // 变化时钟信号，不等待从机应答
    OLED_W_SCL(0);
}
