#include "stm32f10x.h"
#include "Delay.h"

void KEY_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//输入模式下没啥用
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
}

uint8_t get_KeyNum(void)
{
	uint8_t KeyNum=0;
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0)//按键按下就是低电平
	{
		Delay_ms(20);//消抖
		//如果一直按着就一直在循环，直到松手为止
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);
		Delay_ms(20);//消抖
		KeyNum=1;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)//按键按下就是低电平
	{
		Delay_ms(20);//消抖
		//如果一直按着就一直在循环，直到松手为止
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0);
		Delay_ms(20);//消抖
		KeyNum=2;		
	}
	return KeyNum;
}
