/*
 * multithreading.h
 *
 *  Created on: 28 но€б. 2019 г.
 *      Author: BreakingBad
 */

#ifndef CODE_INC_MULTITHREADING_H_
#define CODE_INC_MULTITHREADING_H_

void thread_void(void (*func)(void), int *start_time, int time);
void thread_int(void (*func)(int), int data, int *start_time, int time);
void thread_int_pint(void (*func)(int, int*), int data1, int *data2, int time);
void thread_int_int(void (*func)(int, int), int data1, int data2, int time);

#endif /* CODE_INC_MULTITHREADING_H_ */
