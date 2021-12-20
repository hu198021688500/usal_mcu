#include "drv_flash.h"
#include "stm32f4xx.h"
#include "stm32f4xx_flash.h"
#include <ofc_string.h>

TDrvFlash g_drv_flash;  ///< FLASH驱动对象

/// 页大小配置
typedef struct TDrvFlashPageSizeTag TDrvFlashPageSize;
struct TDrvFlashPageSizeTag
{
    UInt32   addr;
    UInt32   size;
    UInt32   page_idx;
};

/// 页大小
TDrvFlashPageSize g_flash_page_size[] = 
{
    {0x00000000, 0x4000, FLASH_Sector_0},       ///< page 0 16K
    {0x00004000, 0x4000, FLASH_Sector_1},       ///< page 1 16K
    {0x00008000, 0x4000, FLASH_Sector_2},       ///< page 2 16K
    {0x0000C000, 0x4000, FLASH_Sector_3},       ///< page 3 16K
    {0x00010000, 0x10000,FLASH_Sector_4},      ///< page 4 64K
    {0x00020000, 0x20000,FLASH_Sector_5},      ///< page 5 128K
    {0x00040000, 0x20000,FLASH_Sector_6},      ///< page 6 128K
    {0x00060000, 0x20000,FLASH_Sector_7},      ///< page 7 128K
    {0x00080000, 0x20000,FLASH_Sector_8},      ///< page 8 128K
    {0x000A0000, 0x20000,FLASH_Sector_9},      ///< page 9 128K
    {0x000C0000, 0x20000,FLASH_Sector_10},      ///< page 10 128K
    {0x000E0000, 0x20000,FLASH_Sector_11},      ///< page 11 128K
};

static const TDrvFlashConfig g_flash_cfg_ =
{
    .flash_base = FLASH_BASE,
    .flash_size = 100000,
    .flash_unit_size = 8,
    .page_size = 0,
};

TDrvFlashPageSize *TDrvFlashGetPageSize(TDrvFlash *self, UInt32 addr)
{
    TDrvFlashPageSize *result = NULL;
    UInt32 i = 0;

    for (i = 0; i < sizeof(g_flash_page_size) / sizeof(g_flash_page_size[0]); i++)
    {
        if ((addr == g_flash_page_size[i].addr)
        || (addr > g_flash_page_size[i].addr && addr < (g_flash_page_size[i].addr + g_flash_page_size[i].size)))
        {
            result = &g_flash_page_size[i];
            break;
        }
    }

    return result;
}

static void TDrvFlashDestroy(TOfObject *object)
{
    TDrvFlash *self = (TDrvFlash *)object;

    self->Destroy_(object);
}

void TDrvFlashCreate(TDrvFlash *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvFlashDestroy);

    self->cfg_ = &g_flash_cfg_;
    self->receiver = NULL;
    self->OnCmdPending = NULL;
}

Bool TDrvFlashOpen(TDrvFlash *self )
{
    return True;
}

void TDrvFlashClose(TDrvFlash *self )
{
    
}

Int32 TDrvFlashRead( TDrvFlash *self, Int32 offset, void *buf, Int32 size)
{
    Int32 result = 0;

    TDrvFlashPageSize * page_size = TDrvFlashGetPageSize(self, offset);
    if (page_size != NULL)
    {
        memmove(buf, (void*)(self->cfg_->flash_base + offset), size);
        result = size;
    }
    else
        result = 0;

    return result;
}

/**
 * Flash编程
 * @param self: 对象实例指针
 * @param offset: 起始位置
 * @param data: 数据
 * @param size: 数据大小
 * @return
 */
static Int32 TDrvFlashProgram(TDrvFlash *self, Int32 offset, const char *data, Int32 size)
{
    Int32 result = True;

    UInt32 i = 0;

    FLASH_Unlock();
    for (i = 0; i < size; i += 4)
    {
        if (FLASH_ProgramWord(i + offset + self->cfg_->flash_base, *(UInt32 *)(&data[i])) != FLASH_COMPLETE)
        {
            break;
        }
        else
            result += 4;
    }
    FLASH_Lock();

    return result;
}

Int32 TDrvFlashWrite(TDrvFlash *self, Int32 offset, const void *data, Int32 size)
{
    Int32 result = False;
    
    TDrvFlashPageSize * page_size = TDrvFlashGetPageSize(self, offset);
    if (page_size != NULL)
    {
        result = TDrvFlashProgram(self, offset, data, size);
    }
    // 返回结果
    return result;
}

/**
 * 执行Program Flash擦除命令
 * @param self: 对象实例指针
 * @param offset: 擦除页的起始位置
 * @param size: 擦除长度; 长度必须是page_size的整数倍
 * @return 擦除是否成功,true: 成功, false: 失败
 */
static Bool TDrvFlashDoErasePage(TDrvFlash *self, Int32 offset, Int32 size, TDrvFlashPageSize *page_size)
{
    Bool result = True;

    TDrvFlashPageSize *page_info = NULL;
    FLASH_Unlock();
    
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
    
    while (result && size > 0)
    {
        page_info = TDrvFlashGetPageSize(self, offset);
        if (page_info != NULL)
        {
            if (FLASH_EraseSector(page_info->page_idx, VoltageRange_3)  != FLASH_COMPLETE)
                result = False;
        }
        if (result)
        {
            offset += page_size->size;
            size -= page_size->size;
        }
    }
    FLASH_Lock();
    
    return result;
}

Bool TDrvFlashErasePage(TDrvFlash *self, Int32 offset, Int32 size)
{
    Bool result;

    TDrvFlashPageSize * page_size = TDrvFlashGetPageSize(self, offset);
    if (page_size != NULL && page_size->addr == offset)
    {
        result = TDrvFlashDoErasePage(self, offset, size, page_size);
    }
    
//    FLASH_Unlock();
//    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
//    FLASH_EraseSector(FLASH_Sector_6, VoltageRange_3);
//    FLASH_Lock();
    return result;
}
