#include "dev_can.h"
#include "vfs.h"
#include "drv_can.h"
#include "can_def.h"
#include "per_can_mcp2515.h"

/**
 * 设备配置结构体
 */
typedef struct TDevCanConfigTag TDevCanConfig;
struct TDevCanConfigTag
{
    UInt32   drv_can_chn;
    TDrvCanBaudRate defaut_baud;    ///< 默认波特率
};

/**
 * 设备配置结构体
 */
typedef struct TDevPerCanConfigTag TDevPerCanConfig;
struct TDevPerCanConfigTag
{
    TDrvPort   *cs_port;       ///< 端口枚举
    UInt8       cs_pin;        ///< 引脚
    TDrvPortNum  int_port;     ///< 中断端口
    Byte        int_pin;       ///< 中断引脚
    TDrvCanBaudRate defaut_baud;    ///< 默认波特率
};

#include "dev_can_cfg.c"
enum
{
    kDevCanMaxCnt   = 2,
    
    kDrvCanRxBufferSize  = 1000,
    kDrvCanTxBufferSize  = 100,
};

/**
 * 设备基础对象
 */
typedef struct TDevCanTag TDevCan;
struct TDevCanTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    
    TDrvCan   drv_can_[kDevCanMaxCnt];        ///< can驱动设备
    TDrvCanBaudRate can_baud_[kDevCanMaxCnt]; ///< can波特率
    UInt8     can_rx_buffer_[kDevCanMaxCnt][kDrvCanRxBufferSize];     ///< 接收缓存
    UInt8     can_tx_buffer_[kDevCanMaxCnt][kDrvCanTxBufferSize];     ///< 发送缓存
    
    TPerCanMcp2515 *per_can_;                ///< 外设can
    UInt8     per_can_rx_buffer_[kDrvCanRxBufferSize];                ///< 接收缓存
    TPerCanMcp2515BaudRate per_can_baud_;                             ///< 外设can波特率
};


static TDevCan g_dev_can_;        ///< can设备



/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevCanDestroy(TOfObject *object)
{
    TDevCan *self = (TDevCan *)object;
    self->Destroy_(object);
}

static void TDevCanCreate(TDevCan *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevCanDestroy);
    
    Int32 i;
    for (i = 0; i < kDevCanMaxCnt; i++)
    {
        TDrvCanCreate(&self->drv_can_[i], g_dev_can_cfg_[i].drv_can_chn, self->can_tx_buffer_[i], sizeof(self->can_tx_buffer_[i]), self->can_rx_buffer_[i], sizeof(self->can_rx_buffer_[i]));
        /// 默认波特率250K
        self->can_baud_[i] = g_dev_can_cfg_[i].defaut_baud;
    }
    /// 第三路can
    self->per_can_ = &g_per_can_mcp2515;
    TPerCanMcp2515Create(self->per_can_, &self->per_can_rx_buffer_[0], sizeof(self->per_can_rx_buffer_), \
        g_dev_per_can_cfg_.cs_port, g_dev_per_can_cfg_.cs_pin, g_dev_per_can_cfg_.int_port, g_dev_per_can_cfg_.int_pin);
    self->per_can_baud_ = (TPerCanMcp2515BaudRate)g_dev_per_can_cfg_.defaut_baud;
}

/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevCanOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevCan *self = (TDevCan *)dev;
    
    if (sub_id < kDevCanMaxCnt)
    {
        result = TDrvCanOpen(&self->drv_can_[sub_id], self->can_baud_[sub_id]);
    }
    else if (sub_id == kDevCanMaxCnt)
    {
        result = TPerCanMcp2515Open(self->per_can_, self->per_can_baud_);
    }
    
    return result;
}
/**
 * 关闭设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevCanClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevCan *self = (TDevCan *)dev;
    
    if (sub_id < kDevCanMaxCnt)
    {
        TDrvCanClose(&self->drv_can_[sub_id]);
        result = True;
    }
    else if (sub_id == kDevCanMaxCnt)
    {
        TPerCanMcp2515Close(self->per_can_);
        result = True;
    }
    
    return result;
}

/**
 * 根据波特率获取配置命令
 * @param dev: 父类对象
 * @param baud: 波特率
 * @return 波特率配置项
 */
static TDrvCanBaudRate TDevCanBaudGet(TDevCan *self, UInt32 baud)
{
    TDrvCanBaudRate result = kCanBaudRate250k;
    
    if (baud == 500000)
        result = kCanBaudRate500k;
    else if (baud == 1000000)
        result = kCanBaudRate1000k;
    else if (baud == 100000)
        result = kCanBaudRate100k;
    else if (baud == 125000)
        result = kCanBaudRate125k;
    
    return result;
}
/**
 * 根据波特率获取配置命令
 * @param dev: 父类对象
 * @param baud: 波特率
 * @return 波特率配置项
 */
static TPerCanMcp2515BaudRate TDevPerCanBaudGet(TDevCan *self, UInt32 baud)
{
    TPerCanMcp2515BaudRate result = kPerCanMcp2515BaudRate250k;
    
    if (baud == 500000)
        result = kPerCanMcp2515BaudRate500k;
    else if (baud == 1000000)
        result = kPerCanMcp2515BaudRate1000k;
    else if (baud == 100000)
        result = kPerCanMcp2515BaudRate100k;
    else if (baud == 125000)
        result = kPerCanMcp2515BaudRate125k;
    
    return result;
}

/**
 * 设备控制
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param cmd: 命令号
 * @param argv: 命令参数
 * @return True:成功 False:失败
 */
static Bool TDevCanIoctl(TVfsDevBase *dev, UInt8 sub_id, UInt32 cmd, void *argv)
{
    Bool result = False;
    TDevCan *self = (TDevCan *)dev;
    
    if (cmd == 0 && argv != NULL)
    {
        if (sub_id < kDevCanMaxCnt)
        {
            TDrvCanBaudRate baud_index = TDevCanBaudGet(self, *((UInt32 *)argv));
            /// 如果波特率和当前不符
            if (baud_index != self->can_baud_[sub_id])
            {
                self->can_baud_[sub_id] = baud_index;
                
                TDrvCanClose(&self->drv_can_[sub_id]);
                TDrvCanOpen(&self->drv_can_[sub_id], baud_index);
            }
            result = True;
        }
        else if (sub_id == kDevCanMaxCnt)
        {
            TPerCanMcp2515BaudRate baud_index = TDevPerCanBaudGet(self, *((UInt32 *)argv));
            /// 如果波特率和当前不符
            if (baud_index != self->per_can_baud_)
            {
                self->per_can_baud_ = baud_index;
                
                TPerCanMcp2515Close(self->per_can_);
                TPerCanMcp2515Open(self->per_can_, self->per_can_baud_);
            }
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
Int32 TDevCanRead(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    Int32 result = 0;
    
    TDevCan *self = (TDevCan *)dev;
    
    if (buf != NULL && size >= sizeof(TCanData))
    {
        if (sub_id < kDevCanMaxCnt)
        {
            result = TDrvCanRead(&self->drv_can_[sub_id], (TCanData *)buf, (size / sizeof(TCanData)));
            result *= sizeof(TCanData);
        }
        else if (sub_id == kDevCanMaxCnt)
        {
            result = TPerCanMcp2515Read(self->per_can_, (TCanData *)buf, (size / sizeof(TCanData)));
            result *= sizeof(TCanData);
        }
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
static Int32 TDevCanWrite(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len)
{
    Int32 result = 0; 
    TDevCan *self = (TDevCan *)dev;
    
    if (buf != NULL && len == sizeof(TCanData))
    {    
        if (sub_id < kDevCanMaxCnt)
        {
            if (TDrvCanWrite(&self->drv_can_[sub_id], (TCanData *)buf))
                result = len;
        }
        else if (sub_id == kDevCanMaxCnt)
        {
            if (TPerCanMcp2515Write(self->per_can_, (TCanData *)buf))
                result = len;
        }
    }
    
    return result;
}

void DevCanInit(void)
{
    TDevCanCreate(&g_dev_can_);
    g_dev_can_.parent_.open_ = TDevCanOpen;
    g_dev_can_.parent_.close_ = TDevCanClose;
    g_dev_can_.parent_.ioctl_ = TDevCanIoctl;
    g_dev_can_.parent_.write_ = TDevCanWrite;
    g_dev_can_.parent_.read_ = TDevCanRead;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumCan >> 8), (TVfsDevBase*)&g_dev_can_);
}
