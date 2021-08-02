/*
 * rcc.c
 *
 *  Created on: 31 џэт. 2020 у.
 *      Author: ftor
 */


#include "rcc.h"

/*
 * _____________________________________________________________________
 *|	AHB 72MHz (max)	|		APB1 36MHz (max)     	|  APB2 72MHz (max)	|
 *|_________________|_______________________________|___________________|
 *|	FSMC			|  DAC							|	TIM11 timer		|
 *| USB OTG FS		|  Power control PWR			|	TIM10 timer		|
 *| Ethernet		|  Backup registers (BKP)		|	TIM9 timer		|
 *| CRC				|  bxCAN1						|	ADC3			|
 *| DMA2			|  bxCAN2						|	USART1 			|
 *| DMA1			|  USB device FS registers		|	TIM8 timer		|
 *| SDIO			|  I2C2							|	SPI1			|
 *|					|  I2C1							|	TIM1 timer		|
 *|					|  UART5						|	ADC2			|
 *|					|  UART4						|	ADC1			|
 *|					|  USART3						|	GPIO Port G		|
 *|					|  USART2						|	GPIO Port F		|
 *|					|  SPI3/I2S						|	GPIO Port E		|
 *|					|  SPI2/I2S						|	GPIO Port D		|
 *|					|  Independent watchdog (IWDG)	|	GPIO Port C		|
 *|					|  Window watchdog (WWDG)		|	GPIO Port B		|
 *|					|  RTC							|	GPIO Port A		|
 *|					|  TIM14 timer					|	EXTI			|
 *|					|  TIM13 timer					|	AFIO			|
 *|					|  TIM12 timer					|					|
 *|					|  TIM7 timer					|					|
 *|					|  TIM6 timer					|					|
 *|					|  TIM5 timer					|					|
 *|					|  TIM4 timer					|					|
 *|					|  TIM3 timer					|					|
 *|					|  TIM2 timer					|					|
 *|_________________|_______________________________|___________________|
 *
 */

//RCC_struct rcc_struct;
RCC_struct *InitRCC(int HSE_freq, int PLL_PREDIV, int PLLMUL, int AHB_div, int APB1_div, int APB2_div ){ //72MHz
	PLLMUL = PLLMUL > 4 ? PLLMUL : 4;

	static RCC_struct rcc_struct;
	rcc_struct.SYSCLK = (HSE_freq * PLLMUL / PLL_PREDIV);
	rcc_struct.AHBCLK = (rcc_struct.SYSCLK / AHB_div);
	rcc_struct.APB1CLK = (rcc_struct.AHBCLK / APB1_div);
	rcc_struct.APB2CLK = (rcc_struct.AHBCLK / APB2_div);


	if(!(RCC->CR & RCC_CR_HSERDY)){ //check HSE if it didn't launch, then launch
		RCC->CR |= RCC_CR_HSEON;
		while(!(RCC->CR & RCC_CR_HSERDY)); //waiting while HSE launching
	}
	RCC->CFGR |= RCC_CFGR_PLLSRC; //adjust PLL mul to 9 and switch source of PLL on HSE

	RCC->CFGR |= ((PLLMUL-2) << 18);

	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/*
	 * 000 Zero wait state, if 0 	  < SYSCLK < 24 MHz
	 * 001 One wait state, 	if 24 MHz < SYSCLK < 48 MHz
	 * 010 Two wait states, if 48 MHz < SYSCLK < 72 MHz
	 */
	FLASH->ACR &= 0xFFFFFFF8; //clear register
	if(rcc_struct.SYSCLK > 48) FLASH->ACR |= FLASH_ACR_LATENCY_1;
	else if(rcc_struct.SYSCLK > 24 && rcc_struct.SYSCLK <= 48) FLASH->ACR |= FLASH_ACR_LATENCY_0;

	RCC->CFGR |= ((AHB_div + 6) << 4) | ((APB1_div + 2) << 8) | ((APB2_div + 2) << 11);

	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));

	RCC->CR &= ~(RCC_CR_HSION);
	return &rcc_struct;
}
