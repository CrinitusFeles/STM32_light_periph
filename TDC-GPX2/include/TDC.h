/*
 * TDC.h
 *
 *  Created on: 24 апр. 2019 г.
 *      Author: zzzro
 */
#include "stm32f1xx.h"
#ifndef CODE_INCLUDE_TDC_H_
#define CODE_INCLUDE_TDC_H_

#include "SPI.h"
#include "TDC.h"
#include "macro.h"

void TDC_Init();
void SSN_Pulse();
void TDC_Results();
void Get_Results(uint8_t *arr);
void Get_Results_Aver(uint8_t *resultArray);
#endif /* CODE_INCLUDE_TDC_H_ */
