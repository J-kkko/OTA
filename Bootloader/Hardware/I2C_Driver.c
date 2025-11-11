#include "stm32f10x.h"

void IIC_Init(void)
{
	/* 使能GPIOB和I2C2时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;						
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 								// 复用开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  						// SCL 和 SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C初始化 */
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000; 										// 时钟速度，配置为100KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;								// 时钟占空比，Tlow/Thigh = 2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);

	I2C_Cmd(I2C2, ENABLE); // 使能I2C外设
}
