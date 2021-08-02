#include "stm32f1xx.h"
#define RCC_HSE_8_MHz	8000000

typedef struct RCC_struct{
	uint32_t SYSCLK;
	uint32_t AHBCLK;
	uint32_t APB1CLK;
	uint32_t APB2CLK;
} RCC_struct;


RCC_struct *InitRCC(int HSE_freq, int PLL_PREDIV, int PLLMUL, int AHB_div, int APB1_div, int APB2_div );
