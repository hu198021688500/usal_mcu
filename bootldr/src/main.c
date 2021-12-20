#include "ofc_types.h"
#include "drv_clock.h"
#include "drv_flash.h"
#include "drv_pmu.h"
#include "stm32f4xx.h"
#include "upgrade.h"

enum
{
    kAppStartAddress                = 0x08020000,          ///< 应用程序启动地址
};

#if defined (__CC_ARM)
__asm static void JumpTo(unsigned int address)
{
    LDR SP, [R0]  ///< Load new stack pointer address
    LDR PC, [R0, #4]  ///< Load new program counter address
}
#elif defined (__GNUC__)
static void JumpTo(unsigned int address)
{
asm(
    "LDR SP, [R0]\n\t"  ///< Load new stack pointer address
    "LDR PC, [R0, #4]\n\t"  ///< Load new program counter address
    );
}
#endif
/**
 * 用不上,只是ofc_object.c中需要用到
 * @param
 * @return
 */
void *OfMalloc(Int32 size)
{
    return NULL;
}

/**
 * 用不上,只是ofc_object.c中需要用到
 * @param
 * @return
 */
void OfFree(void *ptr)
{
}

/**
 * 系统初始化
 * @note 
 */
static void SysInit(void)
{
    /// 初始化时钟
   TDrvClockCreate(&g_drv_clock);
   TDrvClockInitClock(&g_drv_clock);
   TDrvFlashCreate(&g_drv_flash);
   TDrvFlashOpen(&g_drv_flash);
}
int a = 0;
int main()
{
    SysInit();
//    TBootldrUpgradeCreate(&g_upgrade);
//    
//    if (TBootldrUpgradeCheckIsUpdate(&g_upgrade))
//    {
//        TBootldrUpgradeCopyApp(&g_upgrade);
//        DrvPmuSoftReset();
//    }

    
    /// 引导进入Application
    SCB->VTOR = (kAppStartAddress);
    JumpTo(kAppStartAddress);
    while(1)
    {
        if (a == 1)
        {
            a = 0;
        }
        
    }
}
