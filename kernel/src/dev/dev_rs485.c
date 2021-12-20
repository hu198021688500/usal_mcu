#include "dev_rs485.h"
#include "vfs.h"
#include "drv_uart.h"

/**
 * 设备配置结构体
 */
typedef struct TDevRs485ConfigTag TDevRs485Config;
struct TDevRs485ConfigTag
{
    UInt32   drv_uart_chn;      ///< 驱动串口号
    UInt32   baud_default;      ///< 波特率默认
};

#include "dev_rs485_cfg.c"

enum
{
    kDevRs485Cnt        = (sizeof(g_dev_rs485_cfg_) / sizeof(g_dev_rs485_cfg_[0])),        ///< Rs485路数
};

/**
 * 设备基础对象
 */
typedef struct TDevRs485Tag TDevRs485;
struct TDevRs485Tag
{
    TVfsDevBase parent_;            ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数
PUBLIC   
    TDrvUart    drv_uart_[kDevRs485Cnt];          ///< 驱动设备
    UInt32      baud_[kDevRs485Cnt];              ///< 当前波特率
    UInt8       rx_buffer_[kDevRs485Cnt][200];    ///< 读取缓存
};

static TDevRs485 g_dev_rs485_;        ///< rs485设备

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevRs485Destroy(TOfObject *object)
{
    TDevRs485 *self = (TDevRs485 *)object;
    self->Destroy_(object);
}

static void TDevRs485Create(TDevRs485 *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevRs485Destroy);
    UInt32 i;
    for (i = 0; i < kDevRs485Cnt; i++)
    {
        TDevRs485Config *config = &g_dev_rs485_cfg_[i];
        TDrvUartCreate(&self->drv_uart_[i], config->drv_uart_chn, self->rx_buffer_, sizeof(self->rx_buffer_), kDrvUartMode485);
        self->baud_[i] = config->baud_default;
    }
}
/**
 * 开启设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevRs485Open(TVfsDevBase *dev, UInt8 sub_id)
{
    TDevRs485 *self = (TDevRs485 *)dev;
    if (sub_id < kDevRs485Cnt)
    {
        TDrvUartOpen(&self->drv_uart_[sub_id], self->baud_[sub_id], kDrvUartParityNone);
    }
    return True;
}
/**
 * 关闭设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevRs485Close(TVfsDevBase *dev, UInt8 sub_id)
{
    TDevRs485 *self = (TDevRs485 *)dev;
    if (sub_id < kDevRs485Cnt)
    {
        TDrvUartClose(&self->drv_uart_[sub_id]);
    }
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
Int32 TDevRs485Read(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    Int32 result= 0;
    TDevRs485 *self = (TDevRs485 *)dev;
    
    if (sub_id < kDevRs485Cnt)
    {
        result = TDrvUartRead(&self->drv_uart_[sub_id], buf, size);
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
static Int32 TDevRs485Write(TVfsDevBase *dev, UInt8 sub_id, void *data, Int32 len)
{
    Int32 result= 0;
    TDevRs485 *self = (TDevRs485 *)dev;
    
    if (sub_id < kDevRs485Cnt)
    {
        result = TDrvUartWrite(&self->drv_uart_[sub_id], data, len);
    }
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
static Bool TDevRs485Ioctl(TVfsDevBase *dev, UInt8 sub_id, UInt32 cmd, void *argv)
{
    Bool result = False;
    TDevRs485 *self = (TDevRs485 *)dev;
    
    if (sub_id < kDevRs485Cnt)
    { 
        if (cmd == 0 && argv != NULL)
        {
            UInt32 set_baud = *((UInt32 *)argv);
            /// 如果波特率和当前不符
            if (set_baud != self->baud_[sub_id])
            {
                self->baud_[sub_id] = set_baud;
                
                TDrvUartClose(&self->drv_uart_[sub_id]);
                TDrvUartOpen(&self->drv_uart_[sub_id], self->baud_[sub_id], kDrvUartParityNone);
            }
            result = True;
        }
    }
    
    return result;
}

void DevRs485Init(void)
{
    TDevRs485Create(&g_dev_rs485_);
    g_dev_rs485_.parent_.open_ = TDevRs485Open;
    g_dev_rs485_.parent_.close_ = TDevRs485Close;
    g_dev_rs485_.parent_.read_ = TDevRs485Read;
    g_dev_rs485_.parent_.write_ = TDevRs485Write;
    g_dev_rs485_.parent_.ioctl_ = TDevRs485Ioctl;
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumRs485 >> 8), (TVfsDevBase*)&g_dev_rs485_);
}
