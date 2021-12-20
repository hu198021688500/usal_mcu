/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   time_cur
作者:   km
说明:   时间.
***************************************************************************************************/

#ifndef __SYS_TIME_H__                     //防重包含.
#define __SYS_TIME_H__

#include "base_type.h"
#include <time.h>


extern uint16_t  sycTimeToScree;

extern uint32_t kernel_mktime(const uint16_t year0, const uint16_t mon0, const uint16_t day, 
    const uint16_t hour, const uint16_t min, const uint16_t sec);
extern uint32_t real_timer_init_value(uint32_t year,uint32_t mon_day,uint32_t hour_min_sec);
extern void real_timer_init(uint32_t year,uint32_t mon_day,uint32_t hour_min_sec);
void set_curtimer_val(uint32_t second);
extern uint32_t get_curtimer_val(void);
extern uint32_t get_curtimer_ms(void);
extern void set_curtimer_val(uint32_t val);
extern void timer1s_handle(void);
extern void get_time_string_second(char * pBuffer, uint32_t timeSecond);
extern char *get_time_string(void);
extern char * get_time_string_single(void);
extern void get_time_string_stack(char * pString);
extern void timer1ms_handle(void);
extern uint32_t get_time_format_second(char * pTimeBcdBuffer);
extern void get_time_format_bcd(char * pBuffer, uint32_t timeSecond);
extern char *get_time_buffer(void);
extern void get_time_future(char * pTimeBuf, int32_t future);
extern void real_timer_init_delay(uint32_t year,uint32_t mon_day,uint32_t hour_min_sec,uint16_t delayTime);
extern void realTimer_set(int16_t year, int16_t mon, int16_t mday, int16_t hour, int16_t min, int16_t sec);
extern void realTimer_get(struct tm ** pt);
extern char * realTimer_getString(void);
extern char *get_time_buffer_cp56time2a(void);
extern void set_curtimer_val_delay(uint32_t second, uint16_t delayTime);

#endif  //__SYS_TIME_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/

