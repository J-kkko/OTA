#ifndef __I2C_DRIVER_H
#define __I2C_DRIVER_H

void IIC_Init(void);
uint8_t IIC_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT);
void IIC_Write_N_Byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *wdata, uint8_t dataLen);
void IIC_Read_N_Byte(uint8_t deviceAddress, uint8_t registerAddress, uint8_t *rdata, uint8_t dataLen);

#endif
