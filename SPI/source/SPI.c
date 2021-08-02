#include "stm32f1xx.h"
#include "SPI.h"
#include "System.h"
#include "GPIO.h"

//void SPI_Send_DigiPot(uint8_t addr, uint8_t val){
//	cs_low();
//	//uint16_t data = (addr<<14) | (val<<8);
//	uint16_t data = (addr<<8) | val;
//	while(!(SPI1->SR & SPI_SR_TXE));
//	SPI1->DR = data;
//}
void spi_send_array(SPI_TypeDef *SPIx, uint8_t *data){
	for(int i = 0; i <= sizeof(data)/sizeof(data[0]); i++){
		spi_send8(SPIx, data[i]);
	}
}
void spi_send8(SPI_TypeDef *SPIx, uint8_t data){
	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = data;
}
void spi_send16(SPI_TypeDef *SPIx, uint16_t data){
	uint8_t littleByte = data & 0xFF;
	uint8_t bigByte = (data>>8) & 0xFF;

	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = bigByte;
	while(!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = littleByte;

}
uint8_t spi_waiting_read8(SPI_TypeDef *SPIx){
	SPIx->DR = 0;
	while(!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
uint8_t spi_request_read8(SPI_TypeDef *SPIx){
	SPIx->DR = 0;
	while(!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR;
}
void spi_cs_on(int cs){
	gpio_state(cs, LOW);
}
void spi_cs_off(int cs){
	gpio_state(cs, HIGH);
}
void spi_init(SPI_TypeDef *SPIx, int slave_pin, int speed_div_, int mode_, int data_length_, int first_bit_){
	/*
	 * -----------------------------------------------
	 *  SPI1 - APB2;
	 *
	 * 	NSS -- 	PA4 AFIO PP / Input pull-up
     *  SCK -- 	PA5 AFIO PP
	 *  MISO -- PA6 Input pull-up
	 *  MOSI -- PA7 AFIO PP
	 *  ----------------------------------------------
	 *  SPI2 - APB1;
	 *
	 *  NSS -- 	PB12 AFIO PP / Input pull-up
     *  SCK -- 	PB13 AFIO PP
	 *  MISO -- PB14 Input pull-up
	 *  MOSI -- PB15 AFIO PP
	 *  ----------------------------------------------
	 *  Mode 0 - CPOL=0; CPHA=0
	 *  Mode 1 - CPOL=0; CPHA=1
	 *  Mode 2 - CPOL=1; CPHA=0
	 *  Mode 3 - CPOL=1; CPHA=1
	 */
	if(SPIx == SPI1){
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN;

		gpio_init(PA5, Alternative_PP, Max_50_MHz); 	//SCK
		gpio_init(PA6, Input_pullup, Input); 			//MISO
		gpio_init(PA7, Alternative_PP, Max_50_MHz); 	//MOSI
		gpio_init(slave_pin, General_PP, Max_50_MHz); 	//NSS

		RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	}
	else if(SPIx == SPI2){
		RCC->APB2ENR |= RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPBEN;

		gpio_init(PB13, Alternative_PP, Max_50_MHz); //SCK
		gpio_init(PB14, Input_pullup, Input); //MISO
		gpio_init(PB15, Alternative_PP, Max_50_MHz); //MOSI
		gpio_init(slave_pin, General_PP, Max_50_MHz); //NSS

		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	}

	SPIx->CR1 |= speed_div_ << 3;
	SPIx->CR1 |= mode_ << 0;
	SPIx->CR1 |= data_length_ << 11;
	SPIx->CR1 |= first_bit_ << 7;
	SPIx->CR1 |= SPI_CR1_MSTR;  //Mode Master
	SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; //Software slave management & Internal slave select
	SPIx->CR1 |= SPI_CR1_SPE; //Enable SPI1

}



