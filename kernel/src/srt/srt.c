#include "srt.h"
#include "rtos_api.h"
#include "pfc_sync_objs.h"
#include "string.h"
#include "kfc_file.h"

TSrt g_srt;  ///< 存储单例模式对象

KfcFileFd g_file_fd_;

#define SRT_PHM_FILE_NAME "srt_phm.bin"
#define SRT_THM_FILE_TAG 0x5A
/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TSrtDestroy(TOfObject *object)
{
    TSrt *self = (TSrt *)object;

    self->Destroy_(object);
}

void TSrtCreate(TSrt *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TSrtDestroy);
    
    memset(&self->plm_tab[0], 0, sizeof(self->plm_tab));
    /// 没有配置文件
    if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &g_file_fd_, (Int8 *)SRT_PHM_FILE_NAME, kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite) != kKfcFileResultOk)
    {
        /// 创建文件
        if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &g_file_fd_, (Int8 *)SRT_PHM_FILE_NAME, kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite | kKfcFileOpenModeBitOpenAlways) == kKfcFileResultOk)
        {
            UInt32 w_size;
            memset(&self->phm_tab[0], 0, sizeof(self->phm_tab));
            KfcFileLSeek(&g_file_fd_, 0);
            KfcFileWrite(&g_file_fd_, &self->phm_tab[0], sizeof(self->phm_tab), &w_size);
            KfcFileClose(&g_file_fd_);
            TSrtPhmSetU08(self, kSrtPhmItemFlag, SRT_THM_FILE_TAG);            
        }
    }
    /// 有文件则导入
    else
    {
        UInt32 r_size;
        UInt32 w_size;
        UInt8 flag;
        KfcFileLSeek(&g_file_fd_, 0);
        KfcFileRead(&g_file_fd_, &self->phm_tab[0], sizeof(self->phm_tab), &r_size);
        
        if (TSrtPhmGetU08(self, kSrtPhmItemFlag, &flag) && flag != SRT_THM_FILE_TAG)
        {
            memset(&self->phm_tab[0], 0, sizeof(self->phm_tab));
            
            KfcFileLSeek(&g_file_fd_, 0);
            KfcFileWrite(&g_file_fd_, &self->phm_tab[0], sizeof(self->phm_tab), &w_size);
        }
        KfcFileClose(&g_file_fd_);
    }
}

///-----------------------------------------plm-------------------------------------------///

Bool TSrtPlmSetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool value)
{
    Bool result = False;
    
    /// 4字节对齐可以直接操作
    if (addr % 4 == 0)
    {
        if (value)
            PfInterlockedOr((Int32 *)&self->plm_tab[addr], (Int32)(1 << bit));
        else
            PfInterlockedAnd((Int32 *)&self->plm_tab[addr], (Int32)~(1 << bit));
        
        result = True;
    }
    /// 如果不对齐只操作单字节
    else if (bit < 8)
    {
        RtosEnterCriticalSection();
        
        if (value)
            self->plm_tab[addr] |= 1 << bit;
        else
            self->plm_tab[addr] &= ~(1 << bit);
            
        RtosLeaveCriticalSection();
        
        result = True;
    }
    return result;
}

Bool TSrtPlmSetU08(TSrt *self, UInt32 addr, UInt8 value)
{
    RtosEnterCriticalSection();
    self->plm_tab[addr] = value;
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPlmSetU16(TSrt *self, UInt32 addr, UInt16 value)
{
    RtosEnterCriticalSection();
    *(UInt16 *)&self->plm_tab[addr] = value;
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPlmSetU32(TSrt *self, UInt32 addr, UInt32 value)
{
    RtosEnterCriticalSection();
    *(UInt32 *)&self->plm_tab[addr] = value;
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPlmSetMem(TSrt *self, UInt32 addr, UInt8 *data, UInt32 len)
{
    RtosEnterCriticalSection();
    memcpy(&self->plm_tab[addr], data, len);
    RtosLeaveCriticalSection();
    return True;
}

Bool TSrtPlmGetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool* value)
{    
    if (bit < 8)
    {
        UInt8 u8_value;
        RtosEnterCriticalSection();
        u8_value = self->plm_tab[addr];
        RtosLeaveCriticalSection();
        if (u8_value & (1 << bit))
            *value = True;
        else
            *value = False;
    }
    else
    {
        UInt32 u32_value;
        RtosEnterCriticalSection();
        u32_value = *(UInt32 *)&self->plm_tab[addr];
        RtosLeaveCriticalSection();
        if (u32_value & (1 << bit))
            *value = True;
        else
            *value = False;
    }
    return True;
}


Bool TSrtPlmGetU08(TSrt *self, UInt32 addr, UInt8* value)
{
    RtosEnterCriticalSection();
    *value = self->plm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPlmGetU16(TSrt *self, UInt32 addr, UInt16* value)
{
    RtosEnterCriticalSection();
    *value = *(UInt16 *)&self->plm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPlmGetU32(TSrt *self, UInt32 addr, UInt32* value)
{
    RtosEnterCriticalSection();
    *value = *(UInt32 *)&self->plm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

UInt32 TSrtPlmGetMem(TSrt *self, UInt32 addr, UInt8 *buffer, UInt32 size)
{
    RtosEnterCriticalSection();
    memcpy(buffer, &self->plm_tab[addr], size);
    RtosLeaveCriticalSection();
    return size;
}

///-----------------------------------------phm-------------------------------------------///

/**
 * 同步phm到文件操作
 * @param  self: 对象实例指针
 * @return True:成功 False:失败
 */
static Bool TSrtPhmSyncFile(TSrt *self)
{
    KfcFileFd fd;
    if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &fd, (Int8 *)SRT_PHM_FILE_NAME, kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite | kKfcFileOpenModeBitOpenAlways) == kKfcFileResultOk)
    {
        UInt32 w_size;
        KfcFileLSeek(&fd, 0);
        KfcFileWrite(&fd, &self->phm_tab[0], sizeof(self->phm_tab), &w_size);
        KfcFileClose(&fd);
    }
}

Bool TSrtPhmSetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool value)
{
    Bool result = False;
    Bool now_value;
    
    if (TSrtPhmGetBit(self, addr, bit, &now_value))
    {
        if (now_value != value)
        {
            /// 4字节对齐可以直接操作
            if (addr % 4 == 0)
            {
                if (value)
                    PfInterlockedOr((Int32 *)&self->phm_tab[addr], (Int32)(1 << bit));
                else
                    PfInterlockedAnd((Int32 *)&self->phm_tab[addr], (Int32)~(1 << bit));
            }
            /// 如果不对齐只操作单字节
            else if (bit < 8)
            {
                RtosEnterCriticalSection();
                
                if (value)
                    self->phm_tab[addr] |= 1 << bit;
                else
                    self->phm_tab[addr] &= ~(1 << bit);
                    
                RtosLeaveCriticalSection();
            }
            TSrtPhmSyncFile(self);
            result = True;
        }
    }
    
    return result;
}

Bool TSrtPhmSetU08(TSrt *self, UInt32 addr, UInt8 value)
{
    Bool result = False;
    UInt8 now_value;
    
    if (TSrtPhmGetU08(self, addr, &now_value))
    {
        if (value != now_value)
        {
            RtosEnterCriticalSection();
            self->phm_tab[addr] = value;
            RtosLeaveCriticalSection();
            TSrtPhmSyncFile(self);
        }
        result = True;
    }
    
    return result;
}

Bool TSrtPhmSetU16(TSrt *self, UInt32 addr, UInt16 value)
{    
    Bool result = False;
    UInt16 now_value;
    
    if (TSrtPhmGetU16(self, addr, &now_value))
    {
        if (value != now_value)
        {
            RtosEnterCriticalSection();
            *(UInt16 *)&self->phm_tab[addr] = value;
            RtosLeaveCriticalSection();
            TSrtPhmSyncFile(self);
        }
        result = True;
    }
    
    return result;
}

Bool TSrtPhmSetU32(TSrt *self, UInt32 addr, UInt32 value)
{
    Bool result = False;
    UInt32 now_value;
    
    if (TSrtPhmGetU32(self, addr, &now_value))
    {
        if (value != now_value)
        {
            RtosEnterCriticalSection();
            *(UInt32 *)&self->phm_tab[addr] = value;
            RtosLeaveCriticalSection();
            TSrtPhmSyncFile(self);
        }
        result = True;
    }
    
    return result;
}

Bool TSrtPhmSetMem(TSrt *self, UInt32 addr, UInt8 *data, UInt32 len)
{
    RtosEnterCriticalSection();
    memcpy(&self->phm_tab[addr], data, len);
    RtosLeaveCriticalSection();
    TSrtPhmSyncFile(self);
    return True;
}

Bool TSrtPhmGetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool* value)
{
    if (bit < 8)
    {
        UInt8 u8_value;
        RtosEnterCriticalSection();
        u8_value = self->phm_tab[addr];
        RtosLeaveCriticalSection();
        if (u8_value & (1 << bit))
            *value = True;
        else
            *value = False;
    }
    else
    {
        UInt32 u32_value;
        RtosEnterCriticalSection();
        u32_value = *(UInt32 *)&self->phm_tab[addr];
        RtosLeaveCriticalSection();
        if (u32_value & (1 << bit))
            *value = True;
        else
            *value = False;
    }
    return True;
}


Bool TSrtPhmGetU08(TSrt *self, UInt32 addr, UInt8* value)
{
    RtosEnterCriticalSection();
    *value = self->phm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPhmGetU16(TSrt *self, UInt32 addr, UInt16* value)
{
    RtosEnterCriticalSection();
    *value = *(UInt16 *)&self->phm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

Bool TSrtPhmGetU32(TSrt *self, UInt32 addr, UInt32* value)
{
    RtosEnterCriticalSection();
    *value = *(UInt32 *)&self->phm_tab[addr];
    RtosLeaveCriticalSection();
    
    return True;
}

UInt32 TSrtPhmGetMem(TSrt *self, UInt32 addr, UInt8 *buffer, UInt32 size)
{
    RtosEnterCriticalSection();
    memcpy(buffer, &self->phm_tab[addr], size);
    RtosLeaveCriticalSection();
    return size;
}




