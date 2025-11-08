#include "stm32f10x.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1 | GPIO_Pin_0);
}

void LED_SET(int8_t caozuo ,uint16_t GPIO_Pin)
{
	if(caozuo=='1')
		GPIO_SetBits(GPIOA,GPIO_Pin);
	else	
		GPIO_ResetBits(GPIOA,GPIO_Pin);
}

void LED_TURN(uint16_t GPIO_Pin)
{
	if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin)==0)//能够读取当前输出状态
	{
		GPIO_SetBits(GPIOA,GPIO_Pin);
	}
	else	
		GPIO_ResetBits(GPIOA,GPIO_Pin);
	
}

