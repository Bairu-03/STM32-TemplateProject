#ifndef __INFTRACK_H
#define __INFTRACK_H

// 寻迹模块IO口定义
#define RCC_Periph RCC_APB2Periph_GPIOB
#define InfTGPIO GPIOB
#define ITOUT1 GPIO_Pin_7
#define ITOUT2 GPIO_Pin_6
#define ITOUT3 GPIO_Pin_5
#define ITOUT4 GPIO_Pin_4
#define ITOUT5 GPIO_Pin_3

#define IT_M() GPIO_ReadInputDataBit(InfTGPIO, ITOUT3)  // 五路红外寻迹-中间
#define IT_L1() GPIO_ReadInputDataBit(InfTGPIO, ITOUT2) // 五路红外寻迹-左1
#define IT_L2() GPIO_ReadInputDataBit(InfTGPIO, ITOUT1) // 五路红外寻迹-左2
#define IT_R1() GPIO_ReadInputDataBit(InfTGPIO, ITOUT4) // 五路红外寻迹-右1
#define IT_R2() GPIO_ReadInputDataBit(InfTGPIO, ITOUT5) // 五路红外寻迹-右2

void InfTracker_Init(void);
uint8_t Get_InfTdata(void);

#endif
