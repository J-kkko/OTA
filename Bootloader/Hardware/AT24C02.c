#include "stm32f10x.h"
#include "I2C_Driver.h"
#include "AT24C02.h"

void AT24C02_Init(void)
{
	IIC_Init();
}

void AT24C02_Wait_Busy(void)
{	
	while (1)
	{
		// while((I2C2->CR1 & 0x200) == 1);  														// 等待STOP位清除，按照手册上写的，如果STOP没清除就写CR寄存器时序会有问题
		I2C_GenerateSTART(I2C2, ENABLE);				   										// 生成起始条件
		IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); 										// 等待EV5事件(SB位)，START条件发送完成
		I2C_Send7bitAddress(I2C2, AT24C02_DEVICE_ADDRESS, I2C_Direction_Transmitter); 			// 发送7位设备地址和传输方向(读/写)

		if (IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == 1)
		{
			// 收到ACK，t_WR完成，发送停止条件并退出
			I2C_GenerateSTOP(I2C2, ENABLE);
			return;
		}
		else
		{
			// 未收到ACK（仍在t_WR中），发送停止条件，继续轮询
			I2C_GenerateSTOP(I2C2, ENABLE);
			continue;;
		}
	}
}

void AT24C02_Byte_Write(uint32_t Address, uint8_t Data)
{
	AT24C02_Wait_Busy(); 																		// 等待忙结束
	IIC_Write_N_Byte(AT24C02_DEVICE_ADDRESS, Address, &Data, 1);
}

void AT24C02_N_Write(uint32_t Address, uint8_t *Data, uint16_t Data_Size)
{
	AT24C02_Wait_Busy(); 																		// 等待忙结束
	IIC_Write_N_Byte(AT24C02_DEVICE_ADDRESS, Address, Data, Data_Size);
}

void AT24C02_Byte_Read(uint32_t Address, uint8_t *Data)
{
	AT24C02_Wait_Busy(); 																		// 等待忙结束

	IIC_Read_N_Byte(AT24C02_DEVICE_ADDRESS, Address, Data, 1);
}

void AT24C02_N_Read(uint32_t Address, uint8_t *Data, uint16_t Data_Size)
{
	AT24C02_Wait_Busy(); 																		// 等待忙结束

	IIC_Read_N_Byte(AT24C02_DEVICE_ADDRESS, Address, Data, Data_Size);
}
