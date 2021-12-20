#include "dev_pmu.h"
#include "vfs.h"
#include "drv_port.h"
#include "drv_dma.h"
/**
 * 设备基础对象
 */
typedef struct TDevPmuTag TDevPmu;
struct TDevPmuTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
};


static TDevPmu g_dev_pmu_;        ///< Pmu0设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevPmuDestroy(TOfObject *object)
{
    TDevPmu *self = (TDevPmu *)object;
    self->Destroy_(object);
}

static void TDevPmuCreate(TDevPmu *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevPmuDestroy);
    
}

static Bool TDevPmuOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    //TDevPmu *self = (TDevPmu *)dev;
    
    return result;
}

void DevPmuInit(void)
{
    UInt32 i;
    for (i = kDrvPortNumA; i < kDrvPortNumMax; i++)
    {
        TDrvPortCreate(&g_drv_port[i]);
    }
    
    /// 打开DMA
    TDrvDmaCreate(&g_drv_dma[0], 0);
    TDrvDmaCreate(&g_drv_dma[1], 1);
    TDrvDmaOpen(&g_drv_dma[0]);
    TDrvDmaOpen(&g_drv_dma[1]);

    TDevPmuCreate(&g_dev_pmu_);
    
    g_dev_pmu_.parent_.open_ = TDevPmuOpen;
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumPower >> 8), (TVfsDevBase*)&g_dev_pmu_);
}
