// 定时服务器使用FTM2

#include "drv_timer.h"

#include "drv_clock.h"
#include "drv_int.h"
#include "drv_irq_cfg.h"
#include <pfc_sync_objs.h>
#include "stm32f4xx_tim.h"

TDrvTimer g_drv_timer; /// 定时服务器对象


static TIM_TypeDef* g_timer_base_[] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8};

static UInt8   g_timer_irq_[] = {TIM1_CC_IRQn, TIM2_IRQn, TIM3_IRQn, TIM4_IRQn, TIM5_IRQn, \
                               TIM6_DAC_IRQn, TIM7_IRQn, TIM8_CC_IRQn};


/**
 * 中断处理程序. 用于在中断发生时调用用户中断事件
 * @param port: 发生中断的端口号
 * @return 
 */
static void TDrvTimerIrqHandler(TDrvTimer *self)
{
    /// 如果有自定义回调,触发回调
    if (self->MethodOnIrq != NULL)
        self->MethodOnIrq(self->recevier);
}
/**
 * 定时器3中断 
 */
void TIM3_IRQHandler(void)
{
    TDrvTimer *self = &g_drv_timer;
    
    TIM_TypeDef *base = g_timer_base_[self->timer_index_];
    TIM_ClearITPendingBit(base,TIM_IT_Update); 
    
    TDrvTimerIrqHandler(self);
}

static void TDrvTimerClockOpen(TDrvTimer *self, Byte index)
{
    switch(index)
    {
        case 0: RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;break;
        case 1: RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;break;
        case 2: RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;break;
        case 3: RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;break;
        case 4: RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;break;
        case 5: RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;break;
        case 6: RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;break;
        case 7: RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;break;
        case 8: RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;break;
        case 9: RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;break;
        case 10: RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;break;
        case 11: RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;break;
        case 12: RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;break;
        case 13: RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;break;
        case 14: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;break;
        default:
            break;
    }
}
/**
 * 时钟源关闭
 * @param self: 对象实例指针
 * @param index: 第几路 0起始
 * @return
 */
static void TDrvTimerClockClose(TDrvTimer *self, Byte index)
{
    switch(index)
    {
        case 0: RCC->APB2ENR &= ~RCC_APB2ENR_TIM1EN;break;
        case 1: RCC->APB1ENR &= ~RCC_APB1ENR_TIM2EN;break;
        case 2: RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;break;
        case 3: RCC->APB1ENR &= ~RCC_APB1ENR_TIM4EN;break;
        case 4: RCC->APB1ENR &= ~RCC_APB1ENR_TIM5EN;break;
        case 5: RCC->APB1ENR &= ~RCC_APB1ENR_TIM6EN;break;
        case 6: RCC->APB1ENR &= ~RCC_APB1ENR_TIM7EN;break;
        case 7: RCC->APB2ENR &= ~RCC_APB2ENR_TIM8EN;break;
        case 8: RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;break;
        case 9: RCC->APB2ENR &= ~RCC_APB2ENR_TIM9EN;break;
        case 10: RCC->APB2ENR &= ~RCC_APB2ENR_TIM10EN;break;
        case 11: RCC->APB2ENR &= ~RCC_APB2ENR_TIM11EN;break;
        case 12: RCC->APB1ENR &= ~RCC_APB1ENR_TIM12EN;break;
        case 13: RCC->APB1ENR &= ~RCC_APB1ENR_TIM13EN;break;
        case 14: RCC->APB1ENR &= ~RCC_APB1ENR_TIM14EN;break;
        default:
            break;
    }
}
/**
 * 销毁Dma通道
 * @param self: 对象实例指针
 * @return 
 */
static void TDrvTimerDestroy(TOfObject *self)
{
    TDrvTimer *this = (TDrvTimer *)self;
    this->Destroy_(self);
}

Bool TDrvTimerCreate(TDrvTimer *self)
{
    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvTimerDestroy);
    
    self->is_open_ = False;
    self->timer_index_ = 2; ///< 使用定时器3(需要自己计算)
    return True;
}

Bool TDrvTimerOpen(TDrvTimer *self)
{
    if (!self->is_open_)
    {
        TIM_TypeDef *base = g_timer_base_[self->timer_index_];
        TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   
        
        /// 打开时钟
        TDrvTimerClockOpen(self, self->timer_index_);
        
        TIM_TimeBaseStructure.TIM_Period = 15999;;                  ///<设置自动重装值+1,到了产生中断
        TIM_TimeBaseStructure.TIM_Prescaler = 5;                    ///< 时钟分频96(奇怪) / 6 = 16M
        TIM_TimeBaseStructure.TIM_ClockDivision = 1;                ///< 输入捕获时滤波用的参数,必须设一下
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   ///< 向上计数
        TIM_TimeBaseInit(base, &TIM_TimeBaseStructure);               //根据指定的参数初始

        /// 使能中断
        SysEnableIRQ(g_timer_irq_[self->timer_index_], kDrvIrqCfgPreeTimer, kDrvIrqCfgSubTimer);
        
        TIM_ClearFlag(base, TIM_FLAG_Update);                              //清除标志位，以免一启用中断后立即产生中断
        TIM_ARRPreloadConfig(base, DISABLE);                               //禁止ARR预装载缓冲器  
        TIM_ITConfig(base,TIM_IT_Update, ENABLE);                       //使能TIM3溢出中断
        TIM_Cmd(base, ENABLE);                                           //使能TIM3 总中断

        self->is_open_ = True;
    }
    return True;
}

Bool TDrvTimerClose(TDrvTimer *self)
{
    if (self->is_open_)
    {
        // // 关闭定时器中断
        SysDisableIRQ(g_timer_irq_[self->timer_index_]);
        
        TDrvTimerClockClose(self, self->timer_index_);
        self->is_open_ = False;
    }
    return True;
}




