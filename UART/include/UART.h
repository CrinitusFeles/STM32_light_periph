/*
 * uart.h
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: crinitusfeles
 */

#ifndef UART_H
#define UART_H

#include "stm32f1xx.h"
enum interrupts_mode {
	UART_disable_interrupts, UART_enable_interrupts
};
enum word_length {
	Parcel_8_bit, Parcel_9_bit
};
enum parity_control {
	UART_disable_parity_control, UART_enable_parity_control
};
enum stop_bits {
	One_stop_bit, Half_stop_bit, Two_stop_bit, One_and_half_stop_bit
};


void uart_init(USART_TypeDef *USARTx, int baudrate_, int data_length, int parity_control_, int stop_bits_);
void uart_tx(USART_TypeDef *USARTx, uint8_t data);
void uart_tx_array(USART_TypeDef *USARTx, char *array);
int uart_read(USART_TypeDef *USARTx);
#endif /* UART_H */
