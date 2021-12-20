#include "drv_wdog.h"
//#include "stm32f413xx.h"
#include "stm32f4xx.h"

TDrvWdog g_drv_wdog;

enum
{
    kDrvDogCmdStart        = 0xCCCC,           ///< 看门狗打开命令
    kDrvDogCmdSetEnable    = 0x5555,           ///< 看门狗设置使能命令(配置前需设置)
    kDrvDogCmdFeedDog      = 0xAAAA,           ///< 看门狗喂狗命令
};
   
/**
 * 看门狗分频因子枚举
 */
typedef enum TDrvWdogPrescalerTag TDrvWdogPrescaler;
enum TDrvWdogPrescalerTag
{
    kDrvWdogPrescaler4      = 0,        ///< 预分频因子=4(默认)
    kDrvWdogPrescaler8      = 1,        ///< 预分频因子=8
    kDrvWdogPrescaler16     = 2,        ///< 预分频因子=16
    kDrvWdogPrescaler32     = 3,        ///< 预分频因子=32
    kDrvWdogPrescaler64     = 4,        ///< 预分频因子=64
    kDrvWdogPrescaler128    = 5,        ///< 预分频因子=128
    kDrvWdogPrescaler256    = 6,        ///< 预分频因子=256(设置为7也是256)
};
/**
 * 设置分频因子
 * @param self: 对象实例指针
 * @return
 */
static void TDrvWdogSetPrescaler(TDrvWdog *self, TDrvWdogPrescaler pre)
{
    IWDG->KR = kDrvDogCmdSetEnable;
    while(IWDG->SR & 0x01);
    IWDG->PR = pre;
}
/**
 * 设置重置值
 * @param self: 对象实例指针
 * @param reload: 重置值(0~0xfff)
 * @return
 */
static void TDrvWdogSetReload(TDrvWdog *self, UInt16 reload)
{
    IWDG->KR = kDrvDogCmdSetEnable;
    while(IWDG->SR & 0x02);
    IWDG->RLR = reload & 0xfff;
}

/**
 * 销毁看门狗
 * @param self: 对象实例指针
 * @return
 */
static void TDrvWdogDestroy(TOfObject *self)
{
    ((TDrvWdog *)self)->Destroy_(self);
}

void TDrvWdogCreate(TDrvWdog *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvWdogDestroy);
}

void TDrvWdogOpen(TDrvWdog *self)
{
    TDrvWdogSetPrescaler(&g_drv_wdog, kDrvWdogPrescaler64);
    TDrvWdogSetReload(&g_drv_wdog,1000);
    
    IWDG->KR = kDrvDogCmdStart;
}

void TDrvWdogClose(TDrvWdog *self)
{
    
}

void TDrvWdogFeedDog(TDrvWdog *self)
{
    IWDG->KR = kDrvDogCmdFeedDog;
}


