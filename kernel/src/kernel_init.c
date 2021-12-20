#include "kernel_init.h"
#include "rtos_api.h"
#include "memory.h"
#include "vfs.h"
#include "srt.h"
#include "drv_clock.h"
#include "drv_int.h"
#include "dev_init_cfg.c"
#include "drv_ext_int.h"
#include "kfc_timer.h"
#include "drv_flash.h"
#include "kfc_file.h"
#include "drv_usb_master.h"
#include "diskio.h"


void KernelInit(void)
{
    TDrvClockCreate(&g_drv_clock);
    TDrvClockInitClock(&g_drv_clock);
    SysIRQInit();
    /// 内核初始化
    RtosKernelInit();
    /// 内存管理初始化
    TMemMemoryCreate(&g_memory);
    /// 设备管理器初始化
    TVfsDevMgrCreate(&g_dev_mgr);
    /// 电源设备初始化(初始化GPIO)
    DevPmuInit();
    /// 定时器打开
    TKfcTimerSrvCreate(&g_kfc_timer_srv);
    
    disk_create(kDiskTypeExtFlash);
    disk_create(kDiskTypeUsb);
    
    /// 文件系统初始化
    if (KfcFileMount(kKfcFileDiskTypeExtFlash) == kKfcFileResultNoFilesystem)
    {
        KfcFileMkfs(kKfcFileDiskTypeExtFlash);
        KfcFileMount(kKfcFileDiskTypeExtFlash);
    }
    /// srt配置表创建
    TSrtCreate(&g_srt);
    
    /// 驱动设备加载初始化(dev_init.c里)
    DevInitCfg();

}

Bool KernelStart(void)
{
    Bool result = False;
    if (RtosKernelReady())
    {
        TKfcTimerSrvOpen(&g_kfc_timer_srv);   ///< 定时器打开
        
        result = RtosKernelStart();
    }
    return result;
}

void KernelRunOnce(void)
{
    disk_run_once(kDiskTypeExtFlash);
    disk_run_once(kKfcFileDiskTypeUsb);
}

