#include "drv_clock.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_flash.h"

TDrvClock g_drv_clock;

//时钟系统配置函数
//Fvco=Fs*(plln/pllm);
//SYSCLK=Fvco/pllp=Fs*(plln/(pllm*pllp));
//Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

//Fvco:VCO频率
//SYSCLK:系统时钟频率
//Fusb:USB,SDIO,RNG等的时钟频率
//Fs:PLL输入时钟频率,可以是HSI,HSE等. 
//plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
//pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

//外部晶振为25M的时候,推荐值:plln=360,pllm=25,pllp=2,pllq=8.
//得到:Fvco=25*(360/25)=360Mhz
//     SYSCLK=360/2=180Mhz
//     Fusb=360/8=45Mhz

typedef struct TDrvPllSetTag TDrvPllSet;
struct TDrvPllSetTag
{
    UInt32 pll_m;//主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    UInt32 pll_n;//主PLL倍频系数(PLL倍频),取值范围:64~432.
    UInt32 pll_p;//系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    UInt32 pll_q;//USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
};

typedef struct TDrvBusCLkSetTag TDrvBusCLkSet;
struct TDrvBusCLkSetTag
{
    UInt32 hclk_div;        ///< hck时钟分频,库函数枚举
    UInt32 pb1_div;         ///< pb1时钟分频,库函数枚举
    UInt32 pb2_div;         ///< pb2时钟分频,库函数枚举
};

/// 外部晶振8M
/// Fvco = 8 / 8 * 192 = 192MHz
/// SYSCLK = 192 / 2 = 96MHz
static TDrvPllSet g_drv_pll_set = {8, 192, 2, 4};
/// HCLK = SYSCLK = 96MHz
/// PB1CLK = HCLK / 2 = 48MHz
/// PB2CLK = HCLK / 2 = 48MHz
static TDrvBusCLkSet g_drv_bus_clk_set = {RCC_SYSCLK_Div1, RCC_HCLK_Div2, RCC_HCLK_Div2};
/// 系统时钟
UInt32 SystemCoreClock = 96000000;

TDrvClockFrequency g_clock_frequency_;
/**
 * 销毁时钟
 * @param self: 对象实例指针
 * @return
 */
static void TDrvClockDestroy(TOfObject *self)
{
    ((TDrvClock *)self)->Destroy_(self);
}

void TDrvClockCreate(TDrvClock *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvClockDestroy);
}

void TDrvClockInitClock(TDrvClock *self)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;
    
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == SUCCESS)
    {
        FLASH_PrefetchBufferCmd(ENABLE);
		FLASH_SetLatency(FLASH_Latency_2);
        /// PLL配置
        RCC_PLLConfig(RCC_PLLSource_HSE, g_drv_pll_set.pll_m, g_drv_pll_set.pll_n, g_drv_pll_set.pll_p, g_drv_pll_set.pll_q);
        /// pll使能
        RCC_PLLCmd(ENABLE);
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != SET);
        
        /// 配置pll为系统时钟
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        /// 等待配置时钟切换到pll并稳定
        while(RCC_GetSYSCLKSource() != 0x08);
        /// HCLK = SYSCLK
        RCC_HCLKConfig(g_drv_bus_clk_set.hclk_div);
        /// PB1 = HCK / 2
        RCC_PCLK1Config(g_drv_bus_clk_set.pb1_div);
        /// PB2 = HCK / 2
        RCC_PCLK2Config(g_drv_bus_clk_set.pb2_div);
    }
    
    RCC_GetClocksFreq(&RCC_ClocksStatus);
    
    self->clock_frequency_.sysclk_frequency = RCC_ClocksStatus.SYSCLK_Frequency;
    self->clock_frequency_.hclk_frequency = RCC_ClocksStatus.HCLK_Frequency;
    self->clock_frequency_.pclk1_frequency = RCC_ClocksStatus.PCLK1_Frequency;
    self->clock_frequency_.pclk2_frequency = RCC_ClocksStatus.PCLK2_Frequency;
    
    SystemCoreClock = self->clock_frequency_.sysclk_frequency;
}

Bool TDrvClockGetFrequency(TDrvClock *self, TDrvClockFrequency *clock_frequency)
{
    Bool result = False;
    if (clock_frequency != NULL)
    {
        clock_frequency->sysclk_frequency = self->clock_frequency_.sysclk_frequency;
        clock_frequency->hclk_frequency = self->clock_frequency_.hclk_frequency;
        clock_frequency->pclk1_frequency = self->clock_frequency_.pclk1_frequency;
        clock_frequency->pclk2_frequency = self->clock_frequency_.pclk2_frequency;

        result = True;
    }
    return result;
}

void TDrvClockDelayUs(TDrvClock *self, UInt32 us)
{
    UInt32 i, j;
    for (i = 0; i < us * 9; i++);
}

void TDrvClockDelayMs(TDrvClock *self, UInt32 ms)
{
    UInt32 i, j;
    while(ms > 0)
    {
        TDrvClockDelayUs(self, 1000);
        ms--;
    }  
}

/**
 * main函数前初始化时钟(先使用内部低速时钟)
 * @param self: 对象实例指针
 * @return
 */
void SystemInit(void)
{
  #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
  #endif
    RCC_DeInit();
    RCC_HSICmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    /// HCLK = SYSCLK
    RCC_HCLKConfig(g_drv_bus_clk_set.hclk_div);
    /// PB1 = HCK / 2
    RCC_PCLK1Config(g_drv_bus_clk_set.pb1_div);
    /// PB2 = HCK / 2
    RCC_PCLK2Config(g_drv_bus_clk_set.pb2_div);
}

