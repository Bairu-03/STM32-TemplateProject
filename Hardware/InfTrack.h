#ifndef __INFTRACK_H
#define __INFTRACK_H

// 寻迹模块IO口定义
#define RCC_Periph RCC_APB2Periph_GPIOA
#define GPIO GPIOA
#define ITOUT1 GPIO_Pin_12
#define ITOUT2 GPIO_Pin_11
#define ITOUT3 GPIO_Pin_10
#define ITOUT4 GPIO_Pin_9
#define ITOUT5 GPIO_Pin_8

#define IT_M() GPIO_ReadInputDataBit(GPIO, ITOUT3)  // 五路红外寻迹-中间
#define IT_L1() GPIO_ReadInputDataBit(GPIO, ITOUT2) // 五路红外寻迹-左1
#define IT_L2() GPIO_ReadInputDataBit(GPIO, ITOUT1) // 五路红外寻迹-左2
#define IT_R1() GPIO_ReadInputDataBit(GPIO, ITOUT4) // 五路红外寻迹-右1
#define IT_R2() GPIO_ReadInputDataBit(GPIO, ITOUT5) // 五路红外寻迹-右2

void InfTracker_Init(void);
uint8_t Get_InfTdata(void);

#endif
