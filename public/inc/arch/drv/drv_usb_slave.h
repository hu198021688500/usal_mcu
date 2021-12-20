///**
// * @file drv_usb_slave.h
// * @brief usb从机驱动
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
//#ifndef _DRV_USB_SLAVE_H_
//#define _DRV_USB_SLAVE_H_
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
//typedef void (*TDrvUsbSlaveIsConnected)(void *sender, Bool is_connected);


//typedef struct TDrvUsbSlaveTag TDrvUsbSlave;

//struct TDrvUsbSlaveTag
//{
//PROTECTED
//    TOfObject parent_;
//    TOfVirtualMethod Destroy_;
//    
//PUBLIC
//    void                        *recevier;      ///< 事件接收者
//    TDrvUsbSlaveIsConnected     MethodIsConnected;   ///< 中断事件;
//    
//PRIVATE
//    UInt8       usb_index_;     ///< usb项
//    Bool        is_connected_;  ///< 是否有连接
//};

//extern TDrvUsbSlave g_drv_usb_master;

///**
// * 构造函数
// * @param self: 对象实例指针
// * @param usb_index: usb编号 0:fs 1:hs
// * @return
// */
//void TDrvUsbSlaveCreate(TDrvUsbSlave *self, UInt8 usb_index);

///**
// * 打开驱动
// * @note: 
// * @param self: 对象实例指针
// * @return 打开成功返回true,否则为false
// */
//Bool TDrvUsbSlaveOpen(TDrvUsbSlave *self);

///**
// * 驱动关闭
// * @param self: 对象实例指针
// * @return
// */
//void TDrvUsbSlaveClose(TDrvUsbSlave *self);

//#ifdef __cplusplus
//}  // extern "C"
//#endif

//#endif /// _DRV_USB_SLAVE_H_
