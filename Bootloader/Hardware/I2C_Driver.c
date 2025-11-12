#include "stm32f10x.h"

void IIC_Init(void)
{
	/* 使能GPIOB和I2C2时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	/* GPIO初始化 */
	GPIO_InitTypeDef GPIO_InitStructure;						
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 									// 复用开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;  							// SCL 和 SDA
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* I2C初始化 */
	I2C_InitTypeDef I2C_InitStructure;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000; 											// 时钟速度，配置为100KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;									// 时钟占空比，Tlow/Thigh = 2
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);

	I2C_Cmd(I2C2, ENABLE); // 使能I2C外设
}

uint8_t IIC_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout;
	Timeout = 10000;								   									// 给定超时计数时间
	while (I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)									// 循环等待指定事件
	{
		Timeout--;		  																
		if (Timeout == 0) 																// 自减到0后，等待超时
		{
			break; 																		// 跳出等待，不等了
		}
	}
	return (Timeout == 0) ? 0 : 1;														// 返回是否超时的标志，0表示超时，1表示未超时
}

void IIC_Write_N_Byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *wdata, uint8_t dataLen)
{
	I2C_GenerateSTART(I2C2, ENABLE); 													// 生成起始条件
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); 									// 等待EV5事件(SB位)，START条件发送完成

	I2C_Send7bitAddress(I2C2, deviceAddress, I2C_Direction_Transmitter);			 	// 发送7位设备地址和传输方向(读/写)
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);  					// 等待EV6事件(ADDR位)，地址发送完成

	I2C_SendData(I2C2, registerAddress); 												// 发送目标寄存器地址
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); 							// 等待EV8事件（当前数据发送中，可加载下一字节）

	// 循环发送多个数据
	for (uint8_t i = 0; i < dataLen; i++)
	{
		I2C_SendData(I2C2, wdata[i]); // 发送第i个数据

		if (i < dataLen - 1)
		{
			// 非最后一个字节：等待EV8事件
			IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
		}
		else
		{
			// 最后一个字节：等待EV8_2事件（数据完全发送完成，收到ACK）
			IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		}
	}

	I2C_GenerateSTOP(I2C2, ENABLE); // 生成停止条件
}

void IIC_Read_N_Byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *rdata, uint8_t dataLen)
{
	I2C_GenerateSTART(I2C2, ENABLE); 													// 生成起始条件
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); 									// 等待EV5事件(SB位)，START条件发送完成

	I2C_Send7bitAddress(I2C2, deviceAddress, I2C_Direction_Transmitter);			 	// 发送7位设备地址和传输方向(读/写)
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);  					// 等待EV6事件(ADDR位)，地址发送完成

	I2C_SendData(I2C2, registerAddress); 												// 发送目标寄存器地址
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING); 							// 等待EV8事件（当前数据发送中，可加载下一字节）

	I2C_GenerateSTART(I2C2, ENABLE); 													// 重复生成起始条件
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT); 									// 等待EV5事件(SB位)，START条件发送完成

	/* 下面2行要切换为接收模式 */
	I2C_Send7bitAddress(I2C2, deviceAddress, I2C_Direction_Receiver);					// 发送7位设备地址和传输方向(读/写)
	IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);     					// 等待EV6事件(ADDR位)，地址发送完成

	// 循环接收多个数据
	for (uint8_t i = 0; i < dataLen; i++)
	{
		if(i == dataLen - 1)
		{
			I2C_AcknowledgeConfig(I2C2, DISABLE); 										// 最后一个字节，禁止ACK
			I2C_GenerateSTOP(I2C2, ENABLE); 											// 生成停止条件
		}

		IIC_WaitEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED); 							// 等待EV7事件（接收数据完成,RXNE非空）
		rdata[i] = I2C_ReceiveData(I2C2); 												// 读取接收到的数据
	}

	I2C_AcknowledgeConfig(I2C2, ENABLE); 												// 恢复应答
}

