#ifndef RTC_H
#define RTC_H

typedef struct sRTC_struct{ //32bit
	uint8_t day : 5;
	uint8_t month : 4;
	uint8_t year : 6;
	uint8_t hour : 5;
	uint8_t minute : 6;
	uint8_t sec : 6;
}RTC_struct;

uint8_t RTC_Init();
void RTC_SetTime(RTC_struct *time);
void RTC_GetTime(RTC_struct *time);
char* date_to_string_decoder(RTC_struct time, char *string_arr);
char* time_to_string_decoder(RTC_struct time, char *string_arr);
void RTC_SetAlarm(RTC_struct *time);
#endif /* RTC_H */
