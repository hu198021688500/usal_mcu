#include "dev_power.h"
#include "vfs.h"
#include "drv_port.h"

/**
 * 设备配置结构体
 */
typedef struct TDevPowerConfigTag TDevPowerConfig;
struct TDevPowerConfigTag
{
    TDrvPort   *port;       ///< 端口枚举
    UInt8       pin;        ///< 引脚
    Bool        is_od;      ///< 是否开漏输出
    Bool        def_s;      ///< 默认状态
    Bool        is_open;    ///< 当前状态,默认设置为False;
};

#include "dev_power_cfg.c"

/**
 * 设备基础对象
 */
typedef struct TDevPowerTag TDevPower;
struct TDevPowerTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    TDevPowerConfig     *config_;         ///< 设备配置文件
    UInt8               dev_cnt_;         ///< 设备个数
};

static TDevPower g_dev_power_;        ///< Power0设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevPowerDestroy(TOfObject *object)
{
    TDevPower *self = (TDevPower *)object;
    self->Destroy_(object);
}
/**
 * 构造函数
 * @param self: 对象实例
 * @param dev_config: 配置子设备
 * @param dev_cnt: 子设备个数
 * @return 空
 */
static void TDevPowerCreate(TDevPower *self, TDevPowerConfig *dev_config, UInt8 dev_cnt)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevPowerDestroy);
    
    self->config_ = dev_config;
    self->dev_cnt_ = dev_cnt;
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevPowerOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevPower *self = (TDevPower *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        if (!self->config_[sub_id].is_open)
        {
            TDrvPortPinConfig od_cfg = {kPortMuxDisabled , kPortPullDisabled, kPortStrengthHigh};
        
            if (self->config_[sub_id].is_od)
                    TDrvPortPinOpen(self->config_[sub_id].port, self->config_[sub_id].pin, True, &od_cfg);
            else
                TDrvPortPinOpen(self->config_[sub_id].port, self->config_[sub_id].pin, True, NULL);
            
            TDrvPortPinSet(self->config_[sub_id].port, self->config_[sub_id].pin, self->config_[sub_id].def_s);
            
            self->config_[sub_id].is_open = True;
            
        }
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
static Bool TDevPowerClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevPower *self = (TDevPower *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        if (self->config_[sub_id].is_open)
        {
            TDrvPortPinClose(self->config_[sub_id].port, self->config_[sub_id].pin);
            self->config_[sub_id].is_open = False;
        }
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
static Int32 TDevPowerWrite(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len)
{
    TDevPower *self = (TDevPower *)dev;
    if (sub_id < self->dev_cnt_ && self->config_[sub_id].is_open && buf != NULL  && len >= sizeof(UInt32))
    {
        Bool value = (*(UInt32 *)buf > 0) ? True : False;
        TDrvPortPinSet(self->config_[sub_id].port, self->config_[sub_id].pin, value);
        
        return sizeof(UInt32);
    }
    return 0;
}

void DevPowerInit(void)
{
    TDevPowerCreate(&g_dev_power_, g_dev_power_cfg_, sizeof(g_dev_power_cfg_)/ sizeof(g_dev_power_cfg_[0]));
    
    g_dev_power_.parent_.open_ = TDevPowerOpen;
    g_dev_power_.parent_.close_ = TDevPowerClose;
    g_dev_power_.parent_.write_ = TDevPowerWrite;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumPower >> 8), (TVfsDevBase*)&g_dev_power_);
    
}
