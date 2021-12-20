/**
 * @file drv_uart_cfg.c
 * @brief 用于uart配置
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
const TDrvUartCfg g_drv_uart_cfgs[] = 
{
    [0] = {
        .uart_base = UART8,
        .tx_port = &g_drv_port[kDrvPortNumF],
        .rx_port = &g_drv_port[kDrvPortNumF],
        .pin_mux = kPortMuxAlt8,
        .tx_pin = 9,
        .rx_pin = 8,
        .interrupt_irqn = UART8_IRQn,
    },
    [1] = {
        .uart_base = UART7,
        .tx_port = &g_drv_port[kDrvPortNumF],
        .rx_port = &g_drv_port[kDrvPortNumF],
        .pin_mux = kPortMuxAlt8,
        .tx_pin = 7,
        .rx_pin = 6,
        .interrupt_irqn = UART7_IRQn,
    },
    [2] = {
        .uart_base = USART3,
        .tx_port = &g_drv_port[kDrvPortNumD],
        .rx_port = &g_drv_port[kDrvPortNumD],
        .rts_port = &g_drv_port[kDrvPortNumD],
        .pin_mux = kPortMuxAlt7,
        .pin_rts_mux = kPortMuxDisabled,
        .tx_pin = 8,
        .rx_pin = 9,
        .rts_pin = 12,
        .interrupt_irqn = USART3_IRQn,
    },
    [3] = {
        .uart_base = USART6,
        .tx_port = &g_drv_port[kDrvPortNumC],
        .rx_port = &g_drv_port[kDrvPortNumC],
        .rts_port = &g_drv_port[kDrvPortNumG],
        .pin_mux = kPortMuxAlt8,
        .pin_rts_mux = kPortMuxDisabled,
        .tx_pin = 6,
        .rx_pin = 7,
        .rts_pin = 8,
        .interrupt_irqn = USART6_IRQn,
    },
};
