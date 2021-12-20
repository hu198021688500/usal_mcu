/**
 * @file drv_can_cfg.c
 * @brief 用于can配置
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
const TDrvCanCfg g_drv_can_cfgs[] = 
{
    [0] = {
        .can_base = CAN1,
        .can_rx_port = &g_drv_port[kDrvPortNumG],
        .can_tx_port = &g_drv_port[kDrvPortNumG],
        .can_rx_pin = 0,
        .can_tx_pin = 1,
        .pin_mux = kPortMuxAlt9,
        .interrupt_irqn = CAN1_RX0_IRQn,
    },
    [1] = {
        .can_base = CAN2,
        .can_rx_port = &g_drv_port[kDrvPortNumG],
        .can_tx_port = &g_drv_port[kDrvPortNumG],
        .can_rx_pin = 11,
        .can_tx_pin = 12,
        .pin_mux = kPortMuxAlt9,
        .interrupt_irqn = CAN2_RX1_IRQn,
    },
    [2] = {
        .can_base = CAN3,
        .can_rx_port = &g_drv_port[kDrvPortNumA],
        .can_tx_port = &g_drv_port[kDrvPortNumA],
        .can_rx_pin = 8,
        .can_tx_pin = 15,
        .pin_mux = kPortMuxAlt11,
        .interrupt_irqn = OTG_HS_EP1_IN_CAN3_RX0_IRQn,
    },
};
//const TDrvCanCfg g_drv_can_cfgs[] = 
//{
//    [0] = {
//        .can_base = CAN1,
//        .can_rx_port = &g_drv_port[kDrvPortNumD],
//        .can_tx_port = &g_drv_port[kDrvPortNumD],
//        .can_rx_pin = 0,
//        .can_tx_pin = 1,
//        .pin_mux = kPortMuxAlt9,
//        .interrupt_irqn = CAN1_RX0_IRQn,
//    },
//    [1] = {
//        .can_base = CAN2,
//        .can_rx_port = &g_drv_port[kDrvPortNumB],
//        .can_tx_port = &g_drv_port[kDrvPortNumB],
//        .can_rx_pin = 12,
//        .can_tx_pin = 13,
//        .pin_mux = kPortMuxAlt9,
//        .interrupt_irqn = CAN2_RX1_IRQn,
//    },
//};
