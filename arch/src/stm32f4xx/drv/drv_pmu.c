#include "drv_pmu.h"
#include "stm32f4xx.h"


void DrvPmuWork(void)
{
    //RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
}


void DrvPmuSleep(void)
{
//    uint32_t tmpreg = 0;
//    
//    RCC->APB2ENR &= ~RCC_APB2ENR_AFIOEN;
//    
//    
//    tmpreg = PWR->CR;
//    tmpreg &= 0xFFFFFFFC;
//    PWR->CR = tmpreg;
//    SCB->SCR |= SCB_SCR_SLEEPDEEP;
//    __WFI();
//    SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);  
}

void DrvPmuSoftReset(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}
