#include "drv_dma.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"
#include "drv_dma_cfg.h"
#include "drv_dma_cfg.c"
//////////////////////////////////////////////////////////////////////////

static DMA_Stream_TypeDef *dma1_chs_base[] = {DMA1_Stream0, DMA1_Stream1, DMA1_Stream2, DMA1_Stream3, DMA1_Stream4, \
                                       DMA1_Stream5, DMA1_Stream6, DMA1_Stream7};
static DMA_Stream_TypeDef *dma2_chs_base[] = {DMA2_Stream0, DMA2_Stream1, DMA2_Stream2, DMA2_Stream3, DMA2_Stream4, \
                                       DMA2_Stream5, DMA2_Stream6, DMA2_Stream7};

/// 传输完成标记
const UInt32 g_dma_tcif_[] = {DMA_LISR_TCIF0, DMA_LISR_TCIF1, DMA_LISR_TCIF2, DMA_LISR_TCIF3, DMA_HISR_TCIF4, DMA_HISR_TCIF5, DMA_HISR_TCIF6, DMA_HISR_TCIF7};

/// 通道配置标记
static UInt32 g_dma_channel_num[] = {DMA_Channel_0, DMA_Channel_1, DMA_Channel_2, DMA_Channel_3, DMA_Channel_4, DMA_Channel_5, DMA_Channel_6, DMA_Channel_7};


TDrvDma g_drv_dma[2];  ///< DMA驱动对象

/**
 * 销毁Dma驱动
 * @param object: 对象实例指针
 * @return 
 */
static void TDrvDmaDestroy(TOfObject *object)
{
    TDrvDma *self = (TDrvDma *)object;

    TDrvDmaClose(self);

    self->Destroy_(object);
}

void TDrvDmaCreate(TDrvDma *self, Int32 index)
{
    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvDmaDestroy);

    // 成员初始化
    if (index == 0)
    {
        self->base_ = DMA1;
        self->chs_base_ = (void *)dma1_chs_base;
    }
    else if (index == 1)
    {
        self->base_ = DMA2;
        self->chs_base_ = (void *)dma2_chs_base;
    }
    self->index_ = index;
    self->is_open_ = False;
}

void TDrvDmaOpen( TDrvDma *self )
{
    if (!self->is_open_)
    {
        if (self->index_ == 0)
            RCC->AHB1ENR |= RCC_AHB1Periph_DMA1;
        else if (self->index_ == 1)
            RCC->AHB1ENR |= RCC_AHB1Periph_DMA2;

        self->is_open_ = True;
    }
}

void TDrvDmaClose( TDrvDma *self )
{
    if (self->is_open_)
    {
        if (self->index_ == 0)
            RCC->AHB1ENR &= ~RCC_AHB1Periph_DMA1;
        else if (self->index_ == 1)
            RCC->AHB1ENR &= ~RCC_AHB1Periph_DMA2;

        self->is_open_ = False;
    }
}

//////////////////////////////////////////////////////////////////////////


/**
 * 销毁Dma通道
 * @param self: 对象实例指针
 * @return 
 */
static void TDrvDmaChDestroy(TOfObject *object)
{
    TDrvDmaCh *self = (TDrvDmaCh *)object;

    TDrvDmaChClose(self);

    self->Destroy_(object);
}

void TDrvDmaChCreate(TDrvDmaCh *self, UInt32 index, TDrvDmaChCfg *chn_cfg)
{
    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvDmaChDestroy);

    self->index_ = index;
    
    self->chn_cfg_.buffer = chn_cfg->buffer;
    self->chn_cfg_.buffer_size = (chn_cfg->buffer_size / chn_cfg->data_len);
    self->chn_cfg_.data_len = chn_cfg->data_len;
    self->chn_cfg_.dir = chn_cfg->dir;
    self->chn_cfg_.peripheral_addr = chn_cfg->peripheral_addr;
}

void TDrvDmaChOpen(TDrvDmaCh *self)
{
    DMA_Stream_TypeDef *chn_base;
    if (self->index_ < (sizeof(g_drv_dma_cfgs) / sizeof(g_drv_dma_cfgs[0])))
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_DMA1, ENABLE);
        if (g_drv_dma_cfgs[self->index_].dma_idx == 0)
        {
            chn_base = dma1_chs_base[g_drv_dma_cfgs[self->index_].chn];
        }
        else if (g_drv_dma_cfgs[self->index_].dma_idx == 1)
        {
            chn_base = dma2_chs_base[g_drv_dma_cfgs[self->index_].chn];
        }
        DMA_InitTypeDef DMA_InitStructure;
        //DMA_DeInit(chn_base);
        /// 外设地址
        DMA_InitStructure.DMA_PeripheralBaseAddr = self->chn_cfg_.peripheral_addr;
        /// 缓存地址
        DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)(self->chn_cfg_.buffer);
        /// 如果是输出到外设
        if (self->chn_cfg_.dir)
            DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
        else
            DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;

        /// 缓存大小
        DMA_InitStructure.DMA_BufferSize = self->chn_cfg_.buffer_size;
        /// 外设寄存器不递增
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
        /// 缓存递增
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
        /// 单字节
        if (self->chn_cfg_.data_len == 1)
        {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
            DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
        }
        /// 双字节
        else if (self->chn_cfg_.data_len == 2)
        {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
            DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        }
        /// 4字节
        else if (self->chn_cfg_.data_len == 4)
        {
            DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
            DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
        }
        /// 正常非连续模式,F4循环模式,不然无效
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;
        /// 通道功能选择
        DMA_InitStructure.DMA_Channel = g_dma_channel_num[g_drv_dma_cfgs[self->index_].chn_item];
        
        DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
        DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
        DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        
        DMA_Init(chn_base, &DMA_InitStructure);
        DMA_Cmd(chn_base, ENABLE);
    }
    
}

void TDrvDmaChClose(TDrvDmaCh *self)
{
    DMA_Stream_TypeDef *chn_base;
    
    if (self->index_ < (sizeof(g_drv_dma_cfgs) / sizeof(g_drv_dma_cfgs[0])))
    {
        if (g_drv_dma_cfgs[self->index_].dma_idx == 0)
        {
            chn_base = dma1_chs_base[g_drv_dma_cfgs[self->index_].chn];
        }
        else if (g_drv_dma_cfgs[self->index_].dma_idx == 1)
        {
            chn_base = dma2_chs_base[g_drv_dma_cfgs[self->index_].chn];
        }
        DMA_DeInit(chn_base);
    }
}

void TDrvDmaChStart(TDrvDmaCh *self, Bool is_wait_finish)
{
    DMA_Stream_TypeDef *chn_base;
    DMA_TypeDef *dma_base;
    
    /// DMA暂时设置自动触发，不用软件触发
    if (self->index_ < (sizeof(g_drv_dma_cfgs) / sizeof(g_drv_dma_cfgs[0])))
    {
//        if (g_drv_dma_cfgs[self->index_].dma_idx == 0)
//        {
//            dma_base = DMA1;
//            chn_base = dma1_chs_base[g_drv_dma_cfgs[self->index_].chn];
//        }
//        else if (g_drv_dma_cfgs[self->index_].dma_idx == 1)
//        {
//            dma_base = DMA2;
//            chn_base = dma2_chs_base[g_drv_dma_cfgs[self->index_].chn];
//        }
//        
//        DMA_Cmd(chn_base, DISABLE);  
//        DMA_SetCurrDataCounter(chn_base, self->chn_cfg_.buffer_size);
//        DMA_Cmd(chn_base, ENABLE);
//        if (is_wait_finish)
//        {
//            if (g_drv_dma_cfgs[self->index_].chn < 4)
//            {
//                while ((dma_base->LISR & g_dma_tcif_[g_drv_dma_cfgs[self->index_].chn]) == 0);
//            }
//            else
//            {
//                while ((dma_base->HISR & g_dma_tcif_[g_drv_dma_cfgs[self->index_].chn]) == 0);
//            }
//        }
    }
}
