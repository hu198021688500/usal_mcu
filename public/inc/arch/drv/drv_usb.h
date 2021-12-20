/**
 * @file drv_usb.h
 * @brief usb驱动
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
#ifndef _DRV_USB_H_
#define _DRV_USB_H_
#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvUsbDeviceStorageMethodTag TDrvUsbDeviceStorageMethod;
struct TDrvUsbDeviceStorageMethodTag
{
  Int8 (* Init) (UInt8 lun);
  Int8 (* GetCapacity) (UInt8 lun, UInt32 *block_num, UInt32 *block_size);
  Int8 (* IsReady) (UInt8 lun);
  Int8 (* IsWriteProtected) (UInt8 lun);
  Int8 (* Read) (UInt8 lun, UInt8 *buf, UInt32 blk_addr, UInt16 blk_len);
  Int8 (* Write)(UInt8 lun, UInt8 *buf, UInt32 blk_addr, UInt16 blk_len);
  Int8 (* GetMaxLun)(void);
  Int8 *pInquiry;
};

/**
 * 连接通知函数
 * @note: 由中断中调用
 * @param sender: 事件的产生者
 * @param is_connected: False:断开 True:连接
 * @return
 */
typedef void (*TDrvUsbIsConnected)(void *sender, Bool is_connected);


typedef struct TDrvUsbTag TDrvUsb;

struct TDrvUsbTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;
    
PUBLIC
    void                        *recevier;      ///< 事件接收者
    TDrvUsbIsConnected     MethodIsConnected;   ///< 中断事件;
    
PRIVATE
    UInt8       usb_index_;     ///< usb项
    Bool        is_host_;       ///< 是否主机
    Bool        is_connected_;  ///< 是否有连接
    
    TDrvUsbDeviceStorageMethod *MethodDeviceStorage;  ///< 设备存储回调
};

extern TDrvUsb g_drv_usb;

/**
 * 构造函数
 * @param self: 对象实例指针
 * @param usb_index: usb编号 0:fs 1:hs
 * @return
 */
void TDrvUsbCreate(TDrvUsb *self, UInt8 usb_index);

/**
 * 打开驱动
 * @note: 
 * @param self: 对象实例指针
 * @param is_host: 0:从机 1:主机
 * @param method_device_storage: 从机实现设备调用接口,主机为NULL
 * @return 打开成功返回true,否则为false
 */
Bool TDrvUsbOpen(TDrvUsb *self, Bool is_host, TDrvUsbDeviceStorageMethod *method_device_storage);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return
 */
void TDrvUsbClose(TDrvUsb *self);

/**
* 主机u盘读取
 * @param self: 对象实例指针
 * @param addr: 地址
 * @param buffer: 读取缓存
 * @param read_len: 读取长度
 * @return 0:失败 >=0:成功长度
 */
UInt32 TDrvUsbMasterRead(TDrvUsb *self, UInt32 addr, UInt8 *buffer, UInt32 read_len);
/**
 * 主机u盘写入
 * @param self: 对象实例指针
 * @param addr: 地址
 * @param data: 数据缓存
 * @param write_len: 写入长度
 * @return 0:失败 >=0:成功长度
 */
UInt32 TDrvUsbMasterWrite(TDrvUsb *self, UInt32 addr, const UInt8 *data, UInt32 write_len);
/**
 * 主机驱动循环处理
 * @param self: 对象实例指针
 * @return
 */
void TDrvUsbMasterRunOnce(TDrvUsb *self);
/**
 * 主机获取u盘容量(u盘连接时有效)
 * @param self: 对象实例指针
 * @return
 */
UInt32 TDrvUsbMasterGetCapacity(TDrvUsb *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /// _DRV_USB_H_
