/*
 * GPIO.h
 *
 *  Created on: 28 но€б. 2019 г.
 *      Author: ftor
 */

#ifndef CODE_INC_GPIO_H_
#define CODE_INC_GPIO_H_

#include "stm32f1xx.h"

enum gpio_errors {
	PORT_ERROR=-2, MODE_ERROR, SPEED_ERROR
};
enum gpio {
	PA0, PA1, PA2, PA3, PA4, PA5, PA6, PA7, PA8, PA9, PA10, PA11, PA12, PA13, PA14, PA15,
	PB0, PB1, PB2, PB3, PB4, PB5, PB6, PB7, PB8, PB9, PB10, PB11, PB12, PB13, PB14, PB15,
	PC0, PC1, PC2, PC3, PC4, PC5, PC6, PC7, PC8, PC9, PC10, PC11, PC12, PC13, PC14, PC15,
	PD0, PD1, PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12, PD13, PD14, PD15
};
enum state {
	LOW, HIGH
};
enum mode {
	General_PP, General_OD, Alternative_PP, Alternative_OD, Analog_input=0, Floating_input, Input_pulldown, Input_pullup
};
enum speed {
	Input, Max_10_MHz, Max_2_MHz, Max_50_MHz
};

void gpio_init(int gpio_, int mode_, int speed_);
void spi_cs_on(int cs);
void spi_cs_off(int cs);
void gpio_state(int gpio_, int state_);
void gpio_toggle(int gpio_);
int gpio_read(int gpio_);

#endif /* CODE_INC_GPIO_H_ */
