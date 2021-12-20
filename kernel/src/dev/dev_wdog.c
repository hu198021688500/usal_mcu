#include "dev_wdog.h"
#include "vfs.h"
#include "drv_wdog.h"
/**
 * 设备基础对象
 */
typedef struct TDevWdogTag TDevWdog;
struct TDevWdogTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
};


static TDevWdog g_dev_wdog_;        ///< 看门狗设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevWdogDestroy(TOfObject *object)
{
    TDevWdog *self = (TDevWdog *)object;
    self->Destroy_(object);
}

static void TDevWdogCreate(TDevWdog *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevWdogDestroy);
    
    TDrvWdogCreate(&g_drv_wdog);
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevWdogOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    TDrvWdogOpen(&g_drv_wdog);
    return True;
}
/**
 * 写入设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param  buf: 缓冲区
 * @param  len: 缓冲区长度
 * @return >0 :写入成功到的数据长度
 */
static Int32 TDevWdogWrite(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len)
{
    TDrvWdogFeedDog(&g_drv_wdog);
    return 0;
}

void DevWdogInit(void)
{
    TDevWdogCreate(&g_dev_wdog_);
    
    g_dev_wdog_.parent_.open_ = TDevWdogOpen;
    g_dev_wdog_.parent_.write_ = TDevWdogWrite;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumWdog >> 8), (TVfsDevBase*)&g_dev_wdog_);
    
}
