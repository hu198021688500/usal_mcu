#include "kfc_timer.h"
#include <pfc_sync_objs.h>
#include "drv_timer.h"


TKfcTimerSrv g_kfc_timer_srv;  // 定时服务器对象

//////////////////////////////////////////////////////////////////////////

/**
 * 销毁Dma通道
 * @param self: 对象实例指针
 * @return 
 */
static void TKfcTimerDestroy(TOfObject *self)
{
    TKfcTimer *this = (TKfcTimer *)self;
    this->Destroy_(self);
}

/**
 * 定时器构造函数
 * @param self: 对象实例指针
 * @param server: 定时管理器
 * @return 
 */
static void TKfcTimerCreate(TKfcTimer *self, TKfcTimerSrv *server)
{
    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TKfcTimerDestroy);

    self->enabled_ = False;
    self->interval_ = 10;
    self->tick_count_ = 0;
}

UInt32 TKfcTimerInterval(TKfcTimer *self )
{
    return self->interval_;
}

void TKfcTimerSetInterval(TKfcTimer *self, UInt32 ms )
{
    self->interval_ = ms;
}

void TKfcTimerStart(TKfcTimer *self )
{
    self->tick_count_ = 0;
    self->enabled_ = True;
}

void TKfcTimerStop(TKfcTimer *self )
{
    self->enabled_ = False;
}

//////////////////////////////////////////////////////////////////////////


static void TKfcTimerSrvIrq(void *receiver)
{
    Int32 i; 
    TKfcTimerSrv *self = (TKfcTimerSrv *)receiver;
    //g_timer_srv_tick++;

    for (i = self->count_ - 1; i >= 0; i--)
    {
        TKfcTimer *timer = &self->timers_[i];

        if (timer->enabled_)
        {
            timer->tick_count_++;

            if (timer->tick_count_ >= timer->interval_)
            {
                if (timer->MethodOnTimerIrq != NULL)
                    timer->MethodOnTimerIrq(timer->receiver);

                timer->tick_count_ = 0;
            }
        }
    }
}

/**
 * 销毁Dma驱动
 * @param self: 对象实例指针
 * @return 
 */
static void TKfcTimerSrvDestroy(TOfObject *object)
{
    TKfcTimerSrv *self = (TKfcTimerSrv *)object;

    TKfcTimerSrvClose(self);

    self->Destroy_(object);
}

void TKfcTimerSrvCreate(TKfcTimerSrv *self)
{
    Int32 i;

    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TKfcTimerSrvDestroy);

    // 成员初始化
    self->timer_index_ = 0;
    
    self->count_ = 0;
    self->index_ = 0;
    self->is_open_ = False;

    for (i = 0; i < KFC_MAX_TIMER_COUNT; i++)
        self->timers_[i].enabled_ = False;
    
    TDrvTimerCreate(&g_drv_timer);
    g_drv_timer.recevier = self;
    g_drv_timer.MethodOnIrq = TKfcTimerSrvIrq;
}

void TKfcTimerSrvOpen( TKfcTimerSrv *self )
{
    if (!self->is_open_)
    {
        /// 打开Drv定时器
        
        TDrvTimerOpen(&g_drv_timer);
        self->is_open_ = True;
    }
}

void TKfcTimerSrvClose( TKfcTimerSrv *self )
{
    if (self->is_open_)
    {
        Int32 i;
        
         for (i = 0; i < KFC_MAX_TIMER_COUNT; i++)
             self->timers_[i].enabled_ = False;
        /// 关闭Drv定时器
        
        TDrvTimerClose(&g_drv_timer);
        self->is_open_ = False;
    }
}

TKfcTimer * TKfcTimerSrvCreateTimer( TKfcTimerSrv *self )
{
    TKfcTimer *result = NULL;

    if (self->count_ < KFC_MAX_TIMER_COUNT)
    {
        Int32 index = PfInterlockedInc(&self->index_) - 1; // 用index_而不用count_的目的是为了防止并发操作时count_超过MAX_TIMER_COUNT

        if (index < KFC_MAX_TIMER_COUNT)
        {
            result = &self->timers_[index];
            TKfcTimerCreate(result, self);

            PfInterlockedAdd(&self->count_, 1);  // 定时器个数加1
        }
    }

    return result;
}
