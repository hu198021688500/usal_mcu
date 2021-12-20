#include "drv_uart.h"
#include "stm32f4xx.h"
#include "drv_int.h"
#include "drv_uart_cfg.h"
#include <ofc_string.h>
#include "drv_clock.h"
#include "drv_irq_cfg.h"

#include "drv_uart_cfg.c"
// 需要和uart_configs变量的个数对应
static void *g_uart_instances_[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
};

/**
 * 收到数据后的处理
 * @param self: 对象实例
 * @return
 */
static void TDrvUartOnReceived(TDrvUart *self)
{
    const TDrvUartCfg *config = self->cfg_;
    USART_TypeDef *base = config->uart_base;
    
    uint8_t data = (unsigned char)base->DR;
    
    TOfRingBufWrite(&self->rx_buf_, &data, 1);
    
    if (self->OnDataReceived != NULL)
        self->OnDataReceived(self);
}

/**
 * 中断处理
 * @param self: 对象实例
 * @return
 */
static void TDrvUartIRQHandle(TDrvUart *self)
{
    const TDrvUartCfg *config = self->cfg_;
    USART_TypeDef *base = config->uart_base;
    
    UInt32 state = base->SR;

    if (state & USART_SR_RXNE)
    {
        TDrvUartOnReceived(self);
    }
}
/**
 * 串口1中断
 * @param 
 * @return
 */
void USART1_IRQHandler(void)
{
    if (g_uart_instances_[0] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[0]);
}
/**
 * 串口2中断
 * @param 
 * @return
 */
void USART2_IRQHandler(void)
{
    if (g_uart_instances_[1] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[1]);
}
/**
 * 串口3中断
 * @param 
 * @return
 */
void USART3_IRQHandler(void)
{
    if (g_uart_instances_[2] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[2]);
}
/**
 * 串口4中断
 * @param 
 * @return
 */
void UART4_IRQHandler(void)
{
    if (g_uart_instances_[3] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[3]);
}
/**
 * 串口5中断
 * @param 
 * @return
 */
void UART5_IRQHandler(void)
{
    if (g_uart_instances_[4] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[4]);
}
/**
 * 串口6中断
 * @param 
 * @return
 */
void USART6_IRQHandler(void)
{
    if (g_uart_instances_[5] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[5]);
}
/**
 * 串口7中断
 * @param 
 * @return
 */
void UART7_IRQHandler(void)
{
    if (g_uart_instances_[6] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[6]);
}
/**
 * 串口8中断
 * @param 
 * @return
 */
void UART8_IRQHandler(void)
{
    if (g_uart_instances_[7] != NULL)
        TDrvUartIRQHandle(g_uart_instances_[7]);
}
/**
 * 根据串口base获取是第几路
 * @param self: 对象实例指针
 * @param uart_x: 串口寄存器地址
 * @return
 */
static Int32 TDrvUartIndexGet(TDrvUart *self, USART_TypeDef *uart_x)
{
    Int32 result = -1;
    if (uart_x == USART1)
    {
        result = 0;
    }
    else if (uart_x == USART2)
    {
        result = 1;
    }
    else if (uart_x == USART3)
    {
        result = 2;
    }
    else if (uart_x == UART4)
    {
        result = 3;
    }
    else if (uart_x == UART5)
    {
        result = 4;
    }
    else if (uart_x == USART6)
    {
        result = 5;
    }
    else if (uart_x == UART7)
    {
        result = 6;
    }
    else if (uart_x == UART8)
    {
        result = 7;
    }
    
    return result;
}

/**
 * 时钟源打开
 * @param self: 对象实例指针
 * @param uart_x: 串口寄存器地址
 * @return
 */
static Bool TDrvUartClockOpen( TDrvUart *self, USART_TypeDef *uart_x)
{
    if (uart_x == USART1)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    }
    else if (uart_x == USART2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if (uart_x == USART3)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    }
    else if (uart_x == UART4)
    {
        RCC->APB1ENR |= RCC_APB1ENR_UART4EN;
    }
    else if (uart_x == UART5)
    {
        RCC->APB1ENR |= RCC_APB1ENR_UART5EN;
    }
    else if (uart_x == USART6)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    }
    else if (uart_x == UART7)
    {
        RCC->APB1ENR |= RCC_APB1ENR_UART7EN;
    }
    else if (uart_x == UART8)
    {
        RCC->APB1ENR |= RCC_APB1ENR_UART8EN;
    }
    else
        return False;
    
    return True;
}
/**
 * 时钟源关闭
 * @param self: 对象实例指针
 * @param uart_x: 串口寄存器地址
 * @return
 */
static Bool TDrvUartClockClose( TDrvUart *self, USART_TypeDef *uart_x)
{
    if (uart_x == USART1)
    {
        RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
    }
    else if (uart_x == USART2)
    {
        RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
    }
    else if (uart_x == USART3)
    {
        RCC->APB2ENR &= ~RCC_APB1ENR_USART3EN;
    }
    else if (uart_x == UART4)
    {
        RCC->APB1ENR &= ~RCC_APB1ENR_UART4EN;
    }
    else if (uart_x == UART5)
    {
        RCC->APB1ENR &= ~RCC_APB1ENR_UART5EN;
    }
    else if (uart_x == USART6)
    {
        RCC->APB2ENR &= ~RCC_APB2ENR_USART6EN;
    }
    else if (uart_x == UART7)
    {
        RCC->APB1ENR &= ~RCC_APB1ENR_UART7EN;
    }
    else if (uart_x == UART8)
    {
        RCC->APB1ENR &= ~RCC_APB1ENR_UART8EN;
    }
    else
        return False;
    
    return True;
}

static void TDrvUartSetBaud(TDrvUart *self, Int32 baudrate)
{
    const TDrvUartCfg *config = self->cfg_;
    USART_TypeDef *base = config->uart_base;
    TDrvClockFrequency clock_frequency;
    UInt32 div = 0x00;
    UInt32 p_clock = 0;
    UInt32 tmpreg = 0;
    UInt32 fractionaldivider = 0x00;
    TDrvClockGetFrequency(&g_drv_clock, &clock_frequency);
    if (base == USART1 || base == USART6)
    {
        p_clock = clock_frequency.pclk2_frequency;
    }
    else
        p_clock = clock_frequency.pclk1_frequency;
    if ((base->CR1 & 0x8000) != 0)
    {
        div = ((25 * p_clock) / (2 * (baudrate)));    
    }
    else
    {
        div = ((25 * p_clock) / (4 * (baudrate)));    
    }
    tmpreg = (div / 100) << 4;

    fractionaldivider = div - (100 * (tmpreg >> 4));
    if ((base->CR1 & 0x8000) != 0)
    {
        tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
    }
    else
    {
        tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
    }
    base->BRR = (uint16_t)tmpreg;
}


static void TDrvUartDestroy(TOfObject *object)
{
    TDrvUart *self = (TDrvUart *)object;

    TDrvUartClose(self);

    OfDestroyObject((TOfObject *)&self->rx_buf_);

    self->Destroy_(object);
}

void TDrvUartCreate(TDrvUart *self, Int32 index, void *rx_buf, Int32 rx_buf_size, TDrvUartMode mode)
{
    if (index >= 0 && index < (sizeof(g_drv_uart_cfgs) / sizeof(g_drv_uart_cfgs[0])) && g_drv_uart_cfgs[index].uart_base != NULL)
    {
        TOfObjectCreate((TOfObject *)self);
        OVERRIDE_METHOD(TOfObject, Destroy, TDrvUartDestroy);

        self->cfg_ = &g_drv_uart_cfgs[index];
        
        const TDrvUartCfg *config = self->cfg_;
        USART_TypeDef *base = config->uart_base;
        Int32 uart_idx;
        
        self->receiver = NULL;
        self->OnDataReceived = NULL;
        self->OnDataSent = NULL;
        self->mode_ = mode;

        self->index_ = index;
        /// 根据串口base获取是第几路
        uart_idx = TDrvUartIndexGet(self, base);
        if (uart_idx >= 0)
            g_uart_instances_[uart_idx] = (void *)self;

        self->is_open_ = False;
        self->fifo_size_ = 0;

        TOfRingBufCreate(&self->rx_buf_, rx_buf, rx_buf_size);
    }
}

void TDrvUartOpen( TDrvUart *self, Int32 baudrate, TDrvUartParity parity)
{
    if (!self->is_open_)
    {
        const TDrvUartCfg *config = self->cfg_;
        USART_TypeDef *base = config->uart_base;
        UInt32 temp_reg;

        TDrvUartClockOpen(self, base);
        
        // 设置引脚为串口
        TDrvPortPinConfig pin_config;
        pin_config.mux = config->pin_mux;
        pin_config.pull = kPortPullDisabled;
        pin_config.strength = kPortStrengthLow;
        TDrvPortPinOpen(config->tx_port, config->tx_pin, True, &pin_config);
        
        TDrvPortPinOpen(config->rx_port, config->rx_pin, False, &pin_config);
        // 配置RTS引脚
        if (self->mode_ == kDrvUartMode485)
        {
            TDrvPortPinConfig pin_config;
            pin_config.mux = config->pin_rts_mux;
            pin_config.pull = kPortPullUp;
            pin_config.strength = kPortStrengthLow;
            TDrvPortPinOpen(config->rts_port, config->rts_pin, True, &pin_config);
            
            TDrvPortPinSet(config->rts_port, config->rts_pin, False);
        }

        self->fifo_size_ = 1;
        
        /// CR2
        /// 默认配置停止位为1个停止位
        base->CR2 &= ~USART_CR2_STOP;

        /// CR1
        temp_reg = 0;//base->CR1;
        /// temp_reg &= 0xE9F3;
        ///< 接收, 发送使能, 接收为空中断,串口使能
        temp_reg |= USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;
        /// 有奇偶校验数据位为9位
        if (parity == kDrvUartParityNone)
        {
            temp_reg &= ~(USART_CR1_M | USART_CR1_PCE);
        }
        else if (parity == kDrvUartParityEven)
        {
            temp_reg |= USART_CR1_M | USART_CR1_PCE;
            temp_reg &= ~USART_CR1_PS;
        }
        else if (parity == kDrvUartParityOdd)
        {
            temp_reg |= USART_CR1_M | USART_CR1_PCE;
            temp_reg |= USART_CR1_PS;
        }
        
        /// 波特率设置
        TDrvUartSetBaud(self, baudrate);
        
        base->CR1 = temp_reg;
        // /// 状态寄存器清零
        base->SR = 0x00;
        
        /// CR3
        temp_reg = base->CR3;
        temp_reg &= 0xFCFF;
//        if (self->mode_ == kDrvUartMode485)
//            temp_reg |= USART_CR3_RTSE;
        base->CR3 = (uint16_t)temp_reg;
        
        if (self->mode_ == kDrvUartMode485)
        {
            SysEnableIRQ((IRQn_Type)config->interrupt_irqn, kDrvIrqCfgPreeRs485, kDrvIrqCfgSubRs485);
        }
        else
        {
            SysEnableIRQ((IRQn_Type)config->interrupt_irqn, kDrvIrqCfgPreeUart, kDrvIrqCfgSubUart);
        }
        self->is_open_ = True;
    }
}


void TDrvUartClose( TDrvUart *self )
{
    if (self->is_open_)
    {
        const TDrvUartCfg *config = self->cfg_;
        USART_TypeDef *base = config->uart_base;

        //close 时需要等待关闭后才能关闭时钟
        SysDisableIRQ((IRQn_Type)config->interrupt_irqn);

        // 关闭收发器,并关闭所有中断
        base->CR1 = 0;
        base->CR2 = 0;
        base->CR3 = 0;

        // 禁用引脚,关闭时钟
        if (self->mode_ == kDrvUartMode485)
            TDrvPortPinClose(config->rts_port, config->rts_pin);

        TDrvPortPinClose(config->rx_port, config->rx_pin);
        TDrvPortPinClose(config->tx_port, config->tx_pin);
        TDrvUartClockClose(self, base);
        
        self->is_open_ = False;
    }
}

Int32 TDrvUartRead( TDrvUart *self, void *buf, Int32 size)
{
    Int32 result = 0;

    if (buf != NULL && size > 0)
    {
        result = TOfRingBufDataSize(&self->rx_buf_);
        if (result > 0)
        {
            if (result > size)
               result = size;

            TOfRingBufRead(&self->rx_buf_, (void *)buf, result);
        }
    }

    return result;
}


Int32 TDrvUartWrite(TDrvUart *self, const void *data, Int32 size)
{
    Int32 result = 0;

    if (data != NULL && size > 0)
    {
        const TDrvUartCfg *config = self->cfg_;
        USART_TypeDef *base = config->uart_base;
        Int32 i;
        if (self->mode_ == kDrvUartMode485)
        {
            TDrvPortPinSet(config->rts_port, config->rts_pin, True);
        }
        for (i = 0; i < size; i++)
        {
            base->DR = ((const UInt8 *)data)[i];
            /// 等待发送完成
            while(!(base->SR & USART_SR_TC));
        }
        if (self->mode_ == kDrvUartMode485)
        {
            TDrvPortPinSet(config->rts_port, config->rts_pin, False);
        }
    }
    return result;
}

Bool TDrvUartIsOpen(TDrvUart *self)
{
    return self->is_open_; 
}

