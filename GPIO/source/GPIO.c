/*
 * GPIO.c
 *
 *  Created on: 28 нояб. 2019 г.
 *      Author: ftor
 */

#include "GPIO.h"

#define NUMBER_OF_PINS_ON_EACH_PORT 16

enum port {
	Port_A, Port_B, Port_C, Port_D
};
/*
    _________________________________________________________________________________________________
   | 		Configuration Mode			 |	 CNF1	|	CNF0   |	MODE1	|	MODE0	|	PxODR	 |
   | ____________________________________|__________|__________|____________|___________|____________|
   | General purpose | Push-pull 		 |	  0		|	 0	   |						|	0 or 1	 |		|MODE0,1 - SPEED
   | output		     | Open-drain		 |	  0		|	 1	   |			01			|	0 or 1	 |		|01 - 10MHz
   | ________________|___________________|__________|__________|			10			|____________|		|10 - 2MHz
   | Alternative	 | Push-pull		 |	  1		|	 0	   |			11			| Don't care |		|11 - 50 MHz
   | Function output | Open-drain		 |    1		|	 1	   |						| Don't care |
   | ________________|___________________|__________|__________|________________________|____________|
   | 				 | Analog			 |	  0		|	 0	   |						| Don't care |
   | 				 | Input floating	 |____0_____|____1_____|			00			| Don't care |
   | Input		     | Input pull-down   |	  1		|    0	   |						|	  0		 |
   | 				 | Input pull-up	 |	  1   	|	 0	   |						|	  1		 |
   |_________________|___________________|__________|__________|________________________|____________|

 */
void gpio_init(int gpio_, int mode_, int speed_){
	/*
	 * int gpio_ - число из нумерованного одномерного списка портов
	 * int mode_ - выбор режим работы порта из одномерного нумерованного списка
	 * int speed_ - для режима выхода выбирается максимальная скорость работы порта из одномерного нумерованного списка
	 */
	int pin = gpio_ % NUMBER_OF_PINS_ON_EACH_PORT;
	int CNF = pin >= 8 ? (pin - 8) * 4 + 2 : pin * 4 + 2;
	int MODE = pin >= 8 ? (pin - 8) * 4 : pin * 4;
	GPIO_TypeDef *gpio;


	switch(gpio_ / NUMBER_OF_PINS_ON_EACH_PORT){ //Вычисляет какой порт
		case Port_A:
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
			gpio = GPIOA;
			break;
		case Port_B:
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
			gpio = GPIOB;
			break;
		case Port_C:
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
			gpio = GPIOC;
			break;
		default:
			break;
	}

	if(pin < 8){
		gpio->CRL &= ~(0x03 << MODE | 0x03 << CNF);
		if(mode_ == Input_pullup && speed_ == Input){
			gpio->CRL |= speed_ << MODE | (mode_-1) << CNF;
			gpio->ODR |= 0x01 << pin;
		}
		else{
			gpio->CRL |= speed_ << MODE | mode_ << CNF;
		}
	}
	else{
		gpio->CRH &= ~(0x03 << MODE | 0x03 << CNF);
		if(mode_ == Input_pullup && speed_ == Input){
			gpio->CRH |= speed_ << MODE | (mode_-1) << CNF;
			gpio->ODR |= 0x01 << pin;
		}
		else{
			gpio->CRH |= speed_ << MODE | mode_ << CNF;
		}
	}
}

void gpio_state(int gpio_, int state_){
	/*
	 * Устанавливает выбранный пин в состояние state (LOW or HIGH)
	 */
	int port_ = gpio_ / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio_ % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port_){
		case Port_A:
			GPIOA->BSRR = 0x01 << (pin + (!state_) * 16); //(!state_)
		break;
		case Port_B:
			GPIOB->BSRR = 0x01 << (pin + (!state_) * 16);
		break;
		case Port_C:
			GPIOC->BSRR = 0x01 << (pin + (!state_) * 16);
		break;
	}
}

void gpio_toggle(int gpio_){
	/*
	 * Инвертирует состояние пина
	 */
	int port_ = gpio_ / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio_ % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port_){
		case Port_A:
			GPIOA->ODR ^= 0x01 << pin;
		break;
		case Port_B:
			GPIOB->ODR ^= 0x01 << pin;
		break;
		case Port_C:
			GPIOC->ODR ^= 0x01 << pin;
		break;
	}
}

int gpio_read(int gpio_){
	/*
	 * Возвращает логический уровень указанного пина. ВАЖНО! только пина, а не всего порта
	 */
	int port_ = gpio_ / NUMBER_OF_PINS_ON_EACH_PORT;
	int pin = gpio_ % NUMBER_OF_PINS_ON_EACH_PORT;
	switch(port_){
		case Port_A:
			return (GPIOA->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_B:
			return (GPIOB->IDR & (0x01 << pin)) >> pin;
		break;
		case Port_C:
			return (GPIOC->IDR & (0x01 << pin)) >> pin;
		break;
		default:
			return PORT_ERROR;
		break;
	}
}
