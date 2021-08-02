#ifndef DELAY_H
#define DELAY_H

#include "stm32f1xx.h"
#include "System.h"

#define millisec F_CPU/1000 - 1 //milisec
#define microsec  F_CPU/1000000-1

void SysTick_Handler();

void Delay(int milli);
void Freeze_delay(int milli);
void MicroDelay(int micro);
uint32_t GetMili();
uint32_t GetMicro();

#endif //DELAY_H
