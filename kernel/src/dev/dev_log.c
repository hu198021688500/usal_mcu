#include "dev_log.h"
#include "vfs.h"
#include "drv_uart.h"
#include "kfc_system.h"

/**
 * 设备基础对象
 */
typedef struct TDevLogTag TDevLog;
struct TDevLogTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

    TDrvUart    drv_uart_;        ///< 驱动设备
    UInt8       rx_buffer_[100];       ///< 读取缓存
PUBLIC
};

static TDevLog g_dev_log_;        ///< can0设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevLogDestroy(TOfObject *object)
{
    TDevLog *self = (TDevLog *)object;
    self->Destroy_(object);
}

static void TDevLogCreate(TDevLog *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevLogDestroy);
    
    TDrvUartCreate(&self->drv_uart_, 0, self->rx_buffer_, sizeof(self->rx_buffer_), kDrvUartMode232);
}
/**
 * 开启设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevLogOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    TDevLog *self = (TDevLog *)dev;
    TDrvUartOpen(&self->drv_uart_, 115200, kDrvUartParityNone);
    return True;
}
/**
 * 关闭设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevLogClose(TVfsDevBase *dev, UInt8 sub_id)
{
    TDevLog *self = (TDevLog *)dev;
    TDrvUartClose(&self->drv_uart_);
    return True;
}
/**
 * 读取设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param  buf: 缓冲区
 * @param  len: 缓冲区长度
 * @return >0 :成功读取到的数据长度
 */
Int32 TDevLogRead(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    TDevLog *self = (TDevLog *)dev;
    return TDrvUartRead(&self->drv_uart_, buf, size);
}
/**
 * 写入设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @param  buf: 缓冲区
 * @param  len: 缓冲区长度
 * @return >0 :写入成功到的数据长度
 */
static Int32 TDevLogWrite(TVfsDevBase *dev, UInt8 sub_id, void *data, Int32 len)
{
    Int32 result = 0;
    TDevLog *self = (TDevLog *)dev;
    result = TDrvUartWrite(&self->drv_uart_, data, len);
    KfcDelayMs(2);
    return result;
}

void DevLogInit(void)
{
    TDevLogCreate(&g_dev_log_);
    g_dev_log_.parent_.open_ = TDevLogOpen;
    g_dev_log_.parent_.close_ = TDevLogClose;
    g_dev_log_.parent_.read_ = TDevLogRead;
    g_dev_log_.parent_.write_ = TDevLogWrite;
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumLog >> 8), (TVfsDevBase*)&g_dev_log_);
}
