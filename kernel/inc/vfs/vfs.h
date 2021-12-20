/**
 * @file vfs.h
 * @brief 虚拟设备实现
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

#ifndef _KERNEL_VFS_H_
#define _KERNEL_VFS_H_

#include "ofc_types.h"
#include "ofc_object.h"
#include "dev_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 设备基础对象
 */  
typedef struct TVfsDevBaseTag TVfsDevBase;
    
typedef Bool  (*TVfsMethodOpen) (TVfsDevBase *dev, UInt8 sub_id);
typedef Bool  (*TVfsMethodClose) (TVfsDevBase *dev, UInt8 sub_id);
typedef Bool  (*TVfsMethodIoctl) (TVfsDevBase *dev, UInt8 sub_id, UInt32 cmd, void *argv);
typedef Int32  (*TVfsMethodRead) (TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len);    
typedef Int32  (*TVfsMethodWrite) (TVfsDevBase *dev, UInt8 sub_id, void *buf, Int32 len);
    
struct TVfsDevBaseTag
{
    TOfObject parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;  ///< 析构函数

PUBLIC
    TVfsMethodOpen     open_;       ///< 打开函数
    TVfsMethodClose    close_;      ///< 关闭函数
    TVfsMethodIoctl    ioctl_;      ///< 控制函数
    TVfsMethodRead     read_;       ///< 读取函数
    TVfsMethodWrite    write_;      ///< 写数据函数
};

/**
 * 构造函数
 * @param self: 对象实例指针
 * @return
 */
void TVfsDevBaseCreate(TVfsDevBase *self);

/**
 * 打开设备接口
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
Bool TVfsDevBaseOpen(TVfsDevBase *self, UInt8 sub_id);
/**
 * 关闭设备接口
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
Bool TVfsDevBaseClose(TVfsDevBase *self, UInt8 sub_id);
/**
 * 设备控制接口
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @param  cmd: 命令字
 * @param argv: 命令参数
 * @return True:成功 False:失败
 */
Bool TVfsDevBaseIoctl(TVfsDevBase *self, UInt8 sub_id, UInt32 cmd, void *argv);
/**
 * 设备读取接口
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 TVfsDevBaseRead(TVfsDevBase *self, UInt8 sub_id, void *buf, UInt32 len);
/**
 * 设备写入接口
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 TVfsDevBaseWrite(TVfsDevBase *self, UInt8 sub_id, void *buf, UInt32 len);





/**
 * 虚拟设备管理对象
 */
typedef struct TVfsDevMgrTag TVfsDevMgr;
struct TVfsDevMgrTag
{
    TOfObject parent_;          ///< 继承的父类
    TOfVirtualMethod Destroy_;  ///< 析构函数

PUBLIC
    
PRIVATE
    TVfsDevBase* dev_storage_[(kSfDevMax >> 8) + 1];        ///< 设备存储(最多255个)
};


extern TVfsDevMgr g_dev_mgr;        ///< 设备管理对象

/**
 * 构造函数
 * @param self: 对象实例指针
 * @return
 */
void TVfsDevMgrCreate(TVfsDevMgr *self);

/**
 * 注册设备
 * @param self: 对象实例指针
 * @param dev_id: 设备id(0~255)
 * @param  dev: 注册设备对象
 * @return True:成功 False:失败
 */
Bool TVfsDevMgrRegister(TVfsDevMgr *self, UInt8 dev_id, TVfsDevBase *dev);

/**
 * 卸载设备
 * @param self: 对象实例指针
 * @param dev_id: 设备id(0~255)
 * @return True:成功 False:失败
 */
Bool TVfsDevMgrUnregister(TVfsDevMgr *self, UInt8 dev_id);

/**
 * 打开设备
 * @param self: 对象实例指针
 * @param dev_id: 设备id(0~255)
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
Bool TVfsDevMgrOpenDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id);

/**
 * 关闭设备
 * @param  self: 对象实例指针
 * @param dev_id: 设备id(0~255)
 * @param sub_id: 子设备号
 * @return True:成功 False:失败
 */
Bool TVfsDevMgrCloseDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id);

/**
 * 设备控制
 * @param self: 对象实例指针
 * @param   fd: open后的设备句柄
 * @param  cmd: 命令字
 * @param argv: 命令参数
 * @return True:成功 False:失败
 */
Bool TVfsDevMgrIoctlDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, UInt32 cmd, void *argv);

/**
 * 设备读取
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 TVfsDevMgrReadDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, void *buf, UInt32 len);

/**
 * 设备写入
 * @param self: 对象实例指针
 * @param sub_id: 子设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 TVfsDevMgrWriteDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, void *buf, UInt32 len);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_VFS_H_

