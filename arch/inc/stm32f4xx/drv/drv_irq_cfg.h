/**
 * @file drv_irq_cfg.h
 * @brief 中断优先级配置 
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

#ifndef _DRV_IRQ_CFG_H_
#define _DRV_IRQ_CFG_H_

#include "drv_port.h"
#include "drv_int.h"

#ifdef __cplusplus
extern "C" {
#endif

/// 抢占优先级组0
enum
{
    kDrvIrqCfgPreeCanRx0  = kSysIrqPreePriority0,           ///< can0Rx
    kDrvIrqCfgPreeCanRx1  = kSysIrqPreePriority0,           ///< can1Rx
    kDrvIrqCfgPreeCanRx2  = kSysIrqPreePriority0,           ///< can2Rx
    kDrvIrqCfgPreeCanTx0  = kSysIrqPreePriority0,           ///< can0Tx
    kDrvIrqCfgPreeCanTx1  = kSysIrqPreePriority0,           ///< can1Tx
    kDrvIrqCfgPreeCanTx2  = kSysIrqPreePriority0,           ///< can2Tx
    kDrvIrqCfgPreeRs485   = kSysIrqPreePriority0,          ///< Rs485中断
};
/// 响应优先级组0
enum
{
    kDrvIrqCfgSubCanRx0  = kSysIrqSubPriority0,            ///< can0Rx
    kDrvIrqCfgSubCanRx1  = kSysIrqSubPriority1,            ///< can1Rx
    kDrvIrqCfgSubCanRx2  = kSysIrqSubPriority2,            ///< can2Rx
    kDrvIrqCfgSubCanTx0  = kSysIrqSubPriority3,            ///< can0Tx
    kDrvIrqCfgSubCanTx1  = kSysIrqSubPriority4,            ///< can1Tx
    kDrvIrqCfgSubCanTx2  = kSysIrqSubPriority5,            ///< can2Tx
    kDrvIrqCfgSubRs485   = kSysIrqSubPriority6,            ///< Rs458中断
};



/// 抢占优先级组1
enum
{
    kDrvIrqCfgPreeExitInt = kSysIrqPreePriority1,           ///< io口中断
    kDrvIrqCfgPreeTimer   = kSysIrqPreePriority1,           ///< 定时器中断
    kDrvIrqCfgPreeUsb     = kSysIrqPreePriority1,           ///< usb中断
    kDrvIrqCfgPreeUart    = kSysIrqPreePriority1,           ///< 串口中断
};

/// 响应优先级组1
enum
{ 
    kDrvIrqCfgSubExitInt  = kSysIrqSubPriority0,           ///< io口中断
    kDrvIrqCfgSubTimer    = kSysIrqSubPriority1,           ///< 定时器中断
    kDrvIrqCfgSubUsb      = kSysIrqSubPriority2,           ///< usb中断
    kDrvIrqCfgSubUart     = kSysIrqSubPriority3,           ///< 串口中断
};



#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif ///< _DRV_IRQ_CFG_H_
