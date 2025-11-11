#include "stm32f10x.h"

void Spi_Init(void)
{
	/* 使能GPIOA和SPI1时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;							//上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;								// MISO
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;						//输入模式下没啥用
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	    			// 推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 			    			// SS
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 					// 复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;  			// SCK 和 MOSI
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* SPI初始化 */
	SPI_InitTypeDef SPI_InitStructure;									
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						 // 模式，选择为SPI主模式
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	 // 方向，选择2线全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					 // 数据宽度，选择为8位
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					 // 先行位，选择高位先行
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;   // 波特率分频，选择2分频
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							 // SPI极性，选择低极性
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						 // SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							 // NSS，选择由软件控制
	SPI_InitStructure.SPI_CRCPolynomial = 7;							 // CRC多项式，暂时用不到，给默认值7

	SPI_Init(SPI1, &SPI_InitStructure);

	SPI_Cmd(SPI1, ENABLE); 												 // 使能SPI外设
}

void Spi_Start(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4); // 片选拉低
}

void Spi_Stop(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4); // 片选拉高
}

uint8_t Spi_WriteReadByte(uint8_t TxData)
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 		 // 等待发送区空
	SPI_I2S_SendData(SPI1, TxData); 									 // 发送数据

	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);		 // 等待接收区非空
	return SPI_I2S_ReceiveData(SPI1); 									 // 返回接收到的数据
}
