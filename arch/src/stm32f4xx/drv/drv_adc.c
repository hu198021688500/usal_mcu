#include "drv_adc.h"
#include "stm32f4xx.h"
#include "drv_adc_cfg.h"
#include "stm32f4xx_adc.h"

#include "drv_adc_cfg.c"

typedef struct TDrvAdcChipTag TDrvAdcChip;
struct TDrvAdcChipTag
{
    ADC_TypeDef *base;
    TDrvDmaCh    dma_work;
    Int32        dma_chn;
    Int32        chn_used;
    UInt16       dma_buffer[17];
};

static TDrvAdcChip g_adc_chip_[3] = 
{
    [0] = {
        .base       =     ADC1,
        .dma_chn    = 0,
        .chn_used   = 0,
    },
    [1] = {
        .base       =     ADC2,
        .dma_chn    = 1,
        .chn_used   = 0,
    },    
    [2] = {
        .base       =     ADC3,
        .dma_chn    = 2,
        .chn_used   = 0,
    },
};

static Byte    g_index_rule_chn_[17];  ///< 保存对应index的规则组位置序号
/// ADC初始化一次
static UInt32 g_adc_init = 0;

/**
 * adc时钟配置
 * @param self: 对象实例指针
 * @param index: 索引0, 1
 * @param is_open: True:打开 False:关闭
 * @return
 */
static void TDrvAdcRccCtl(TDrvAdc *self, Int32 index, Bool is_open)
{
//    UInt32 temp_reg;
    if (is_open)
    {
        if (index == 0)
            RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
        else if (index == 1)
            RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;
        else if (index == 2)
            RCC->APB2ENR |= RCC_APB2ENR_ADC3EN;
            
//        // 时钟分频,不能大于14MHz
//        temp_reg = RCC->CFGR;
//        temp_reg &= ~RCC_CFGR_ADCPRE;
//        temp_reg |= RCC_CFGR_ADCPRE_DIV6;
//        RCC->CFGR = temp_reg;
        
        ADC_CommonInitTypeDef ADC_CommonInitStructure;
        ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
        ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
        ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
        ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
        ADC_CommonInit(&ADC_CommonInitStructure);

    }
    else
    {
        if (index == 0)
            RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN;
        else if (index == 1)
            RCC->APB2ENR &= ~RCC_APB2ENR_ADC2EN;
        else if (index == 2)
            RCC->APB2ENR &= ~RCC_APB2ENR_ADC3EN;
    }
}
/**
 * adc规则通道配置
 * @param self: 对象实例指针
 * @param base: ADC结构体
 * @param rule_chn: 规则组序号
 * @param adc_chn: adc通道号
 * @return
 */
static void TDrvAdcRuleChnCfg(TDrvAdc *self, ADC_TypeDef *base, Byte rule_chn, Byte adc_chn)
{
    UInt32 temp_reg;
    if (rule_chn < 6)
    {
        temp_reg = base->SQR3;
        temp_reg &= ~(0x1f << (rule_chn * 5));
        temp_reg |= ((adc_chn) << (rule_chn * 5));
        base->SQR3 = temp_reg;
    }
    else if (rule_chn < 12)
    {
        temp_reg = base->SQR2;
        temp_reg &= ~(0x1f << ((rule_chn - 6) * 5));
        temp_reg |= ((adc_chn) << ((rule_chn - 6) * 5));
        base->SQR2 = temp_reg;
    }
    else if (rule_chn < 16)
    {
        temp_reg = base->SQR1;
        temp_reg &= ~(0x1f << ((rule_chn - 12) * 5));
        temp_reg |= ((adc_chn) << ((rule_chn - 12) * 5));
        base->SQR1 = temp_reg;
    }
}
/**
 * adc通道时间配置
 * @param self: 对象实例指针
 * @param base: ADC结构体
 * @param adc_chn: adc通道号
 * @param Tim: 时间配置序列
 * @return
 */
static void TDrvAdcChnTim(TDrvAdc *self, ADC_TypeDef *base, Byte adc_chn, Byte Tim)
{
    UInt32 temp_reg;
    if (adc_chn < 10)
    {
        /// 设置采样时间
        temp_reg = base->SMPR2;
        temp_reg &= ~((UInt32)0x07 << ((adc_chn) * 3));
        temp_reg |= (Tim << ((adc_chn) * 3));
        base->SMPR2 = temp_reg;
    }
    else
    {   /// 设置采样时间
        temp_reg = base->SMPR1;
        temp_reg &= ~((UInt32)0x07 << ((adc_chn - 10) * 3));
        temp_reg |= (Tim << ((adc_chn - 10) * 3));
        base->SMPR1 = temp_reg;
    }
}
/**
 * adc延时函数(不准)
 * @param value: 对象实例指针
 * @return
 */
static void Delay(uint32_t value)
{
    uint32_t i,j;
    for (i = value; i > 0; i--)
    {
        for (j = 100; j > 0; j--);
    }
}

__IO uint16_t ADC3ConvertedValue[2];


static void TDrvAdcInit(TDrvAdc *self)
{
    Int32 i;
    Int32 j;
    UInt32 tmpreg = 0;
    
    for (i = 0; i < sizeof(g_adc_chip_) / sizeof(g_adc_chip_[0]); i++)
    {
        ADC_TypeDef *base = g_adc_chip_[i].base;
        /// 时钟配置
        TDrvAdcRccCtl(self, i, True);
        /// CR1
        tmpreg = base->CR1;
        tmpreg &= 0xFCFFFEFF;
        tmpreg |= ADC_CR1_SCAN; ///< 非扫描模式
        base->CR1 = tmpreg;
        /// CR2
        tmpreg = base->CR2;
        tmpreg &= 0xC0FFF7FD;
        tmpreg |= (ADC_CR2_EXTSEL | ADC_CR2_DMA);
        base->CR2 = tmpreg;
    }
    /// 循环检查配置ADC
    for (i = 0; i < (sizeof(g_drv_adc_cfgs) / sizeof(g_drv_adc_cfgs[0])); i++)
    {
        for (j = 0; j < (sizeof(g_adc_chip_) / sizeof(g_adc_chip_[0])); j++)
        {
            ADC_TypeDef *base = g_adc_chip_[j].base;
            /// 找到当前adc
            if (g_drv_adc_cfgs[i].base == g_adc_chip_[j].base)
            {
                g_index_rule_chn_[i] = g_adc_chip_[j].chn_used;
                TDrvAdcRuleChnCfg(self, base, g_adc_chip_[j].chn_used, g_drv_adc_cfgs[i].adc_channel);
                TDrvAdcChnTim(self, base, g_drv_adc_cfgs[i].adc_channel, g_drv_adc_cfgs[i].adc_sample_tim);
                g_adc_chip_[j].chn_used++;
                break;
            }
        }
    }
    /// 配置规则通道总个数
    for (i = 0; i < sizeof(g_adc_chip_) / sizeof(g_adc_chip_[0]); i++)
    {
        ADC_TypeDef *base = g_adc_chip_[i].base;
        tmpreg = base->SQR1;
        tmpreg &= 0xFF0FFFFF;
        if (g_adc_chip_[i].chn_used > 1)
            tmpreg |= ((g_adc_chip_[i].chn_used - 1) << 20);
        base->SQR1 = tmpreg;

        if (g_adc_chip_[i].chn_used > 0)
        {
            TDrvDmaChCfg dma_cfg;
            dma_cfg.buffer = (UInt8 *)(&g_adc_chip_[i].dma_buffer[0]);
            dma_cfg.buffer_size = g_adc_chip_[i].chn_used * 2;
            dma_cfg.data_len = 2;
            dma_cfg.dir = 0;
            dma_cfg.peripheral_addr = (UInt32)(&base->DR);
            /// 创建使能DMA
            TDrvDmaChCreate(&g_adc_chip_[i].dma_work, g_adc_chip_[i].dma_chn, &dma_cfg);
            TDrvDmaChOpen(&g_adc_chip_[i].dma_work);
            
            base->CR2 |= (uint32_t)ADC_CR2_DDS;
            /// 使能ADC模块
            base->CR2 |= ADC_CR2_ADON;
            base->CR2 |= ADC_CR2_SWSTART;
        }
    }
}

/**
 * 析构函数
 * @param self: 对象实例指针
 * @return
 */
static void TDrvAdcDestroy(TOfObject *object)
{
    TDrvAdc *self = (TDrvAdc *)object;

    TDrvAdcClose(self);
    self->Destroy_(object);
}

void TDrvAdcCreate(TDrvAdc *self, Int32 index)
{
    if (index >= 0 && index < (sizeof(g_drv_adc_cfgs) / sizeof(g_drv_adc_cfgs[0])) && g_drv_adc_cfgs[index].base != NULL)
    {
        Int32 i;
        TOfObjectCreate((TOfObject *)self);
        OVERRIDE_METHOD(TOfObject, Destroy, TDrvAdcDestroy);
        
        self->cfg_ = &g_drv_adc_cfgs[index];
        self->index_ = index;
        /// ADC未初始化过
        if (g_adc_init == 0)
        {
            TDrvAdcInit(self);
            g_adc_init = 1;
        }

        for (i = 0; i < sizeof(g_adc_chip_) / sizeof(g_adc_chip_[0]); i++)
        {
            ADC_TypeDef *base = g_adc_chip_[i].base;
            if (((TDrvAdcCfg *)(self->cfg_))->base == base)
            {
                self->adc_index_ = i;
                break;
            }
        }
        self->chn_index_ = g_index_rule_chn_[index];

        self->is_open_ = False;
    }
}

Bool TDrvAdcOpen( TDrvAdc *self)
{
    if (!self->is_open_)
    {
        const TDrvAdcCfg *config = (TDrvAdcCfg *)(self->cfg_);
//        ADC_TypeDef *base = config->base;
        // 设置引脚为模拟输入
        TDrvPortPinConfig pin_config;
        pin_config.mux = kPortMuxAltAdc;
        TDrvPortPinOpen(config->adc_port, config->adc_pin, False, &pin_config);

        self->is_open_ = True;
    }
    return True;
}

Bool TDrvAdcClose( TDrvAdc *self )
{
    if (self->is_open_)
    {
//        const TDrvAdcCfg *config = (TDrvAdcCfg *)(self->cfg_);
        self->is_open_ = False;
    }
    return True;
}

UInt16 TDrvAdcRead(TDrvAdc *self)
{
    UInt16 result = 0;

    if (self->is_open_)
    {
        const TDrvAdcCfg *config = (TDrvAdcCfg *)(self->cfg_);
        ADC_TypeDef *base = config->base;
        
        //TDrvDmaChStart(&g_adc_chip_[self->adc_index_].dma_work, False);
        base->CR2 |= (ADC_CR2_SWSTART);
        Delay(1);
        result = g_adc_chip_[self->adc_index_].dma_buffer[self->chn_index_];
    }
    return result;
}
