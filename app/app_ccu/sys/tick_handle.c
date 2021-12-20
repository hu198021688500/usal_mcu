/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   tick_handle
作者:   km
说明:   系统时钟.
***************************************************************************************************/
#include "tick_handle.h"         
#include <string.h>
#include "time_cur.h"

uint8_t     g_uSys1msFlag = 0;                   //1ms发生标志.
uint8_t     g_uSys1msEvent = 0;                  //1ms事件.0-9.
uint8_t     g_uSys10msEvent = 0;                 //10ms事件.0-9.
uint8_t     g_uSys100msEvent = 0;                //100ms事件.0-9.

uint32_t    g_uSys10msEventCount = 0;                   //10ms事件.累积计数.
uint32_t    g_uSys100msEventCount = 0;                 //100ms事件.累积计数.
uint32_t    g_uSys1sEventCount = 0;                    //1s事件.累积计数.
uint32_t    g_uSys1sEventCountInt = 0;                 //中断1s事件.累积计数.

BOOL      g_bSysMsgTimer10ms = BOOL_FALSE;    //10ms消息.
BOOL      g_bSysMsgTimer100ms = BOOL_FALSE;   //100ms消息.
BOOL      g_bSysMsgTimer1s = BOOL_FALSE;      //1s消息.

BOOL      g_bSysTimer10msEventFlag = BOOL_FALSE;    //10ms事件标志.
BOOL      g_bSysTimer100msEventFlag = BOOL_FALSE;   //100ms事件标志.
BOOL      g_bSysTimer1sEventFlag = BOOL_FALSE;      //1s事件标志.

static struct TimerList *timerHead=0;

void sys_timer1ms_int(void);
void sys_timer1ms(void);
void sys_timer1s_int(void);

void sys_timer10ms(void);
void sys_timer100ms(void);
void sys_timer1s(void);

void timer_task_init(void)
{
    
}

void timer_add_task(TimerList *timerList)
{
    TimerList *pTimeList = timerHead;
    timerHead = timerList;
    timerList->next = pTimeList;
}

void timer_init_struct(TimerList *timerList,int32_t time,void(*function)(void))
{
    if ((!timerList)||(!function)||(time < 1))
    {
        return;
    }
    
    timerList->next = 0;
    timerList->runIntervalTime = time;
    timerList->function = function;
}

void timer_task_run(void)
{
    TimerList *pTimeList = timerHead;
    while(pTimeList)
    {
        if (pTimeList->expiresTime > 0)
        {
            pTimeList->expiresTime--;
            if(pTimeList->expiresTime == 0)
            {
                if (pTimeList->function)
                {
                    pTimeList->function();
                }
                
                pTimeList->expiresTime = pTimeList->runIntervalTime;
            }
        }
        else
        {
            if (pTimeList->function)
            {
                pTimeList->function();
            }
            pTimeList->expiresTime = pTimeList->runIntervalTime;
        }
        
        pTimeList = pTimeList->next;
    }
}

/***************************************************************************************************
函数:   系统定时10ms事件处理.中断中.
输入:   无.
返回:   无.
说明:   在中断内.产生各种定时消息.
***************************************************************************************************/
void sys_timer1ms_int(void)
{
    g_uSys1msEvent++;
    g_uSys1msFlag = BOOL_TRUE;
    if(g_uSys1msEvent >= 10)
    {
        g_uSys1msEvent = 0;
        g_bSysMsgTimer10ms = BOOL_TRUE;                 //定时10ms消息.
        g_uSys10msEvent++;
        if (g_uSys10msEvent >= 10)
        {
            g_uSys10msEvent = 0;
            g_bSysMsgTimer100ms = BOOL_TRUE;            //定时100ms消息.
            g_uSys100msEvent++;
            if (g_uSys100msEvent >= 10)
            {
                g_uSys100msEvent = 0;
                g_uSys1sEventCountInt++;
                g_bSysMsgTimer1s = BOOL_TRUE;           //定时1s消息.
            }
        }
    }
}

/***************************************************************************************************
函数:   系统实时事件处理.
输入:   无
返回:   无
说明:   本函数被main()循环调用,实现超级循环.
        本函数的作用就是确定在每一次循环时要做的事.
        除了中断处理函数外,这个函数是所有其它函数调用的本源.
***************************************************************************************************/
void sys_real_time_handle(void)
{
    g_bSysTimer10msEventFlag = BOOL_FALSE;    //10ms事件标志.
    g_bSysTimer100msEventFlag = BOOL_FALSE;   //100ms事件标志.
    g_bSysTimer1sEventFlag = BOOL_FALSE;      //1s事件标志.
    
    if (g_uSys1msFlag == BOOL_TRUE)             //1ms消息.
    {
         g_uSys1msFlag =  BOOL_FALSE;
         sys_timer1ms();
    }
    if (g_bSysMsgTimer10ms == BOOL_TRUE)        //10ms消息.
    {
        g_bSysMsgTimer10ms = BOOL_FALSE;
        g_bSysTimer10msEventFlag = BOOL_TRUE;
        sys_timer10ms();
    }   
    if (g_bSysMsgTimer100ms == BOOL_TRUE)       //100ms消息.
    {
        g_bSysMsgTimer100ms = BOOL_FALSE;
        g_bSysTimer100msEventFlag = BOOL_TRUE;
        sys_timer100ms();
    }
    if (g_bSysMsgTimer1s == BOOL_TRUE)          //1s消息.
    {
        g_bSysMsgTimer1s = BOOL_FALSE;
        g_bSysTimer1sEventFlag = BOOL_TRUE;
        sys_timer1s();
    }
}

/***************************************************************************************************
函数:   系统定时1ms事件处理.
输入:   无.
返回:   无.
***************************************************************************************************/
void sys_timer1ms(void)
{
    timer_task_run();
}

/***************************************************************************************************
函数:   系统定时10ms事件处理.
输入:   无.
返回:   无.
***************************************************************************************************/
void sys_timer10ms(void)
{
    g_uSys10msEventCount++;
}
/***************************************************************************************************
函数:   系统定时100ms事件处理.
输入:   无.
返回:   无.
***************************************************************************************************/
void sys_timer100ms(void)
{
    g_uSys100msEventCount++;
}

/***************************************************************************************************
函数:   系统定时1s事件处理.
输入:   无.
返回:   无.
***************************************************************************************************/
void sys_timer1s(void)
{
    g_uSys1sEventCount++;
    timer1s_handle();
}


/***************************************************************************************************
函数:   .
输入:   无.
返回:   g_uSys1sCount.
***************************************************************************************************/
uint32_t get_poweron_times(void)
{
    return g_uSys1sEventCount;
}
/***************************************************************************************************
文件结束.
***************************************************************************************************/


