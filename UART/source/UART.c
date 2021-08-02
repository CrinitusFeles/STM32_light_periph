/*
 * uart.c
 *
 *  Created on: 2 РјР°СЂ. 2019 Рі.
 *      Author: crinitusfeles
 */
#include "UART.h"
#include "GPIO.h"
#include "System.h"

//#define F_CPU 72000000 //exists in System.h
#define APB2_CLK F_CPU
#define APB1_CLK F_CPU/2

void uart_init(USART_TypeDef *USARTx, int baudrate_, int data_length, int parity_control_, int stop_bits_){
	/*
	 * USART3
	 * PB10 - TX - PP
	 * PB11 - RX - input pull-up or open drain
	 *
	 *
	 * USART2
	 * PA2 - TX - PP
	 * PA3 - RX - input pull-up or open drain
	 *
	 * USART1
	 * PA10 - RX
	 * PA9  - TX
	 *
	 * APB2 - USART1; APB1 - USART2,3,4,5
	 */

	if(USARTx == USART1){
		gpio_init(PA9, Alternative_PP, Max_50_MHz); //TX
		gpio_init(PA10, Floating_input, Input); //RX
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN; // enable clock
//		NVIC_EnableIRQ(USART1_IRQn);
		USART1->BRR = (APB2_CLK - baudrate_ / 2) / baudrate_;
	}
	else if(USARTx == USART2){
		gpio_init(PA2, Alternative_PP, Max_50_MHz); //TX
		gpio_init(PA3, Floating_input, Input); //RX
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // enable clock
		NVIC_EnableIRQ(USART2_IRQn);
		USART2->BRR = (APB1_CLK - baudrate_ / 2) / baudrate_;
	}
	else if(USARTx == USART3){
		gpio_init(PB10, Alternative_PP, Max_50_MHz); //TX
		gpio_init(PB11, Floating_input, Input); //RX
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable clock
		NVIC_EnableIRQ(USART3_IRQn);
		USART2->BRR = (APB2_CLK - baudrate_ / 2) / baudrate_;
	}

	USARTx->CR1 &= ~(0x01 << 12);
	USARTx->CR1 |= data_length << 12;

	USARTx->CR1 &= ~(0x01 << 10);
	USARTx->CR1 |= parity_control_ << 10;

	USARTx->CR2 &= ~(0x03 << 12);
	USARTx->CR2 |= stop_bits_ << 12;

	USARTx->CR1 |= USART_CR1_TE | USART_CR1_RE;
	USARTx->CR1 |= USART_CR1_UE; // enable USART
	USARTx->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USART1_IRQn);
}

void uart_tx(USART_TypeDef *USARTx, uint8_t data){
	while((USARTx->SR & USART_SR_TC) == 0);
	USARTx->DR = data;
}

void uart_tx_array(USART_TypeDef *USARTx,  char *array){
	for(uint8_t i = 0; array[i] != '\0'; i++){
		uart_tx(USARTx, array[i]);
	}
	uart_tx(USARTx, 0x0D);
	uart_tx(USARTx, 0x0A);
}

int uart_read(USART_TypeDef *USARTx){
	while(!(USARTx->SR & USART_SR_RXNE)); //если данные не поступают, то зависнет навсегда. Лучше добавить таймаут.
//	uart1_data_counter++;
	return USARTx->DR;
}

int int_to_ascii(int num){

}

void USART1_IRQHandler(){
	if(USART1->SR & USART_SR_RXNE){
		uint8_t data = uart_read(USART1);
		printf("%c\n", data);
	}
	else if(USART1->SR & USART_SR_ORE){
		printf("Buffer overflow!\n");
	}
	else if(USART1->SR & USART_SR_TXE){
		printf("tx interrupt!\n");
	}
}
