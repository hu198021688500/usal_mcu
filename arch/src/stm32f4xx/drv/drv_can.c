#include "drv_can.h"
#include "stm32f4xx.h"
#include "drv_int.h"
#include "drv_can_cfg.h"
#include "stm32f4xx_can.h"
#include "string.h"
#include "drv_clock.h"
#include "drv_irq_cfg.h"

#include "drv_can_cfg.c"

// 需要和uart_configs变量的个数对应
static void *g_can_instances_[] =
{
    NULL,
    NULL,
    NULL,
};

/* 
    CAN 波特率 = RCC_APB1Periph_CAN1 / Prescaler / (SJW + BS1 + BS2);	
    CAN 波特率 = 420000000 / 2 / (1 + 12 + 8) / = 1 Mbps		
*/
typedef struct TDrvCanBaudCfgTag TDrvCanBaudCfg;
struct TDrvCanBaudCfgTag
{
    UInt8 brp;      ///< 预分频配置
    UInt8 swj;      ///< 同步跳转宽度 ///< 0~3
    UInt8 ts1;      ///< 时间段1   ///< 0~15
    UInt8 ts2;      ///< 时间段2   ///< 0~7
};
///// 波特率具体配置(对应42M频率)
//static TDrvCanBaudCfg g_drv_can_baud_cfg[kCanBaudRateMax] = 
//{
//    {8,  0, 11, 7},              ///< 250K
//    {4,  0, 11, 7},              ///< 500K
//    {2,  0, 11, 7},              ///< 1M
//    {20, 0, 11, 7},              ///< 100K
//    {16, 0, 11, 7},              ///< 125K
//};
/// 波特率具体配置(对应48M频率)
static TDrvCanBaudCfg g_drv_can_baud_cfg[kCanBaudRateMax] = 
{
    {8,  0, 14, 7},              ///< 250K
    {4,  0, 14, 7},              ///< 500K
    {2,  0, 14, 7},              ///< 1M
    {20, 0, 14, 7},              ///< 100K
    {16, 0, 14, 7},              ///< 125K
};
/**
 * CAN执行发送
 * @param self: 对象实例
 * @return
 */
void TDrvCanOnSend(TDrvCan *self, CAN_TypeDef *base)
{
    TCanData data;
    
    if (TOfRingBufRead(&self->tx_buf_, &data, sizeof(data)) > 0)
    {
        CanTxMsg send_msg;
        send_msg.RTR = data.frame_type ? CAN_RTR_Remote : CAN_RTR_Data;
        send_msg.IDE = data.id_type ? CAN_ID_EXT : CAN_ID_STD;
        if (send_msg.IDE == CAN_ID_STD)
            send_msg.StdId = data.id;
        else
            send_msg.ExtId = data.id;
        send_msg.DLC = data.length;
        memcpy(&send_msg.Data[0], &data.data[0], send_msg.DLC);
        
        CAN_Transmit(base, &send_msg);
    }
}
/**
 * 收到CAN报文后的处理
 * @param self: 对象实例
 * @param rec_msg:接收到的数据
 * @return
 */
static void TDrvCanOnRecv(TDrvCan *self, CanRxMsg *rec_msg)
{
    TCanData msg;
   
    
    msg.bus_index = self->index_;
    
    if (rec_msg->IDE == CAN_ID_STD)
    {
        msg.id_type = 0;
        msg.id = rec_msg->StdId & 0x1FFFFFFF;
    }
    else
    {
        msg.id_type = 1;
        msg.id = rec_msg->ExtId & 0x1FFFFFFF;
    }
    /// 如果是远程帧
    if (rec_msg->RTR == CAN_RTR_Remote)
        msg.frame_type = 1;
    else 
        msg.frame_type = 0;
    
    msg.length = rec_msg->DLC;
    memcpy(&msg.data[0], &rec_msg->Data[0], msg.length);
    /// 接收tick
    msg.param.recv_tick = 0;
    

    TOfRingBufWrite(&self->rx_buf_, &msg, sizeof(msg));
}
/**
 * 中断处理
 * @param self: 对象实例
 * @return
 */
static void TDrvCanIRQHandle(TDrvCan *self, Bool is_send)
{
    const TDrvCanCfg *config = self->cfg_;
    CAN_TypeDef *base = config->can_base;
    /// 发送
    if (is_send)
    {
        if (TOfRingBufDataSize(&self->tx_buf_) <= 0)
            /// 关闭发送中断
            CAN_ITConfig(base, CAN_IT_TME, DISABLE);
        else
            TDrvCanOnSend(self, base);
    }
    /// 接收
    else
    {
        CanRxMsg rec_msg;
        UInt8 can_pend;
        Byte rec_fifo = CAN_FIFO0;
        if (base == CAN2)
            rec_fifo = CAN_FIFO1;
        can_pend = CAN_MessagePending(base, rec_fifo);
        while(can_pend > 0)
        {
            CAN_Receive(base, rec_fifo, &rec_msg);
            TDrvCanOnRecv(self, &rec_msg);
            can_pend--;
        }

    }
}
/// CAN1接收中断
void CAN1_RX0_IRQHandler(void)
{
    if (g_can_instances_[0] != NULL)
        TDrvCanIRQHandle(g_can_instances_[0], False);   
}
/// CAN2接收中断
void CAN2_RX1_IRQHandler(void)
{
    if (g_can_instances_[1] != NULL)
        TDrvCanIRQHandle(g_can_instances_[1], False);  
}
/// CAN3接收中断
void CAN3_RX0_IRQHandler(void)
{
    if (g_can_instances_[2] != NULL)
        TDrvCanIRQHandle(g_can_instances_[2], False);  
}

/// CAN1发送中断
void CAN1_TX_IRQHandler(void)
{
    if (g_can_instances_[0] != NULL)
        TDrvCanIRQHandle(g_can_instances_[0], True);   
}
/// CAN2发送中断
void CAN2_TX_IRQHandler(void)
{
    if (g_can_instances_[1] != NULL)
        TDrvCanIRQHandle(g_can_instances_[1], True); 
}
/// CAN3发送中断
void CAN3_TX_IRQHandler(void)
{
    if (g_can_instances_[2] != NULL)
        TDrvCanIRQHandle(g_can_instances_[2], True); 
}




/**
 * 根据串口base获取是第几路
 * @param self: 对象实例指针
 * @param uart_x: 串口寄存器地址
 * @return
 */
static Int32 TDrvCanIndexGet(TDrvCan *self, CAN_TypeDef *can_x)
{
    Int32 result = -1;
    if (can_x == CAN1)
    {
        result = 0;
    }
    else if (can_x == CAN2)
    {
        result = 1;
    }
    else if (can_x == CAN3)
    {
        result = 2;
    }
    return result;
}

/**
 * 时钟源打开
 * @param self: 对象实例指针
 * @param uart_x: 串口寄存器地址
 * @return
 */
static void TDrvCanClockOpen(TDrvCan *self, Byte index)
{
    if (index == 0)
        RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
    else if (index == 1)
        RCC->APB1ENR |= RCC_APB1ENR_CAN2EN;
    else if (index == 2)
        RCC->APB1ENR |= RCC_APB1ENR_CAN3EN;
}

/**
 * CAN驱动销毁
 * @param object: 对象实例
 * @return
 */
static void TDrvCanDestroy(TOfObject *object)
{
    TDrvCan *self = (TDrvCan *)object;
    TDrvCanClose(self);
    
    OfDestroyObject((TOfObject *)&self->tx_buf_);
    OfDestroyObject((TOfObject *)&self->rx_buf_);
    self->Destroy_(object);
}

void TDrvCanCreate(TDrvCan *self, Int32 index, void *tx_buf, Int32 tx_buf_size, void *rx_buf, Int32 rx_buf_size)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvCanDestroy);
    
    self->index_ = index;
    self->cfg_ = &g_drv_can_cfgs[index];
    
    const TDrvCanCfg *config = self->cfg_;
    CAN_TypeDef *base = config->can_base;
    Int32 can_idx;
    
    /// 根据can base获取是第几路
    can_idx = TDrvCanIndexGet(self, base);
    if (can_idx >= 0)
        g_can_instances_[can_idx] = (void *)self;
    
    TOfRingBufCreate(&self->tx_buf_, tx_buf, tx_buf_size);
    TOfRingBufCreate(&self->rx_buf_, rx_buf, rx_buf_size);
    
    self->is_open_ = False;
}

Bool TDrvCanOpen(TDrvCan *self, TDrvCanBaudRate baud_index)
{
    if (!self->is_open_)
     { 
        CAN_InitTypeDef        CAN_InitStructure;
        CAN_FilterInitTypeDef  CAN_FilterInitStructure;
             
        const TDrvCanCfg *config = self->cfg_;
        CAN_TypeDef *base = config->can_base;
         
        TDrvCanClockOpen(self, self->index_);
        
        /// 设置引脚为can复用
        TDrvPortPinConfig pin_config;
        pin_config.mux = config->pin_mux;
        pin_config.pull = kPortPullDisabled;
        pin_config.strength = kPortStrengthLow;
        TDrvPortPinOpen(config->can_tx_port, config->can_tx_pin, True, &pin_config);
        
        TDrvPortPinOpen(config->can_rx_port, config->can_rx_pin, False, &pin_config);
         
        CAN_DeInit(base);
         
        CAN_InitStructure.CAN_TTCM = DISABLE;			/* 禁止时间触发模式（不生成时间戳), T  */
        CAN_InitStructure.CAN_ABOM = ENABLE;			/* 启用自动总线关闭管理 */
        CAN_InitStructure.CAN_AWUM = DISABLE;			/* 禁止自动唤醒模式 */
        CAN_InitStructure.CAN_NART = DISABLE;			/* 禁止仲裁丢失或出错后的自动重传功能 */
        CAN_InitStructure.CAN_RFLM = DISABLE;			/* 禁止接收FIFO加锁模式 */
        CAN_InitStructure.CAN_TXFP = DISABLE;			/* 禁止传输FIFO优先级 */
        CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	/* 设置CAN为正常工作模式 */
        
        CAN_InitStructure.CAN_SJW = g_drv_can_baud_cfg[baud_index].swj;
        CAN_InitStructure.CAN_BS1 = g_drv_can_baud_cfg[baud_index].ts1;
        CAN_InitStructure.CAN_BS2 = g_drv_can_baud_cfg[baud_index].ts2;
        CAN_InitStructure.CAN_Prescaler = g_drv_can_baud_cfg[baud_index].brp;
        CAN_Init(base, &CAN_InitStructure);
        
        if (base == CAN3)
        {
            CAN_FilterInitStructure.CAN_FilterNumber = 0;		/*  筛选器序号，0-13，共14个滤波器 */
            CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		/* 筛选器绑定FIFO 0 */
            CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		/* 筛选器模式，设置ID掩码模式 */
            CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	/* 32位筛选器 */
            CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					/* 掩码后ID的高16bit */
            CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					/* 掩码后ID的低16bit */
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				/* ID掩码值高16bit */
            CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				/* ID掩码值低16bit */
            CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				/* 使能筛选器 */
            CAN3_FilterInit(&CAN_FilterInitStructure);
        }
        else 
        {
            if (base == CAN1)
            {
                CAN_FilterInitStructure.CAN_FilterNumber = 0;		/*  筛选器序号，0-13，共14个滤波器 */
                CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;		/* 筛选器绑定FIFO 0 */
            }
            else if (base == CAN2)
            {
                CAN_FilterInitStructure.CAN_FilterNumber = 14;						/* 筛选器序号，14-27，共14个滤波器 */
                CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO1;		/*  筛选器绑定FIFO 1 */
            }
            CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;		/* 筛选器模式，设置ID掩码模式 */
            CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	/* 32位筛选器 */
            CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					/* 掩码后ID的高16bit */
            CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;					/* 掩码后ID的低16bit */
            CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;				/* ID掩码值高16bit */
            CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				/* ID掩码值低16bit */
            CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;				/* 使能筛选器 */
            CAN_FilterInit(&CAN_FilterInitStructure);
        }
        if (base == CAN1)
        {
            CAN_ITConfig(base, CAN_IT_FMP0, ENABLE);
            //SysEnableIRQ(CAN1_TX_IRQn, kDrvIrqCfgPreeCanTx0, kDrvIrqCfgSubCanTx0);
            
            SysEnableIRQ((IRQn_Type)config->interrupt_irqn, kDrvIrqCfgPreeCanRx0, kDrvIrqCfgSubCanRx0);
        }
        else if (base == CAN2)
        {
            CAN_ITConfig(base, CAN_IT_FMP1, ENABLE);
            //SysEnableIRQ(CAN2_TX_IRQn, kDrvIrqCfgPreeCanTx1, kDrvIrqCfgSubCanTx1);
            
            SysEnableIRQ((IRQn_Type)config->interrupt_irqn, kDrvIrqCfgPreeCanRx1, kDrvIrqCfgSubCanRx1);
        }
        else if (base == CAN3)
        {
            CAN_ITConfig(base, CAN_IT_FMP0, ENABLE);
            SysEnableIRQ((IRQn_Type)config->interrupt_irqn, kDrvIrqCfgPreeCanRx2, kDrvIrqCfgSubCanRx2);
        }
        
        self->is_open_ = True;
     }

    return self->is_open_;
}

void TDrvCanClose(TDrvCan *self)
{
    if (self->is_open_)
    {
        self->is_open_ = False;
    }
}

Int32 TDrvCanRead(TDrvCan *self, TCanData *buf, Int32 count)
{
    Int32 result = TOfRingBufDataSize(&self->rx_buf_);
    if (result > 0)
    {
        result /= sizeof(*buf);
        if (result > count)
           result = count;

        TOfRingBufRead(&self->rx_buf_, (void *)buf, result * sizeof(*buf));
    }
    return result;
}


Bool TDrvCanWrite(TDrvCan *self, const TCanData *data)
{
    Bool result = False;
    UInt8 send_res = 0;
    UInt32 send_cnt = 0;
    UInt32 wait_cnt = 0;
/// 使用缓存暂时有问题
//    Int32 size = TOfRingBufFreeSize(&self->tx_buf_);
//    if (size > 0)
//    {
//        const TDrvCanCfg *config = self->cfg_;
//        CAN_TypeDef *base = config->can_base;

//        TOfRingBufWrite(&self->tx_buf_, (void *)data, sizeof(*data));
//        
//        TDrvCanOnSend(self, base);
//        /// 打开发送中断
//        //CAN_ITConfig(base, CAN_IT_TME, ENABLE);
//    }
    
    const TDrvCanCfg *config = self->cfg_;
    CAN_TypeDef *base = config->can_base;
    CanTxMsg send_msg;
    
    send_msg.RTR = data->frame_type ? CAN_RTR_Remote : CAN_RTR_Data;
    send_msg.IDE = data->id_type ? CAN_ID_EXT : CAN_ID_STD;
    if (send_msg.IDE == CAN_ID_STD)
        send_msg.StdId = data->id;
    else
        send_msg.ExtId = data->id;
    send_msg.DLC = data->length;
    memcpy(&send_msg.Data[0], &data->data[0], send_msg.DLC);
    
    
    while(send_cnt++ < 100)
    {
        send_res = CAN_Transmit(base, &send_msg);
        if (send_res != CAN_TxStatus_NoMailBox)
        {
            /// 等待发送完成
            while(wait_cnt++ < 1000)
            {
                if (CAN_TransmitStatus(base, send_res) == CAN_TxStatus_Ok)
                {
                    result = True;
                    break;
                }
            }
            break;
        }
    }
    return result;
}
