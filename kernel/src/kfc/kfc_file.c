#include "kfc_file.h"
#include "ff.h"
#include "diskio.h"
#include "stdio.h"
#include "kfc_memory.h"

/// fatfs对象
FATFS g_fatfs_[kKfcFileDiskTypeMax];

TKfcFileResult KfcFileDeskIsReady(TKfcFileDiskType disk_type)
{
    if (disk_status(disk_type) == 0)
        return kKfcFileResultOk;
    else
        return kKfcFileResultDiskErr;
}

TKfcFileResult KfcFileMount(TKfcFileDiskType disk_type)
{
    UInt8 str_temp[8];
    
    snprintf((char *)&str_temp[0], sizeof(str_temp), "%d:", disk_type);
    /// 挂载,操作模式为立刻
    return (TKfcFileResult)f_mount(&g_fatfs_[disk_type], (const TCHAR *)&str_temp[0], 1);
}

TKfcFileResult KfcFileUnmount(TKfcFileDiskType disk_type)
{
    UInt8 str_temp[8];
    
    snprintf((char *)&str_temp[0], sizeof(str_temp), "%d:", disk_type);
    /// 卸载
    return (TKfcFileResult)f_unmount((const TCHAR *)&str_temp[0]);
}

TKfcFileResult KfcFileMkfs(TKfcFileDiskType disk_type)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    UInt8 str_temp[8];
    UInt32 str_len;
    /// 文件系统格式化硬盘工作缓存,太大使用内存分配,用完即释放
    Int8 *f_work = KfcMalloc(FF_MAX_SS);
    
    str_len = snprintf((char *)&str_temp[0], sizeof(str_temp), "%d:", disk_type);
    str_temp[str_len] = '\0';
    
    if (f_work != NULL)
    {
        /// 按FAT32格式格式化
        result = (TKfcFileResult)f_mkfs((const TCHAR *)str_temp, 0, f_work, FF_MAX_SS);
        KfcFree(f_work);
    }
    return result;
}

TKfcFileResult KfcFileOpen(TKfcFileDiskType disk_type, KfcFileFd *fd, Int8 *path, TKfcFileOpenModeBit open_mode)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    FIL *f_fd = KfcMalloc(sizeof(FIL));
    if (f_fd != NULL)
    {
        UInt8 *str_temp = KfcMalloc(FF_MAX_LFN);
        if (str_temp != NULL)
        {
            UInt32 str_len;
            
            str_len = snprintf((char *)str_temp, FF_MAX_LFN, "%d:%s", disk_type, path);
            str_temp[str_len] = '\0';
            
            result = (TKfcFileResult)f_open(f_fd, (const TCHAR*)&str_temp[0], open_mode);
            if (result == kKfcFileResultOk)
            {
                *fd = f_fd;
            }
            else
            {
                KfcFree(f_fd);
            }
            KfcFree(str_temp);
        }
        else
        {
            KfcFree(f_fd);
        }
    }
    return result;
}

TKfcFileResult KfcFileClose(KfcFileFd *fd)
{
    FIL *f_fd = *fd;
    if (f_fd != NULL)
    {
        f_sync(f_fd);
        return (TKfcFileResult)f_close(f_fd);
    }
    else
        return kKfcFileResultInvalidParameter;
}

TKfcFileResult KfcFileRead(KfcFileFd *fd, void *buffer, UInt32 size, UInt32 *r_len)
{    
    FIL *f_fd = *fd;
    if (f_fd != NULL)
    {
        return (TKfcFileResult)f_read(f_fd, buffer, size, r_len);
    }
    else
        return
            kKfcFileResultInvalidParameter;
}

TKfcFileResult KfcFileWrite(KfcFileFd *fd, const void *data, UInt32 len, UInt32 *w_len)
{
    FIL *f_fd = *fd;
    if (f_fd != NULL)
    {
        return (TKfcFileResult)f_write(f_fd, data, len, w_len);
    }
    else
        return
            kKfcFileResultInvalidParameter;
}

TKfcFileResult KfcFileLSeek(KfcFileFd *fd, UInt32 ofs)
{
    FIL *f_fd = *fd;
    if (f_fd != NULL)
    {
        return (TKfcFileResult)f_lseek(f_fd, ofs);
    }
    else
        return
            kKfcFileResultInvalidParameter;
}


TKfcFileResult KfcFileSync(KfcFileFd *fd)
{
    FIL *f_fd = *fd;
    if (f_fd != NULL)
    {
        return (TKfcFileResult)f_sync(f_fd);
    }
    else
        return
            kKfcFileResultInvalidParameter;
}


TKfcFileResult KfcFileMkdir(TKfcFileDiskType disk_type, Int8 *path)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    UInt8 *str_temp = KfcMalloc(FF_MAX_LFN);
    if (str_temp != NULL)
    {
        UInt32 str_len;
        
        str_len = snprintf((char *)str_temp, FF_MAX_LFN, "%d:%s", disk_type, path);
        str_temp[str_len] = '\0';
        
        result = (TKfcFileResult)f_mkdir((const TCHAR*)str_temp);
        KfcFree(str_temp);
    }
    return result;
}

TKfcFileResult KfcFileOpenDir(TKfcFileDiskType disk_type, KfcDirFd *dir_fd, Int8 *path)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    DIR *f_dir_fd = KfcMalloc(sizeof(FIL));
    if (f_dir_fd != NULL)
    {
        UInt8 *str_temp = KfcMalloc(FF_MAX_LFN);
        if (str_temp != NULL)
        {
            UInt32 str_len;
            
            str_len = snprintf((char *)str_temp, FF_MAX_LFN, "%d:%s", disk_type, path);
            str_temp[str_len] = '\0';
            
            result = (TKfcFileResult)f_opendir(f_dir_fd, (const TCHAR*)str_temp);
            if (result == kKfcFileResultOk)
            {
                *dir_fd = f_dir_fd;
            }
            else
            {
                KfcFree(f_dir_fd);
            }
            KfcFree(str_temp);
        }
        else
        {
            KfcFree(f_dir_fd);
        }
    }
    return result;
}


TKfcFileResult KfcFileCloseDir(KfcDirFd *dir_fd)
{
    DIR *f_dir_fd = *dir_fd;
    if (f_dir_fd != NULL)
    {
        return (TKfcFileResult)f_closedir(f_dir_fd);
    }
    else
        return kKfcFileResultInvalidParameter;
}


TKfcFileResult KfcFileReadDir(KfcDirFd *dir_fd, TKfcFileInfo *info)
{
    DIR *f_dir_fd = *dir_fd;
    if (f_dir_fd != NULL)
    {
        return (TKfcFileResult)f_readdir(f_dir_fd, (FILINFO *)info);
        
        TKfcFileResult res = (TKfcFileResult)f_readdir(f_dir_fd, (FILINFO *)info);
        /// fatfs接口没有文件也会返回成功,但是文件名为空
        if (res == kKfcFileResultOk && info->fname[0] == 0)
        {
            res = kKfcFileResultNoFile;
        }
        return res;
    }
    else
        return kKfcFileResultInvalidParameter;
}


TKfcFileResult KfcFileUnlink(TKfcFileDiskType disk_type, Int8 *path)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    UInt8 *str_temp = KfcMalloc(FF_MAX_LFN);
    if (str_temp != NULL)
    {
        UInt32 str_len;
        
        str_len = snprintf((char *)str_temp, FF_MAX_LFN, "%d:%s", disk_type, path);
        str_temp[str_len] = '\0';
        
        result = (TKfcFileResult)f_unlink((const TCHAR*)str_temp);
        KfcFree(str_temp);
    }
    return result;
}


TKfcFileResult KfcFileRename(TKfcFileDiskType disk_type, Int8 *path_old, Int8 *path_new)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    UInt8 *str_temp1 = NULL;
    UInt8 *str_temp2 = NULL;
    
    str_temp1 = KfcMalloc(FF_MAX_LFN);
    if (str_temp1 != NULL)
    {
        UInt32 str_len;
        
        str_len = snprintf((char *)str_temp1, FF_MAX_LFN, "%d:%s", disk_type, path_old);
        str_temp1[str_len] = '\0';
        
        str_temp2 = KfcMalloc(FF_MAX_LFN);
        if (str_temp2 != NULL)
        {
            str_len = snprintf((char *)str_temp2, FF_MAX_LFN, "%d:%s", disk_type, path_new);
            str_temp2[str_len] = '\0';
            
            result = (TKfcFileResult)f_rename((const TCHAR*)str_temp1, (const TCHAR*)str_temp2);
            KfcFree(str_temp2);
        }
        KfcFree(str_temp1);
    }
    return result;
}

TKfcFileResult KfcFileStat(TKfcFileDiskType disk_type, Int8 *path, TKfcFileInfo *info)
{
    TKfcFileResult result = kKfcFileResultNotEnoughCore;
    
    UInt8 *str_temp = KfcMalloc(FF_MAX_LFN);
    if (str_temp != NULL)
    {
        UInt32 str_len;
        
        str_len = snprintf((char *)str_temp, FF_MAX_LFN, "%d:%s", disk_type, path);
        str_temp[str_len] = '\0';
        
        result = (TKfcFileResult)f_stat((const TCHAR*)str_temp, (FILINFO *)info);
        KfcFree(str_temp);
    }
    return result;
}
