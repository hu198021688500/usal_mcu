#include "drv_ext_int.h"
//#include "stm32f413xx.h"
#include "stm32f4xx.h"
#include "drv_int.h"
#include "drv_irq_cfg.h"

enum
{
    kDrvExtiMaxCnt          = 16,       ///< 每组pin个数
};

/// 存放端口的中断链表
static TDrvExtInt *g_drv_ext_ints_[kDrvExtiMaxCnt] = {NULL, NULL, NULL, NULL, NULL,
                                          NULL, NULL, NULL, NULL, NULL,
                                          NULL, NULL, NULL, NULL, NULL,
                                          NULL};

/// 中断序号
static UInt32 g_irq_[kDrvExtiMaxCnt] = {EXTI0_IRQn, EXTI1_IRQn, EXTI2_IRQn, EXTI3_IRQn, EXTI4_IRQn, 
                             EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn, EXTI9_5_IRQn,
                             EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn, EXTI15_10_IRQn,
                             EXTI15_10_IRQn};

/**
 * 中断处理程序. 用于在中断发生时调用用户中断事件
 * @param port: 发生中断的端口号
 * @return 
 */
static void IrqHandler(TDrvExtInt *self)
{
    /// 如果有自定义回调,触发回调
    if (self->MethodOnIrq != NULL)
        self->MethodOnIrq(self->recevier);
}
/**
 * EXTI-0中断 
 */
void EXTI0_IRQHandler(void)
{
    UInt8 exti = 0;
    if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        IrqHandler(g_drv_ext_ints_[exti]);
    EXTI->PR = (1 << exti);
}
/**
 * EXTI-1中断 
 */
void EXTI1_IRQHandler(void)
{
    UInt8 exti = 1;
    if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        IrqHandler(g_drv_ext_ints_[exti]);
    EXTI->PR = (1 << exti);
}
/**
 * EXTI-2中断 
 */
void EXTI2_IRQHandler(void)
{
    UInt8 exti = 2;
    if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        IrqHandler(g_drv_ext_ints_[exti]);
    EXTI->PR = (1 << exti);
}
/**
 * EXTI-3中断 
 */
void EXTI3_IRQHandler(void)
{
    UInt8 exti = 3;
    if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        IrqHandler(g_drv_ext_ints_[exti]);
    EXTI->PR = (1 << exti);
}
/**
 * EXTI-4中断 
 */
void EXTI4_IRQHandler(void)
{
    UInt8 exti = 4;
    if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        IrqHandler(g_drv_ext_ints_[exti]);
    EXTI->PR = (1 << exti);  
}
/**
 * EXTI-5~9中断 
 */
void EXTI9_5_IRQHandler(void)
{
    UInt8 exti = 5;
    while(exti <= 9)
    {
        if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        {
            IrqHandler(g_drv_ext_ints_[exti]);
            EXTI->PR = (1 << exti);
        }
        exti++;
    }
}
/**
 * EXTI-10~15中断 
 */
void EXTI15_10_IRQHandler(void)
{
    UInt8 exti =10;
    while(exti <= 15)
    {
        if (g_drv_ext_ints_[exti] != NULL && EXTI->PR & (1 << exti))
        {
            IrqHandler(g_drv_ext_ints_[exti]);
            EXTI->PR = (1 << exti);
        }
        exti++;
    }
}

/**
 * 注册引脚中断到端口中断
 * @param self: 对象实例
 * @return 
 */
static void TDrvExtIntRegister(TDrvExtInt *self)
{
    g_drv_ext_ints_[self->pin_] = self;
}

/**
 * 撤销引脚中断到端口中断的注册
 * @param self: 对象实例
 * @return 
 */
static void TDrvExtIntUnRegister(TDrvExtInt *self)
{
    g_drv_ext_ints_[self->pin_] = NULL;
}
/**
 * 析构函数
 * @param object: 对象实例
 * @return 
 */
static void TDrvExtIntDestroy(TOfObject *object)
{
    TDrvExtInt *self = (TDrvExtInt *)object;

    TDrvExtIntSetTrigerMode(self, kDrvExtIntDisable);

    TDrvExtIntUnRegister(self);

    self->Destroy_(object);
}

void TDrvExtIntCreate(TDrvExtInt *self, TDrvPortNum port, Byte pin)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvExtIntDestroy);

    self->port_ = port;
    self->pin_ = pin;
    self->next_ = NULL;
    self->MethodOnIrq = NULL;
    self->recevier = NULL;
    //RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    TDrvExtIntSetTrigerMode(self, kDrvExtIntDisable);

    TDrvExtIntRegister(self);
}

void TDrvExtIntSetTrigerMode(TDrvExtInt *self, TDrvExtIntTriggerMode mode)
{
    if (mode <= kDrvExtIntBoth)
    {
        if (mode == kDrvExtIntDisable)
        {
            Bool disable = True;
            /// 关闭中断
            EXTI->IMR &= ~(1 << self->pin_);
            /// 关闭事件
            EXTI->EMR &= ~(1 << self->pin_);
            /// 关闭上升沿使能
            EXTI->RTSR &= ~(1 << self->pin_);
            /// 关闭下降沿使能
            EXTI->FTSR &= ~(1 << self->pin_);
            
            /// 判断Exti5~9是否全部关闭
            if (self->pin_ >= 5 && self->pin_ <= 9)
            {
                if (EXTI->IMR & 0x03E0)
                    disable = False;
            }
            /// 判断EXti10~15是否全部关闭
            else if (self->pin_ >= 10)
            {
                if (EXTI->IMR & 0xFC00)
                    disable = False;
            }
            if (disable)
                SysDisableIRQ(self->pin_);
        }
        else
        {
            /// EXTI中断设置为该端口引脚
            uint32_t tmp = 0x00;
            /// 系统配置时钟打开
            tmp = RCC->APB2ENR;
            tmp |= RCC_APB2ENR_SYSCFGEN;
            RCC->APB2ENR = tmp;

            tmp = SYSCFG->EXTICR[self->pin_ >> 2];
            tmp &= ~(((uint32_t)0x0F) << (4 * (self->pin_ & 0x03)));
            tmp |= ((uint32_t)(self->port_) << (4 * (self->pin_ & 0x03)));
            SYSCFG->EXTICR[self->pin_ >> 2] = tmp;

            /// GPIO设置为输入
            TDrvPortPinOpen(&g_drv_port[self->port_], self->pin_, False, NULL);
            
            /// 打开中断，关闭事件
            EXTI->IMR |= 1 << self->pin_;
            EXTI->EMR &= ~(1 << self->pin_);
            
            if (mode == kDrvExtIntRising)
            {
                EXTI->RTSR |= 1 << self->pin_;
                EXTI->FTSR &= ~(1 << self->pin_);
                
            }
            else if (mode == kDrvExtIntFalling)
            {
                EXTI->RTSR &= ~(1 << self->pin_);
                EXTI->FTSR |= 1 << self->pin_;
            }
            /// 双边沿
            else
            {
                EXTI->RTSR |= 1 << self->pin_;
                EXTI->FTSR |= 1 << self->pin_;
            }
            
            SysEnableIRQ(g_irq_[self->pin_], kDrvIrqCfgPreeExitInt, kDrvIrqCfgSubExitInt);
        }
    }
}


