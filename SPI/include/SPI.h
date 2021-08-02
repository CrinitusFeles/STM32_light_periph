
#ifndef SPI_H
#define SPI_H
#include "stm32f1xx.h"
enum spi_mode{
	Mode_0, Mode_1, Mode_2, Mode_3
};
enum spi_data_legth{
	data_8_bit, data_16_bit
};
enum spi_first_bit_mode{
	MSB, LSB
};
enum spi_speed_divider {
	div_2, div_4, div_8, div_16, div_32, div_64, div_128, div_256
};

void SPI1_IRQHandler();
void SPI_Send_DigiPot(uint8_t addr, uint8_t val);
void spi_send8(SPI_TypeDef *SPIx, uint8_t data);
void spi_send_array(SPI_TypeDef *SPIx, uint8_t *data);
void SPI_SEND16(uint16_t data);
void DMA_SPI_Send8(uint8_t data);
void spi_send16(SPI_TypeDef *SPIx, uint16_t data);
uint8_t spi_waiting_read8(SPI_TypeDef *SPIx);
uint8_t spi_request_read8(SPI_TypeDef *SPIx);
void spi_init(SPI_TypeDef *SPIx, int slave_pin, int speed_div_, int mode_, int data_length_, int first_bit_);
void spi_cs_off(int cs);
void spi_cs_on(int cs);

#endif //SPI_H
