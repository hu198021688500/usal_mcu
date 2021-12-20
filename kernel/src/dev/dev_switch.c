#include "dev_switch.h"
#include "vfs.h"
#include "drv_port.h"

/**
 * 设备配置结构体
 */
typedef struct TDevSwitchConfigTag TDevSwitchConfig;
struct TDevSwitchConfigTag
{
    TDrvPort   *port;       ///< 端口枚举
    UInt8       pin;        ///< 引脚
    TPortPull   pull;       ///< 上下拉配置
    Bool        is_open;    ///< 默认为False
};

#include "dev_switch_cfg.c"

/**
 * 设备基础对象
 */
typedef struct TDevSwitchTag TDevSwitch;
struct TDevSwitchTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    TDevSwitchConfig    *config_;         ///< 设备配置文件
    UInt8               dev_cnt_;         ///< 设备个数
    Bool                is_open_;         ///< 是否打开
};


static TDevSwitch g_dev_switch_;        ///< Switch0设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevSwitchDestroy(TOfObject *object)
{
    TDevSwitch *self = (TDevSwitch *)object;
    self->Destroy_(object);
}

/**
 * 构造函数
 * @param self: 对象实例
 * @param dev_config: 配置子设备
 * @param dev_cnt: 子设备个数
 * @return 空
 */
static void TDevSwitchCreate(TDevSwitch *self, TDevSwitchConfig *dev_config, UInt8 dev_cnt)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevSwitchDestroy);
    
    self->is_open_ = False;
    self->config_ = dev_config;
    self->dev_cnt_ = dev_cnt;
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevSwitchOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevSwitch *self = (TDevSwitch *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        if (!self->config_[sub_id].is_open)
        {
            TDrvPortPinConfig od_cfg = {kPortMuxDisabled , self->config_[sub_id].pull, kPortStrengthLow};
            TDrvPortPinOpen(self->config_[sub_id].port, self->config_[sub_id].pin, False, &od_cfg);
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
static Bool TDevSwitchClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevSwitch *self = (TDevSwitch *)dev;
    
    if (sub_id < self->dev_cnt_)
    {
        if (self->config_[sub_id].is_open)
        {
            TDrvPortPinClose(self->config_[sub_id].port, self->config_[sub_id].pin);
            self->config_[sub_id].is_open = False;
            
            result = True;
        }
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
Int32 TDevSwitchRead(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    Int32 result = 0;
    
    TDevSwitch *self = (TDevSwitch *)dev;
    if (sub_id < self->dev_cnt_ && self->config_[sub_id].is_open && buf != NULL  && size >= sizeof(UInt32))
    {
        Bool status;
        if (TDrvPortPinGet(self->config_[sub_id].port, self->config_[sub_id].pin, &status))
        {
            *(UInt32 *)buf = status;
            
            result = sizeof(UInt32);
        }
    }
    return result;
}

void DevSwitchInit(void)
{
    TDevSwitchCreate(&g_dev_switch_, g_dev_switch_cfg_, sizeof(g_dev_switch_cfg_)/ sizeof(g_dev_switch_cfg_[0]));
    
    g_dev_switch_.parent_.open_ = TDevSwitchOpen;
    g_dev_switch_.parent_.close_ = TDevSwitchClose;
    g_dev_switch_.parent_.read_ = TDevSwitchRead;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumSwitch >> 8), (TVfsDevBase*)&g_dev_switch_);
    
}
