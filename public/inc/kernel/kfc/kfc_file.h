/**
 * @file kfc_file.h
 * @brief 文件操作接口(依赖fatfs和KfcMalloc)
 * @note 
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */

#ifndef _KFC_FILE_H_
#define _KFC_FILE_H_

#include <ofc_types.h>
#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 磁盘类型
 */
typedef enum TKfcFileDiskTypeTag TKfcFileDiskType;
enum TKfcFileDiskTypeTag
{
    kKfcFileDiskTypeExtFlash        = 0,        ///< 外部flash
    kKfcFileDiskTypeUsb,                        ///< u盘
    
    kKfcFileDiskTypeMax,
};

/**
 * 磁盘操作结果
 */
typedef enum TKfcFileResultTag TKfcFileResult;
enum TKfcFileResultTag
{
    kKfcFileResultOk        = 0,            ///< 成功
    kKfcFileResultDiskErr,                  ///< (1) A hard error occurred in the low level disk I/O layer */
    kKfcFileResultIntErr,                   ///< (2) Assertion failed
    kKfcFileResultNotReady,                 ///< (3) The physical drive cannot work
    kKfcFileResultNoFile,                   ///< (4) Could not find the file
    kKfcFileResultNoPath,                   ///< (5) Could not find the path
    kKfcFileResultInvalidName,              ///< (6) The path name format is invalid
    kKfcFileResultDenied,                   ///< (7) Access denied due to prohibited access or directory full
    kKfcFileResultExist,                    ///< (8) Access denied due to prohibited access
    kKfcFileResultInvalidObject,            ///< (9) The file/directory object is invalid
    kKfcFileResultWriteProtected,           ///< (10) The physical drive is write protected
    kKfcFileResultInvalidDrive,             ///< (11) The logical drive number is invalid
    kKfcFileResultNotEnabled,               ///< (12) The volume has no work area
    kKfcFileResultNoFilesystem,             ///< (13) There is no valid FAT volume
    kKfcFileResultMkfsAborted,              ///< (14) The f_mkfs() aborted due to any problem
    kKfcFileResultTimeout,                  ///< (15) Could not get a grant to access the volume within defined period
    kKfcFileResultLocked,                   ///< (16) The operation is rejected according to the file sharing policy
    kKfcFileResultNotEnoughCore,            ///< (17) LFN working buffer could not be allocated
    kKfcFileResultTooManyOpenFiles,         ///< (18) Number of open files > FF_FS_LOCK
    kKfcFileResultInvalidParameter,         ///< (19) Given parameter is invalid
    
    kKfcFileResultMax,
};

/**
 * 文件打开模式(多权限需要或运算)
 */
typedef enum TKfcFileOpenModeBitTag TKfcFileOpenModeBit;
enum TKfcFileOpenModeBitTag
{
    kKfcFileOpenModeBitRead            = 0x01,         ///< 指定读访问对象。可以从文件中读取数据。与 kKfcFileOpenModeWrite 结合可以进行读写访问
    kKfcFileOpenModeBitWrite           = 0x02,         ///< 指定写访问对象。可以向文件中写入数据。与 kKfcFileOpenModeRead 结合可以进行读写访问
    kKfcFileOpenModeBitOpenExisting    = 0x00,         ///< 打开文件。如果文件不存在，则打开失败。 ( 默认 )
    kKfcFileOpenModeBitCreateNew       = 0x04,         ///< 创建一个新文件。如果文件已存在，则创建失败
    kKfcFileOpenModeBitCreateAlways    = 0x08,         ///< 创建一个新文件。如果文件已存在，则它将被截断并覆盖
    kKfcFileOpenModeBitOpenAlways      = 0x10,         ///< 如果文件存在，则打开；否则，创建一个新文件
    kKfcFileOpenModeBitOpenAppend      = 0x30,         ///< 如果文件存在，则打开；否则，创建一个新文件(操作指针指向尾部)
};

/**
 * 文件句柄类型
 */
typedef void* KfcFileFd;

/**
 * 文件夹句柄类型
 */
typedef void* KfcDirFd;

/**
 * 文件信息属性(位标记,或运算)
 */
typedef enum TKfcFileInfoAttrBitTag TKfcFileInfoAttrBit;
enum TKfcFileInfoAttrBitTag
{
    kKfcFileInfoAttrBitReadOnly            = 0x01,         ///< 只读
    kKfcFileInfoAttrBitHidden              = 0x02,         ///< 隐藏
    kKfcFileInfoAttrBitSystem              = 0x04,         ///< 系统文件
    kKfcFileInfoAttrBitDir                 = 0x10,         ///< 文件夹
    kKfcFileInfoAttrBitArchive             = 0x20,         ///< 版本文件
};
/**
 * 文件信息(结构体与fatfs定义相同FILINFO)
 */
typedef struct TKfcFileInfoTag TKfcFileInfo;
struct TKfcFileInfoTag
{
	UInt32	fsize;			///< 文件大小
	UInt16	fdate;			///< 修改日期
	UInt16	ftime;			///< 修改时间
	UInt8	fattrib;	    ///< 文件属性 定义为TKfcFileInfoAttrBit
    
	Int8	altname[13];    /* Altenative file name */
	Int8	fname[256];	    /* Primary file name */
};

 /**
  * 文件操作举例
  * @par 示例
  * @code
    /// 系统初始化设置
    if (KfcFileMount(kKfcFileDiskTypeExtFlash) == kKfcFileResultNoFilesystem)
    {
        KfcFileMkfs(kKfcFileDiskTypeExtFlash);
        KfcFileMount(kKfcFileDiskTypeExtFlash);
    }
    KfcFileFd fd;
    KfcDirFd dir_fd;

    /// 新建文件夹
    KfcFileMkdir(kKfcFileDiskTypeExtFlash, "dir1");
    /// 删除文件夹需要先删除里面文件
    KfcFileUnlink(kKfcFileDiskTypeExtFlash, "dir1");

    /// 写文件操作
    if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &fd, "dir1/abc.txt", kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite | kKfcFileOpenModeBitOpenAlways) == kKfcFileResultOk)
    {
        /// KfcFileLSeek(&fd, 0);
        KfcFileWrite(&fd, &flash_write[0], sizeof(flash_write), &w_size);
        
        KfcFileClose(&fd);
    }
    /// 读文件操作
    if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &fd, "dir1/abc.txt", kKfcFileOpenModeBitRead) == kKfcFileResultOk)
    {
        ///KfcFileLSeek(&fd, 0);
        KfcFileRead(&fd, &flash_read[0], sizeof(flash_read), &r_size);
        
        KfcFileClose(&fd);
    }
    
    /// 读取文件夹目录信息
    if (KfcFileOpenDir(kKfcFileDiskTypeExtFlash, &dir_fd, "dir1") == kKfcFileResultOk)
    {
        while (KfcFileReadDir(&dir_fd, &file_info) == kKfcFileResultOk)
        {
            /// 循环处理,无文件后结束
        }
        KfcFileCloseDir(&dir_fd);
    }
  * @endcode
 */
/**
 * 磁盘是否就绪
 * @param  disk_type: 磁盘类型
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileDeskIsReady(TKfcFileDiskType disk_type);

/**
 * 磁盘挂载
 * @param  disk_type: 磁盘类型
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileMount(TKfcFileDiskType disk_type);
/**
 * 磁盘卸载
 * @param  disk_type: 磁盘类型
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileUnmount(TKfcFileDiskType disk_type);
/**
 * 格式化磁盘
 * @param  disk_type: 磁盘类型
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileMkfs(TKfcFileDiskType disk_type);

/**
 * 打开文件(打开后需要关闭,不然内存泄漏)
 * @param  disk_type: 磁盘类型
 * @param  fd: 文件句柄
 * @param  path: 文件路径 根目录下为"abc.txt"或者dir目录下为"dir/abc.txt"(有目录需要先新建目录,无目录返回kKfcFileResultNoPath)
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileOpen(TKfcFileDiskType disk_type, KfcFileFd *fd, Int8 *path, TKfcFileOpenModeBit open_mode);

/**
 * 关闭文件
 * @param  fd: 文件句柄
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileClose(KfcFileFd *fd);

/**
 * 文件读取
 * @param  fd: 文件句柄
 * @param  buffer: 缓存地址
 * @param  size: 缓存大小
 * @param  r_len: 实际读取成功个数, 返回值为kKfcFileResultOk有效
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileRead(KfcFileFd *fd, void *buffer, UInt32 size, UInt32 *r_len);

/**
 * 文件写入
 * @param  fd: 文件句柄
 * @param  data: 数据地址
 * @param  len: 数据大小
 * @param  w_len: 实际写入成功个数, 返回值为kKfcFileResultOk有效
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileWrite(KfcFileFd *fd, const void *data, UInt32 len, UInt32 *w_len);

/**
 * 修改读写指针位置
 * @param  fd: 文件句柄
 * @param  ofs: 位置值,0起始
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileLSeek(KfcFileFd *fd, UInt32 ofs);

/**
 * 文件同步
 * @param  fd: 文件句柄
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileSync(KfcFileFd *fd);

/**
 * 新建文件夹
 * @param  disk_type: 磁盘类型
 * @param  path: 文件夹路径,根目录为""空,其他为"dir","dir/dir1"
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileMkdir(TKfcFileDiskType disk_type, Int8 *path);

/**
 * 打开文件夹(打开后需要关闭,不然内存泄漏)
 * @param  disk_type: 磁盘类型
 * @param  dir_fd: 文件夹句柄
 * @param  path: 文件夹路径,根目录为""空,其他比如:"dir","dir/dir1"
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileOpenDir(TKfcFileDiskType disk_type, KfcDirFd *dir_fd, Int8 *path);

/**
 * 关闭文件夹
 * @param  dir_fd: 文件夹句柄
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileCloseDir(KfcDirFd *dir_fd);

/**
 * 读取文件夹文件信息(Opendir后每次读取返回1个有效文件信息,然后后移1个)
 * @param  dir_fd: 文件夹句柄
 * @param  info: 文件信息(返回成功时有效)
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileReadDir(KfcDirFd *dir_fd, TKfcFileInfo *info);

/**
 * 删除文件或者文件夹(删除文件夹,文件夹需为空,不然会被拒绝)
 * @param  disk_type: 磁盘类型
 * @param  path: 文件或者文件夹路径
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileUnlink(TKfcFileDiskType disk_type, Int8 *path);

/**
* 重命名文件或者文件夹
 * @param  disk_type: 磁盘类型
 * @param  path_old: 旧文件或者文件夹路径
 * @param  path_new: 新文件或者文件夹路径
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileRename(TKfcFileDiskType disk_type, Int8 *path_old, Int8 *path_new);

/**
 * 获取文件状态信息
 * @param  disk_type: 磁盘类型
 * @param  path: 文件路径
 * @param  info: 文件信息(返回成功时有效)
 * @return kKfcFileResultOk:成功 其他:失败
 */
TKfcFileResult KfcFileStat(TKfcFileDiskType disk_type, Int8 *path, TKfcFileInfo *info);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_FILE_H_
