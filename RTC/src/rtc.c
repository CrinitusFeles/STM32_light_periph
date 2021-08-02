#include "stm32f1xx.h"
#include "rtc.h"

uint8_t RTC_Init(){
 //разрешить тактирование модулей управления питанием и управлением резервной областью
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
  //разрешить доступ к области резервных данных
  PWR->CR |= PWR_CR_DBP;
  NVIC_EnableIRQ(RTC_IRQn);
  //если часы выключены - инициализировать их
  if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
  {
    //выполнить сброс области резервных данных
    RCC->BDCR |=  RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;

    //выбрать источником тактовых импульсов внешний кварц 32768 и подать тактирование
    RCC->BDCR |=  RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSE;

    RTC->CRL  |=  RTC_CRL_CNF; //разрешаем конфигурирование регистров RTC

    RTC->PRLL  = 0x7FFF;
    //регистр деления на 32768


    RTC->CRL  &=  ~RTC_CRL_CNF; //запрещаем конфигурирование регистров RTC

    //установить бит разрешения работы и дождаться установки бита готовности
    RCC->BDCR |= RCC_BDCR_LSEON;
    while ((RCC->BDCR & RCC_BDCR_LSEON) != RCC_BDCR_LSEON){}

    RTC->CRL &= (uint16_t)~RTC_CRL_RSF;
    while((RTC->CRL & RTC_CRL_RSF) != RTC_CRL_RSF){}


//    return 1;
  }
  RTC->CRL  |=  RTC_CRL_CNF;
  RTC->CRH  |=  RTC_CRH_SECIE;  //разрешить прерывание от секундных импульсов
  RTC->CRH  |=  RTC_CRH_ALRIE;  //разрешить прерывание при совпадении счетного и сигнального регистра
  RTC->CRL  &=  ~RTC_CRL_CNF;

  return 0;
}
void RTC_SetAlarm(RTC_struct *time){
	uint32_t counter = time->day * 86400 + time->hour * 3600 + time->minute * 60 + time->sec;
	RTC->CRL |= RTC_CRL_CNF;    //включить режим конфигурирования

	RTC->ALRH = counter >> 16;      //записать новое значение счетного регистра
	RTC->ALRL = counter;
	RTC->CRL &= ~RTC_CRL_CNF;   //выйти из режима конфигурирования
}
void RTC_SetTime(RTC_struct *time){
	int counter = time->day * 86400 + time->hour * 3600 + time->minute * 60 + time->sec;
	RTC->CRL |= RTC_CRL_CNF;    //включить режим конфигурирования
	RTC->CNTH = counter >> 16;      //записать новое значение счетного регистра
	RTC->CNTL = counter;          //

	RTC->CRL &= ~RTC_CRL_CNF;   //выйти из режима конфигурировани

}

void RTC_GetTime(RTC_struct *time){
	uint32_t TimeVar = (uint32_t)((RTC->CNTH << 16) | RTC->CNTL);
//	time->day = TimeVar / 86400;
	time->hour = TimeVar % 86400 / 3600;
	time->minute = (TimeVar % 3600) / 60;
	time->sec = (TimeVar % 3600) % 60;
	if(time->hour == 0 && time->minute == 0 && time->sec == 0){
		if(time->month == 1 || time->month == 3 || time->month == 5 || time->month == 7 || time->month == 8 || time->month == 10 || time->month == 12){
			if(time->day < 31){
				time->day++;
			}
			else{
				time->day = 1;
				if(time->month != 12){
					time->month++;
				}
				else{
					time->month = 1;
					time->year++;
				}
			}
		}
		else if(time->month == 2){
			if(time->day < 28){
				time->day++;
			}
			else{
				time->day = 1;
				time->month++;
			}
		}
		else{
			if(time->day < 30){
				time->day++;
			}
			else{
				time->day = 1;
				time->month++;
			}
		}
	}
}

char* date_to_string_decoder(RTC_struct time, char *string_arr){
	uint8_t yB = time.year / 10 + 48;
	uint8_t yL = time.year % 10 + 48;
	uint8_t mB = time.month / 10 + 48;
	uint8_t mL = time.month % 10 + 48;
	uint8_t dB = time.day / 10 + 48;
	uint8_t dL = time.day % 10 + 48;
	string_arr[0] = dB;
	string_arr[1] = dL;
	string_arr[2] = '.';
	string_arr[3] = mB;
	string_arr[4] = mL;
	string_arr[5] = '.';
	string_arr[6] = yB;
	string_arr[7] = yL;
	string_arr[8] = '\0';
	return string_arr;
}
char* time_to_string_decoder(RTC_struct time, char *string_arr){
	uint8_t hB = time.hour / 10 + 48;
	uint8_t hL = time.hour % 10 + 48;
	uint8_t mB = time.minute / 10 + 48;
	uint8_t mL = time.minute % 10 + 48;
	uint8_t sB = time.sec / 10 + 48;
	uint8_t sL = time.sec % 10 + 48;
//	uint8_t *string_arr = malloc(9);
	string_arr[0] = hB;
	string_arr[1] = hL;
	string_arr[2] = ':';
	string_arr[3] = mB;
	string_arr[4] = mL;
	string_arr[5] = ':';
	string_arr[6] = sB;
	string_arr[7] = sL;
	string_arr[8] = '\0';
	return string_arr;
}
