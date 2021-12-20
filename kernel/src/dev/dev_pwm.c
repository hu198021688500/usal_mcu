#include "dev_pwm.h"
#include "vfs.h"
#include "drv_pwm.h"

/**
 * 设备配置结构体
 */
typedef struct TDevPwmConfigTag TDevPwmConfig;
struct TDevPwmConfigTag
{
    UInt8       drv_index;        ///< 驱动序列号
};

#include "dev_pwm_cfg.c"

/**
 * 设备基础对象
 */
typedef struct TDevPwmTag TDevPwm;
struct TDevPwmTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    TDevPwmConfig     *config_;         ///< 设备配置文件
    UInt8              dev_cnt_;        ///< 设备个数

    TDrvPwm            drv_pwm_[10];    ///< 驱动对象
};


static TDevPwm g_dev_pwm_;        ///< pwm设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevPwmDestroy(TOfObject *object)
{
    TDevPwm *self = (TDevPwm *)object;
    self->Destroy_(object);
}
/**
 * 构造函数
 * @param self: 对象实例
 * @param dev_config: 配置子设备
 * @param dev_cnt: 子设备个数
 * @return 空
 */
static void TDevPwmCreate(TDevPwm *self, TDevPwmConfig *dev_config, UInt8 dev_cnt)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevPwmDestroy);
    UInt8 i;

    self->config_ = dev_config;
    self->dev_cnt_ = dev_cnt;

    for (i = 0; i < dev_cnt; i++)
    {
        TDrvPwmCreate(&self->drv_pwm_[i], dev_config[i].drv_index);
    }
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevPwmOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevPwm *self = (TDevPwm *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        TDrvPwmOpen(&self->drv_pwm_[sub_id]);

        result = True;
    }
    return result;
}
/**
 * 关闭设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevPwmClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevPwm *self = (TDevPwm *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        TDrvPwmClose(&self->drv_pwm_[sub_id]);

        result = True;
    }
    return result;
}
/**
 * 写入设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param  buf: 缓冲区
 * @param  len: 缓冲区长度
 * @return >0 :写入成功到的数据长度
 */
static Int32 TDevPwmWrite(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len)
{
    TDevPwm *self = (TDevPwm *)dev;
    if (sub_id < self->dev_cnt_ && buf != NULL  && len >= sizeof(UInt32))
    {
        UInt32 value = *((UInt32 *)buf);
        TDrvPwmSetVaule(&self->drv_pwm_[sub_id], value);

        return sizeof(UInt32);
    }
    return 0;
}

void DevPwmInit(void)
{
    TDevPwmCreate(&g_dev_pwm_, g_dev_pwm_cfg_, sizeof(g_dev_pwm_cfg_)/ sizeof(g_dev_pwm_cfg_[0]));
    
    g_dev_pwm_.parent_.open_ = TDevPwmOpen;
    g_dev_pwm_.parent_.close_ = TDevPwmClose;
    g_dev_pwm_.parent_.write_ = TDevPwmWrite;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumPwm >> 8), (TVfsDevBase*)&g_dev_pwm_);
    
}
