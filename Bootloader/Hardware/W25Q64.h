#ifndef __W25Q64_H
#define __W25Q64_H

void W25Q64_Init(void);
void W25Q64_Read_ID(uint8_t *MID, uint16_t *DID);
void W25Q64_Read_Data(uint32_t Address, uint8_t *Data, uint16_t Data_Size);
void W25Q64_Block_Erase(uint32_t Address);
void W25Q64_Sector_Erase(uint32_t Address);
void W25Q64_Page_Program(uint32_t Address, uint8_t *Data, uint16_t Data_Size);

#endif
