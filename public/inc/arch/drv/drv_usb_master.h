///**
// * @file drv_usb_master.h
// * @brief usb主机驱动
// * @note 
// * @author zgq
// * @date 2021-10-24
// * @version 1.0

// * @par          修改日志
// * <table>
// * <tr><th>Date        <th>Version               <th>Author          <th>Description
// * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
//  * @copyright<mrr_zhang@qq.com>
// */
//#ifndef _DRV_USB_MASTER_H_
//#define _DRV_USB_MASTER_H_
//#include <ofc_object.h>
//#include <ofc_types.h>

//#ifdef __cplusplus
//extern "C" {
//#endif


///**
// * 连接通知函数
// * @note: 由中断中调用
// * @param sender: 事件的产生者
// * @param is_connected: False:断开 True:连接
// * @return
// */
//typedef void (*TDrvUsbMasterIsConnected)(void *sender, Bool is_connected);


//typedef struct TDrvUsbMasterTag TDrvUsbMaster;

//struct TDrvUsbMasterTag
//{
//PROTECTED
//    TOfObject parent_;
//    TOfVirtualMethod Destroy_;
//    
//PUBLIC
//    void                        *recevier;      ///< 事件接收者
//    TDrvUsbMasterIsConnected     MethodIsConnected;   ///< 中断事件;
//    
//PRIVATE
//    UInt8       usb_index_;     ///< usb项
//    Bool        is_connected_;  ///< 是否有连接
//};

//extern TDrvUsbMaster g_drv_usb_master;

///**
// * 构造函数
// * @param self: 对象实例指针
// * @param usb_index: usb编号 0:fs 1:hs
// * @return
// */
//void TDrvUsbMasterCreate(TDrvUsbMaster *self, UInt8 usb_index);

///**
// * 打开驱动
// * @note: 
// * @param self: 对象实例指针
// * @return 打开成功返回true,否则为false
// */
//Bool TDrvUsbMasterOpen(TDrvUsbMaster *self);

///**
// * 驱动关闭
// * @param self: 对象实例指针
// * @return
// */
//void TDrvUsbMasterClose(TDrvUsbMaster *self);
///**
// * u盘读取
// * @param self: 对象实例指针
// * @param addr: 地址
// * @param buffer: 读取缓存
// * @param read_len: 读取长度
// * @return 0:失败 >=0:成功长度
// */
//UInt32 TDrvUsbMasterRead(TDrvUsbMaster *self, UInt32 addr, UInt8 *buffer, UInt32 read_len);
///**
// * u盘写入
// * @param self: 对象实例指针
// * @param addr: 地址
// * @param data: 数据缓存
// * @param write_len: 写入长度
// * @return 0:失败 >=0:成功长度
// */
//UInt32 TDrvUsbMasterWrite(TDrvUsbMaster *self, UInt32 addr, const UInt8 *data, UInt32 write_len);
///**
// * 驱动循环处理
// * @param self: 对象实例指针
// * @return
// */
//void TDrvUsbMasterRunOnce(TDrvUsbMaster *self);
///**
// * 获取u盘容量(u盘连接时有效)
// * @param self: 对象实例指针
// * @return
// */
//UInt32 TDrvUsbMasterGetCapacity(TDrvUsbMaster *self);

//#ifdef __cplusplus
//}  // extern "C"
//#endif

//#endif /// _DRV_USB_Master_H_
