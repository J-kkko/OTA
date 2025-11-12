#ifndef __AT24C02_H
#define __AT24C02_H

#define AT24C02_DEVICE_ADDRESS  0xA0  // AT24C02器件地址

void AT24C02_Init(void);
void AT24C02_Byte_Write(uint32_t Address, uint8_t Data);
void AT24C02_N_Write(uint32_t Address, uint8_t *Data, uint16_t Data_Size);
void AT24C02_Byte_Read(uint32_t Address, uint8_t *Data);
void AT24C02_N_Read(uint32_t Address, uint8_t *Data, uint16_t Data_Size);

#endif
