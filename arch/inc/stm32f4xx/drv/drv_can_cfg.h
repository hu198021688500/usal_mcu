/**
 * @file drv_can_cfg.h
 * @brief CAN驱动配置 
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

#ifndef _DRV_CAN_CFG_H_
#define _DRV_CAN_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvCanCfgTag TDrvCanCfg;
struct TDrvCanCfgTag
{
    CAN_TypeDef    *can_base;           //< Uart寄存器组基地址
    TDrvPort       *can_rx_port;         //< 接收引脚所在的端口
    TDrvPort       *can_tx_port;         //< 发送引脚所在的端口
    Byte            can_rx_pin;          //< 接收引脚编号
    Byte            can_tx_pin;          //< 发送引脚编号
    TPortMux        pin_mux;            //< 引脚复用功能
    Byte            interrupt_irqn;     //<
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_UART_CFG_H_
