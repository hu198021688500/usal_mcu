#include "dev_flash.h"
#include "vfs.h"
#include "drv_flash.h"

/**
 * 设备配置结构体
 */
typedef struct TDevFlashConfigTag TDevFlashConfig;
struct TDevFlashConfigTag
{
    UInt32   addr;
    UInt32   size;
};

#include "dev_flash_cfg.c"
enum
{
    kDevFlashMaxCnt   = (sizeof(g_dev_flash_cfg_) / sizeof(g_dev_flash_cfg_[0])),
};

/**
 * 设备基础对象
 */
typedef struct TDevFlashTag TDevFlash;
struct TDevFlashTag
{
    TVfsDevBase parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;    ///< 析构函数

PUBLIC
    TDrvFlash        *drv_flash_;   ///< 驱动对象
    TDevFlashConfig  *config_;      ///< 配置项
    UInt32            w_[kDevFlashMaxCnt];  ///< 写指针地址
    UInt32            r_[kDevFlashMaxCnt];  ///< 读指针地址
};


static TDevFlash g_dev_flash_;        ///< flash设备



/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TDevFlashDestroy(TOfObject *object)
{
    TDevFlash *self = (TDevFlash *)object;
    self->Destroy_(object);
}

static void TDevFlashCreate(TDevFlash *self)
{
    TVfsDevBaseCreate((TVfsDevBase *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDevFlashDestroy);
    self->drv_flash_ = &g_drv_flash;
    self->config_ = &g_dev_flash_cfg_[0];
}
/**
 * 打开设备
 * @param dev: 父类对象
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
static Bool TDevFlashOpen(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevFlash *self = (TDevFlash *)dev;
    
    if (sub_id < kDevFlashMaxCnt)
    {
        self->w_[sub_id] = 0;
        self->r_[sub_id] = 0;
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
static Bool TDevFlashClose(TVfsDevBase *dev, UInt8 sub_id)
{
    Bool result = False;
    TDevFlash *self = (TDevFlash *)dev;
    
    if (sub_id < kDevFlashMaxCnt)
    {
        self->w_[sub_id] = 0;
        self->r_[sub_id] = 0;
        result = True;
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
static Bool TDevFlashIoctl(TVfsDevBase *dev, UInt8 sub_id, UInt32 cmd, void *argv)
{
    Bool result = False;
    TDevFlash *self = (TDevFlash *)dev;
    
    if (sub_id < kDevFlashMaxCnt && cmd < kSfDevNumFlashIoctlCmdMax)
    {
        /// 擦除指令
        if (cmd == kSfDevNumFlashIoctlCmdErasePage && argv != NULL)
        {
            UInt32 addr = *(UInt32 *)argv;
            UInt32 size = *((UInt32 *)argv + 1);
            result = TDrvFlashErasePage(self->drv_flash_, self->config_[sub_id].addr + addr, size);
        }
        /// 写指针
        else if (cmd == kSfDevNumFlashIoctlCmdWriteAddr && argv != NULL)
        {
            UInt32 addr = *(UInt32 *)argv;
            self->w_[sub_id] = addr;
            result = True;
        }
        /// 读指针
        else if (cmd == kSfDevNumFlashIoctlCmdReadAddr && argv != NULL)
        {
            UInt32 addr = *(UInt32 *)argv;
            self->r_[sub_id] = addr;
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
Int32 TDevFlashRead(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 size)
{
    Int32 result = 0;
    
    TDevFlash *self = (TDevFlash *)dev;
    
    if (sub_id < kDevFlashMaxCnt && buf != NULL)
    {
        result = TDrvFlashRead(self->drv_flash_, self->config_[sub_id].addr + self->r_[sub_id], buf, size);
        self->r_[sub_id] += result;
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
static Int32 TDevFlashWrite(TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len)
{
    Int32 result = 0; 
    TDevFlash *self = (TDevFlash *)dev;
    
    if (sub_id < kDevFlashMaxCnt)
    {
        if (buf != NULL)
        {
            result = TDrvFlashWrite(self->drv_flash_, self->config_[sub_id].addr + self->w_[sub_id], buf, len);
            self->w_[sub_id] += result;
        }
    }
    
    return result;
}

void DevFlashInit(void)
{
    /// 打开驱动设备
    TDrvFlashCreate(&g_drv_flash);
    TDrvFlashOpen(&g_drv_flash);

    TDevFlashCreate(&g_dev_flash_);
    g_dev_flash_.parent_.open_ = TDevFlashOpen;
    g_dev_flash_.parent_.close_ = TDevFlashClose;
    g_dev_flash_.parent_.ioctl_ = TDevFlashIoctl;
    g_dev_flash_.parent_.write_ = TDevFlashWrite;
    g_dev_flash_.parent_.read_ = TDevFlashRead;
    
    TVfsDevMgrRegister(&g_dev_mgr, (kSfDevNumFlash >> 8), (TVfsDevBase*)&g_dev_flash_);
}
