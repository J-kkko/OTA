#ifndef __SPI_DRIVER_H
#define __SPI_DRIVER_H

void Spi_Init(void);
void Spi_Start(void);
void Spi_Stop(void);
uint8_t Spi_WriteReadByte(uint8_t TxData);

#endif
