/*
 * multithreading.c
 *
 *  Created on: 28 но€б. 2019 г.
 *      Author: BreakingBad
 */
#include "multithreading.h"
#include "delay.h"

int thread_time_counter = 0;
int thread_time_counter_void = 0;
void thread_void(void (*func)(void), int *start_time, int time){
	if((GetMili() - *start_time) > time){

		*start_time = GetMili();
		func();
	}
}
void thread_int(void (*func)(int), int data, int *start_time, int time){
	if((GetMili() - *start_time) > time){

		*start_time = GetMili();
		func(data);
	}
}
void thread_int_pint(void (*func)(int, int*), int data1, int *data2, int time){
	if((GetMili() - thread_time_counter) > time){
		func(data1, data2);
		thread_time_counter = GetMili();
	}
}
void thread_int_int(void (*func)(int, int), int data1, int data2, int time){
	if((GetMili() - thread_time_counter) > time){
		func(data1, data2);
		thread_time_counter = GetMili();
	}
}
