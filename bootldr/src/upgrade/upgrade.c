#include "upgrade.h"
#include "drv_flash.h"
TBootldrUpgrade g_upgrade;  ///< 内存单例模式对象


// /// 页大小
// TDrvFlashPageSize g_flash_page_size[] = 
// {
//     {0x00000000, 0x4000},       ///< page 0 16K
//     {0x00004000, 0x4000},       ///< page 1 16K

//     {0x00008000, 0x4000},       ///< page 2 16K
//     {0x0000C000, 0x4000},       ///< page 3 16K
//     {0x00010000, 0x10000},      ///< page 4 64K

//     {0x00020000, 0x20000},      ///< page 5 128K
//     {0x00040000, 0x20000},      ///< page 6 128K
//     {0x00060000, 0x20000},      ///< page 7 128K
//     {0x00080000, 0x20000},      ///< page 8 128K


//     {0x000A0000, 0x20000},      ///< page 9 128K
//     {0x000C0000, 0x20000},      ///< page 10 128K
//     {0x000E0000, 0x20000},      ///< page 11 128K
// };


enum
{
    kAppAreaStart               = 0x00020000,       // APP区域的起始
    kAppAreaSize                = 0x40000,          // APP区域的尺寸
    kBackupAreaStart            = 0x00060000,       // 备份区域的起始
    kBackupAreaSize             = 0x40000,          // 备份区域的尺寸
    kInfoAreaStart              = 0x00004000,       // 信息区域的起始
    kInfoAreaSize               = 0x4000,           // 信息区域的尺寸
    kUpdateCount                = 3,                // 更新尝试次数 
    kQuadWordSize               = 8,                // 8字节对齐尺寸
    kAppSectorSize              = 0x20000,           // 扇区尺寸
};


typedef struct {                                    // 更新信息类型
    UInt32                    start_addr;         // APP起始地址
    UInt32                    data_len;           // 更新数据长度
    UInt16                    data_crc;           // 更新数据CRC
    UInt16                    reserve;            // 保留
}TInfo;


const TInfo * const g_ptr_info_ = (const TInfo *)kInfoAreaStart;    // 指向信息区


/**
 * 析构函数
 * @param object: 对象实例
 * @return 
 */
static void TBootldrUpgradeDestroy(TOfObject *object)
{
    TBootldrUpgrade *self = (TBootldrUpgrade *)object;

    self->Destroy_(object);
}

void TBootldrUpgradeCreate(TBootldrUpgrade *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TBootldrUpgradeDestroy);


}


Bool TBootldrUpgradeCheckIsUpdate(TBootldrUpgrade *self)
{
    Bool result = False;
    // 检查信息区
    if (g_ptr_info_->start_addr == kAppAreaStart
        && g_ptr_info_->data_len != 0
        && g_ptr_info_->data_len <= kAppAreaSize)
    {
        UInt16 crc_value = OfCalcCrc16((const char*)kBackupAreaStart, g_ptr_info_->data_len);
        // CRC比对
        if (crc_value == g_ptr_info_->data_crc)
            result = True;
    }
    /// 如果没有升级但是信息区不为空,擦除信息区
    if (result == False && *(const UInt32 *)kInfoAreaStart != 0xffffffff)
    {
        TDrvFlashErasePage(&g_drv_flash, kInfoAreaStart, kInfoAreaSize);
    }

    return result;    
}
/**
* 按8字节对齐
* @param ALen:          输入值
* @return uint32_t:     8字节对齐输出值
*/
static UInt32 AlignWithQuadWord(UInt32 a_len)
{
    return (a_len + 7) >> 3 << 3;
}

/// 拷贝备份区到app区域
static Bool TBootldrUpgradeCopyBackToApp(TBootldrUpgrade *self)
{
    Bool result = True;
    UInt32 begin = kAppAreaStart;
    UInt32 bak_begin = kBackupAreaStart;
    UInt32 end = kAppAreaStart + (g_ptr_info_->data_len / kAppSectorSize) * kAppSectorSize;
    UInt32 remain = AlignWithQuadWord(g_ptr_info_->data_len % kAppSectorSize);

    while (begin < end)
    {
        if (TDrvFlashWrite(&g_drv_flash, begin, (const void *)bak_begin, kAppSectorSize) <= 0)
        {
            result = False;
            break;
        }

        begin += kAppSectorSize;
        bak_begin += kAppSectorSize;
        //Delay(50);
    }
    // 拷贝剩余不足一个扇区部分
    if(result && remain > 0)
    {
        if (TDrvFlashWrite(&g_drv_flash, begin, (const void *)bak_begin, remain) <= 0)
        {
            result = False;
        }
        //Delay(50);
    }
    return result;
}
/// 校验app区域
static Bool TBootldrUpgradeCheckApp(TBootldrUpgrade *self)
{
    Bool result = False;

    UInt16 crc_value = OfCalcCrc16((const char*)kAppAreaStart, g_ptr_info_->data_len);
    // CRC比对
    if (crc_value == g_ptr_info_->data_crc)
        result = True;

    return result;
}

Bool TBootldrUpgradeCopyApp(TBootldrUpgrade *self)
{
    Bool result = False;
    UInt8 try_count = kUpdateCount;

    while(try_count > 0)
    {
        /// 擦除片区
        if (TDrvFlashErasePage(&g_drv_flash, kAppAreaStart, kAppAreaSize)
            && TBootldrUpgradeCopyBackToApp(self)
            && TBootldrUpgradeCheckApp(self))
        {
            break;
        }
        else
            try_count--;
    }

    // 升级成功,先擦除信息区,再擦除备份区
    if (try_count > 0 && TDrvFlashErasePage(&g_drv_flash, kInfoAreaStart, kInfoAreaSize))
    {
        //TDrvFlashErasePage(&g_drv_flash, kBackupAreaStart, kBackupAreaSize);
        result = True;
    }
    // Delay(50);
    return result;   
}




