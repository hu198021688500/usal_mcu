/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   tick_handle
作者:   km
说明:   系统时钟.
***************************************************************************************************/

#ifndef __TICK_HANDLE_H__                     //防重包含.
#define __TICK_HANDLE_H__

#include "base_type.h"

typedef union
{
    uint16_t all;
    struct
    {    
        uint16_t  bEvent1ms:1;
        uint16_t  bEvent10ms:1;
        uint16_t  bEvent100ms:1;
        uint16_t  bEvent1000ms:1;
        uint16_t  bit4:1;
        uint16_t  bit5:1;
        uint16_t  bit6:1;
        uint16_t  bit7:1;
    }bit;
}TimeFlag;

typedef  struct TimerList
{
    struct TimerList *next;
    int32_t runIntervalTime;    //执行间隔
    int32_t expiresTime;        //剩余时间
    void (*function)(void);
}TimerList;


extern BOOL      g_bSysTimer10msEventFlag;    //10ms事件标志.
extern BOOL      g_bSysTimer100msEventFlag;   //100ms事件标志
extern BOOL      g_bSysTimer1sEventFlag;      //1s事件标志

extern uint32_t    g_uSys10msEventCount;      //10ms事件.累积计数.
extern uint32_t    g_uSys100msEventCount;     //100ms事件.累积计数.
extern uint32_t    g_uSys1sEventCount;        //100ms事件.累积计数.


void sys_real_time_handle(void);
void sys_timer1ms_int(void);
uint32_t get_poweron_times(void);
void timer1ms_handle_sys(void);
void timer_task_init(void);
void timer_add_task(TimerList *timerList);
void timer_init_struct(TimerList *timerList,int32_t time,void(*function)(void));

#endif  //__TICK_HANDLE_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/

