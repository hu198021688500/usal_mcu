/**
 * @file drv_uart_cfg.h
 * @brief 串口驱动配置 
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

#ifndef _DRV_UART_CFG_H_
#define _DRV_UART_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvUartCfgTag TDrvUartCfg;
struct TDrvUartCfgTag
{
    USART_TypeDef   *uart_base;         //< Uart寄存器组基地址
    TDrvPort        *tx_port;           //< 发送引脚所在的端口
    TDrvPort        *rx_port;           //< 接收引脚所在的端口
    TDrvPort        *rts_port;          //< 485.rts引脚所在的端口
    Byte            tx_pin;             //< 发送引脚编号
    Byte            rx_pin;             //< 接收引脚编号
    Byte            rts_pin;            //< rts引脚编号
    TPortMux        pin_mux;            //< 引脚复用功能
    TPortMux        pin_rts_mux;        //< rts的引脚复用功能
    Byte            interrupt_irqn;     //<
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_UART_CFG_H_
