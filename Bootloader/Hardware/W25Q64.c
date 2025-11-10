#include "stm32f10x.h"
#include "Spi_Driver.h"
#include "W25Q64_Instruction.h"

void W25Q64_Init(void)
{
	Spi_Init();
}

void W25Q64_Write_Enable(void)
{
	Spi_Start();
	Spi_WriteReadByte(W25Q64_WRITE_ENABLE);  						// 交换发送写使能的指令
	Spi_Stop();
}

void W25Q64_Wait_Busy(void)
{
	uint32_t Timeout = 100000;
	Spi_Start();
	Spi_WriteReadByte(W25Q64_READ_STATUS_REGISTER_1);
	while ((Spi_WriteReadByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01) 	// 循环等待忙标志位
	{
		Timeout--;		  											// 等待时，计数值自减
		if (Timeout == 0) 											// 自减到0后，等待超时
		{
			break; 													// 跳出等待，不等了
		}
	}
	Spi_Stop();
}

void W25Q64_Read_ID(uint8_t *MID, uint16_t *DID)
{
	W25Q64_Wait_Busy(); 											// 等待忙结束

	Spi_Start();
	Spi_WriteReadByte(W25Q64_JEDEC_ID);
	*MID = Spi_WriteReadByte(W25Q64_DUMMY_BYTE); 					// 交换接收制造商ID

	// 交换接收设备ID
	*DID = Spi_WriteReadByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID = *DID | Spi_WriteReadByte(W25Q64_DUMMY_BYTE);

	Spi_Stop();
}

void W25Q64_Read_Data(uint32_t Address, uint8_t *Data, uint16_t Data_Size)
{
	W25Q64_Wait_Busy(); // 等待忙结束

	Spi_Start();
	Spi_WriteReadByte(W25Q64_READ_DATA);
	Spi_WriteReadByte(Address >> 16); 								// 交换发送地址23~16位
	Spi_WriteReadByte(Address >> 8); 								// 交换发送地址15~8位
	Spi_WriteReadByte(Address);										// 交换发送地址7~0位
	for (uint16_t i = 0; i < Data_Size; i++)
	{
		Data[i] = Spi_WriteReadByte(W25Q64_DUMMY_BYTE); 			// 交换接收一个字节的数据
	}
	Spi_Stop();
}

void W25Q64_Block_Erase(uint32_t Address)
{
	W25Q64_Wait_Busy(); 											// 等待忙结束

	W25Q64_Write_Enable(); 											// 写使能

	Spi_Start();
	Spi_WriteReadByte(W25Q64_BLOCK_ERASE_64KB);
	Spi_WriteReadByte((Address >> 16) & 0xFF);
	Spi_WriteReadByte((Address >> 8) & 0xFF);
	Spi_WriteReadByte(Address & 0xFF);
	Spi_Stop();
}

void W25Q64_Sector_Erase(uint32_t Address)
{
	W25Q64_Wait_Busy();

	W25Q64_Write_Enable();

	Spi_Start();
	Spi_WriteReadByte(W25Q64_SECTOR_ERASE_4KB);
	Spi_WriteReadByte((Address >> 16) & 0xFF);
	Spi_WriteReadByte((Address >> 8) & 0xFF);
	Spi_WriteReadByte(Address & 0xFF);
	Spi_Stop();
}

void W25Q64_Page_Program(uint32_t Address, uint8_t *Data, uint16_t Data_Size)
{
	W25Q64_Wait_Busy();

	W25Q64_Write_Enable();

	Spi_Start();
	Spi_WriteReadByte(W25Q64_PAGE_PROGRAM);			 
	Spi_WriteReadByte((Address >> 16) & 0xFF);		 
	Spi_WriteReadByte((Address >> 8) & 0xFF);		 
	Spi_WriteReadByte(Address & 0xFF);				 
	for (uint16_t i = 0; i < Data_Size; i++)		
	{
		Spi_WriteReadByte(*(Data + i));				 				// 发送一个字节的数据
	}
	Spi_Stop();
}


