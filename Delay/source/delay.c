
#include "delay.h"


volatile static uint32_t count = 0;
volatile static uint32_t delay_count = 0;
void SysTick_Handler(){
	count++;
	delay_count++;
}
void Delay(int milli){
	delay_count = 0;
	while(delay_count < milli);
	delay_count = 0;
}
void Freeze_delay(int milli){
	for(int counter = 7200 * milli; counter != 0; counter--);
}
uint32_t GetMicro(){
	return count;
}
uint32_t GetMili(){
	return count;
}
