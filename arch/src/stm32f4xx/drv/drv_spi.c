#include "drv_spi.h"
#include "drv_spi_cfg.h"
#include "drv_clock.h"
#include "stm32f4xx.h"

#include "drv_spi_cfg.c"


static SPI_TypeDef* g_spi_base_[] = {SPI1, SPI2, SPI3};

static void TDrvSpiClockOpen(TDrvSpi *self, Byte spi_index)
{
    switch(spi_index)
    {
        case 0: RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;break;
        case 1: RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;break;
        case 2: RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;break;
        case 3: RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;break;
        case 4: RCC->APB2ENR |= RCC_APB2ENR_SPI5EN;break;
        case 5: RCC->APB2ENR |= RCC_APB2ENR_SPI6EN;break;
        default:
            break;
    }
}

static void TDrvSpiClockClose(TDrvSpi *self, Byte spi_index)
{
    switch(spi_index)
    {
        case 0: RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;break;
        case 1: RCC->APB1ENR &= ~RCC_APB1ENR_SPI2EN;break;
        case 2: RCC->APB1ENR &= ~RCC_APB1ENR_SPI3EN;break;
        case 3: RCC->APB2ENR &= ~RCC_APB2ENR_SPI4EN;break;
        case 4: RCC->APB2ENR &= ~RCC_APB2ENR_SPI5EN;break;
        case 5: RCC->APB2ENR &= ~RCC_APB2ENR_SPI6EN;break;
        default:
            break;
    }
}


static void TDrvSpiPinOpen(TDrvSpi *self, const TDrvSpiCfg *cfg)
{
    TDrvPortPinConfig pin_config;
    
    pin_config.mux = cfg->spi_mux;
    pin_config.pull = kPortPullDisabled;
    pin_config.strength = kPortStrengthLow;
    TDrvPortPinOpen(cfg->sck_port, cfg->sck_pin, True, &pin_config);
    TDrvPortPinOpen(cfg->mosi_port, cfg->mosi_pin, True, &pin_config);
    
    TDrvPortPinOpen(cfg->miso_port, cfg->miso_pin, False, &pin_config);
}

static void TDrvSpiDestroy(TOfObject *object)
{
    TDrvSpi *self = (TDrvSpi *)object;
    
    self->Destroy_(object);
}

Bool TDrvSpiCreate(TDrvSpi *self, UInt8 index)
{
    Bool result = False;
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvSpiDestroy);
    
    self->is_open_ = False;
    self->index_ = -1;
    self->spi_index_ = -1;
    if (index < (sizeof(g_drv_spi_cfgs) / sizeof(g_drv_spi_cfgs[0])))
    {
        self->index_ = index;
        self->spi_index_ = g_drv_spi_cfgs[index].spi_index;
        
        result = True;
    }
    return result; 
}

void TDrvSpiOpen(TDrvSpi *self)
{
    if (self->spi_index_ >= 0 && !self->is_open_)
    {
        const TDrvSpiCfg *cfg = &g_drv_spi_cfgs[self->index_];
        
        SPI_TypeDef *spi_base = g_spi_base_[self->spi_index_];
        /// 打开时钟
        TDrvSpiClockOpen(self, cfg->spi_index);
        /// 初始化引脚
        TDrvSpiPinOpen(self, cfg);
        
        uint16_t temp_reg = 0;
        temp_reg = spi_base->CR1;
        
        temp_reg &= 0x3040;
        
        temp_reg |= (SPI_CR1_MSTR | SPI_CR1_SSI | SPI_CR1_SSM);
        if (cfg->cpol)
            temp_reg |= SPI_CR1_CPOL;
        
        if (cfg->cpha)
            temp_reg |= SPI_CR1_CPHA;
        
        temp_reg |= (cfg->baud_div << 3);
  
        spi_base->CR1 = temp_reg;
//      /* Write to SPIx CRCPOLY */
        //spi_base->CRCPR = 7;
        spi_base->CRCPR = 10;
//        /// 使能spi
        spi_base->CR1 |= SPI_CR1_SPE;
        self->is_open_ = True;
    }
    
}

void TDrvSpiClose(TDrvSpi *self)
{
    if (self->is_open_)
    {
        UInt32 try_max = 10;
        SPI_TypeDef *spi_base = g_spi_base_[self->spi_index_];
        /// 等待关闭spi
        while(!((spi_base->SR & SPI_SR_TXE) && !(spi_base->SR & SPI_SR_RXNE)
            && !(spi_base->SR & SPI_SR_BSY)) && try_max -- > 0)
        {
            TDrvClockDelayUs(&g_drv_clock, 1);
        }
        /// 关闭spi
        spi_base->CR1 &= ~SPI_CR1_SPE;
        
        TDrvSpiClockClose(self, self->spi_index_);
        
        self->is_open_ = False;
    }
}

Bool TDrvSpiIsOpen( TDrvSpi *self )
{
    return self->is_open_; 
}

Byte TDrvSpiWriteAndRead(TDrvSpi *self, Byte value)
{
    Byte result = 0;
    if (self->is_open_)
    {
        UInt32 try_max = 10;
        SPI_TypeDef *spi_base = g_spi_base_[self->spi_index_];
        
        while(!(spi_base->SR & SPI_SR_TXE) && try_max-- > 0)
        {
            TDrvClockDelayUs(&g_drv_clock, 1);
        }
        spi_base->DR = value;
        try_max = 10;
        while(!(spi_base->SR & SPI_SR_RXNE) && try_max-- > 0)
        {
            TDrvClockDelayUs(&g_drv_clock, 1);
        }
        result = spi_base->DR;
    }
    return result;
}
