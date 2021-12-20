#include "dev_adc.h"
#include "vfs.h"
#include "drv_adc.h"

/**
 * 设备配置结构体
 */
typedef struct TDevAdcConfigTag TDevAdcConfig;
struct TDevAdcConfigTag
{
    UInt32   drv_adc_chn;
};

#include "dev_adc_cfg.c"

enum
{
    kDevAdcMaxCnt   = (sizeof(g_dev_adc_cfg_) / sizeof(g_dev_adc_cfg_[0])),
};

/**
 * 设备基础对象
 */
typedef struct TDevAdcTag TDevAdc;
struct TDevAdcTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    
    TDrvAdc drv_adc_[kDevAdcMaxCnt];          
};


static TDevAdc g_dev_adc_;        ///< adc设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevAdcDestroy(TOfObject *object)
{
    TDevAdc *self = (TDevAdc *)object;

    self->Destroy_(object);
}

/**
 * 构造函数
 * @param self: 对象实例
 * @param dev_config: 配置子设备
 * @param dev_cnt: 子设备个数
 * @return 空
 */
static void TDevAdcCreate(TDevAdc *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevAdcDestroy);
    
    Int32 i;
    for (i = 0; i < kDevAdcMaxCnt; i++)
    {
        TDrvAdcCreate(&self->drv_adc_[i], g_dev_adc_cfg_[i].drv_adc_chn);
    }
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevAdcOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevAdc *self = (TDevAdc *)dev;

    if (sub_id < kDevAdcMaxCnt)
    {
        result = TDrvAdcOpen(&self->drv_adc_[sub_id]);
    }
    
    return result;
}
/**
 * 关闭设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevAdcClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevAdc *self = (TDevAdc *)dev;

    if (sub_id < kDevAdcMaxCnt)
    {
        result = TDrvAdcClose(&self->drv_adc_[sub_id]);
    }
    
    return result;
}
/**
 * 读取设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param  buf: 缓冲区
 * @param  len: 缓冲区长度
 * @return >0 :成功读取到的数据长度
 */
Int32 TDevAdcRead(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    Int32 result = 0;
    TDevAdc *self = (TDevAdc *)dev;
    UInt32 value = 0;
    
    if (sub_id < kDevAdcMaxCnt)
    {
        value = TDrvAdcRead(&self->drv_adc_[sub_id]);
        *(UInt32 *)buf = value;
        result = 4;
    }
    return result;
}

void DevAdcInit(void)
{
    TDevAdcCreate(&g_dev_adc_);
    
    g_dev_adc_.parent_.open_ = TDevAdcOpen;
    g_dev_adc_.parent_.close_ = TDevAdcClose;
    g_dev_adc_.parent_.read_ = TDevAdcRead;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumAdc >> 8), (TVfsDevBase*)&g_dev_adc_);
    
}
