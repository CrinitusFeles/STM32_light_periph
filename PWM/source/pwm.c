#include "stm32f1xx.h"
#include "pwm.h"
#include "GPIO.h"
#include "System.h"


#define TIMER_DEVIDER 1 //TIMx->PSC

int global_freq; //���������� ���������� ��� �������� �������, ������������ � ������� �������������. ������ �������� ���� ���-�� �������� �������
				// ChangeFilling(TIM_TypeDef *timer, int channel, int fill) �.�. � ��� ���� ���������� ������� ���������� ����.

void PWM_init(TIM_TypeDef *timer, int channel, int freq, int fill){
	/*
	 * ������� ������������� ������ �������.
	 * TIM_TypeDef *timer - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 * int channel - ����� ������ �������. ��������� �� ���� ����� 1-4 ��� �������� �� ������������� ������ PWM_CH1-PWM_CH4.
	 * int freq - ������� ����. ���������� ��������� TIMER_DEVIDER. ��� ������ �������� TIMER_DEVIDER, ��� �� ������� ������� ����� �������� ���.
	 * 			  ��� TIMER_DEVIDER=1 � ������� ������������ ������� = 72��� ����������� ������� ��� = 550��.
	 * 			  ����� ������� ��� ���������� ������� ��� PWM_freq = timer_clk / (timer->ARR * (timer->PSC + 1))
	 * int fill - ������� ���������� ����. ����� ��������� �������� 0-100.
	 */

	global_freq = freq;

	if(timer == TIM1) {
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		if(channel == 1) 	  gpio_init(PA8, Alternative_PP, Max_50_MHz);
		else if(channel == 2) gpio_init(PA9, Alternative_PP, Max_50_MHz);
		else if(channel == 3) gpio_init(PA10, Alternative_PP, Max_50_MHz);
		else if(channel == 4) gpio_init(PA11, Alternative_PP, Max_50_MHz);
	}
	else if(timer == TIM3) {
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		if(channel == PWM_CH1) 	  gpio_init(PA6, Alternative_PP, Max_50_MHz);
		else if(channel == PWM_CH2) gpio_init(PA7, Alternative_PP, Max_50_MHz);
		else if(channel == PWM_CH3) gpio_init(PB0, Alternative_PP, Max_50_MHz);
		else if(channel == PWM_CH4) gpio_init(PB1, Alternative_PP, Max_50_MHz);
	}

	timer->PSC = TIMER_DEVIDER; // !�������! (��� ���������� �������� �������� ������ ������ ��������)

	int pwm_freq = F_CPU / ((TIMER_DEVIDER + 1) * freq); //��� timer->PSC = 1 ����������� ������� 550�� (�.�. timer->ARR 16 ���)
	timer->ARR = pwm_freq; //��� timer->PSC = 1 ����������� ������� 550�� (�.�. timer->ARR 16 ���)

	switch (channel) { //�������� ��������� �����
		case PWM_CH1:
			timer->CCR1 = pwm_freq * fill / 100; 	//������� ����������
			timer->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0;		//PWM mode 1, ��������� ��� 2 �����
			break;
		case PWM_CH2:
			timer->CCR2 = pwm_freq * fill / 100;	//������� ����������
			timer->CCMR1 |= TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_0;		//PWM mode 1, ��������� ��� 2 �����
			break;
		case PWM_CH3:
			timer->CCR3 = pwm_freq * fill / 100;	//������� ����������
			timer->CCMR2 |= TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_0;	 	//PWM mode 2, ��������� ��� 3 �����
			break;
		case PWM_CH4:
			timer->CCR4 = pwm_freq * fill / 100;	//������� ����������
			timer->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_0;		//PWM mode 2, ��������� ��� 4 �����
			break;
		default:
			break;
	}
	timer->BDTR |= TIM_BDTR_MOE; 	//���������� ���� ��� ������ ��� ����������� �������� TIMx_CCER. �������... �������� � ��� �����...
	timer->CR1 &= ~TIM_CR1_DIR; 	//������� �����
	timer->CR1 &= ~TIM_CR1_CMS; 	//������������ �� ������, Fast PWM
	timer->CR1 |= TIM_CR1_CEN;		//��������� ������
}

void PWM_start_single(TIM_TypeDef *timer, int channel){
	timer->CCER |= (0x3 << (4 * (channel-1))); //�������� �� ����� �����.
}

void PWM_start_all(TIM_TypeDef *timer){
	//������� ��� ������� ���� ������� ������������. ����� ������� ������ ��������� ����� ������� ������� � ������ �����.
	timer->CCER |= TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P;
}

void PWM_stop_all(TIM_TypeDef *timer){
	//������� ��� ��������� ���� ������� ������������
	timer->CCER &= ~(TIM_CCER_CC4E | TIM_CCER_CC4P | TIM_CCER_CC3E | TIM_CCER_CC3P | TIM_CCER_CC2E | TIM_CCER_CC2P | TIM_CCER_CC1E | TIM_CCER_CC1P);
}

void PWM_change_fill(TIM_TypeDef *timer, int channel, int fill){
	//�������� ���������� ���� �� ��������� ������.
	/*
	 * TIM_TypeDef *timer - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 * int channel - ����� ������ �������. ��������� �� ���� ����� 1-4 ��� �������� �� ������������� ������ PWM_CH1-PWM_CH4.
	 * int fill - ������� ���������� ����. ����� ��������� �������� 0-100.
	 */

	if(channel == PWM_CH1) timer->CCR1 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100; // F_CPU / ((TIMER_DEVIDER + 1) * global_freq) - ������� ����
	else if(channel == PWM_CH2) timer->CCR2 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100; // PWM_freq * fill /100 - ������� ����������
	else if(channel == PWM_CH3) timer->CCR3 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100;
	else if(channel == PWM_CH4) timer->CCR4 = F_CPU / ((TIMER_DEVIDER + 1) * global_freq) * fill / 100;
}

void PWM_stop_single(TIM_TypeDef *timer, int channel){
	/*
	 * ������� ���������� ������ ���������� ������.
	 */
	timer->CCER &= ~(0x3 << (4 * (channel-1)));
//	timer->BDTR &= ~TIM_BDTR_MOE;
//	timer->CR1 &= ~TIM_CR1_CEN;

}

void PWM_deinit(TIM_TypeDef *timer){
	/*
	 * ������� ��������������� ���������� �������, �.�. ��������� ���, ��������� �������� ������� � ���� �� � ��������� ���������
	 * TIM_TypeDef *timer - ������� �������, ��������������� ��� �����, �������� TIM1, TIM3 � �.�.
	 */
	timer->CCMR2 = 0;
	timer->CCMR1 = 0;
	timer->CCR1 = 0;
	timer->CCR2 = 0;
	timer->CCR3 = 0;
	timer->CCR4 = 0;
	timer->CCER = 0;
	timer->ARR = 0;
	if(timer == TIM1) {
		RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;
		gpio_init(PA8, General_PP, Max_2_MHz);
		gpio_init(PA9, General_PP, Max_2_MHz);
		gpio_init(PA10, General_PP, Max_2_MHz);
		gpio_init(PA11, General_PP, Max_2_MHz);
	}
	else if(timer == TIM3) {
		RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;
		gpio_init(PA6, General_PP, Max_2_MHz);
		gpio_init(PA7, General_PP, Max_2_MHz);
		gpio_init(PB0, General_PP, Max_2_MHz);
		gpio_init(PB1, General_PP, Max_2_MHz);
	}
}
