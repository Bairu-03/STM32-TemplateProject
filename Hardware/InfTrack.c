#include "stm32f10x.h"
#include "InfTrack.h"

/**
 * @brief  初始化红外寻迹模块
 * @param  无
 * @retval 无
 */
void InfTracker_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_Periph, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_InitStructure.GPIO_Pin = ITOUT5 | ITOUT4 | ITOUT3 | ITOUT2 | ITOUT1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  读取红外寻迹传感器状态数据
 * @param  无
 * @retval 5位二进制数，高位到低位对应传感器左对管（OUT1）到右对管（OUT5）
 */
uint8_t Get_InfTdata(void)
{
    uint8_t InfTdata, i;
    for (i = 0; i < 5; i++)
    {
        InfTdata = InfTdata << 1;
        InfTdata = InfTdata + GPIO_ReadInputDataBit(GPIO, ITOUT1 >> i);
    }
    return InfTdata;
}
