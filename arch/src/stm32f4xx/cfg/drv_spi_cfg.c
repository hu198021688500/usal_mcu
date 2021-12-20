/**
 * @file drv_spi_cfg.c
 * @brief 用于spi配置
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

const TDrvSpiCfg g_drv_spi_cfgs[2] = 
{
    [0] = {
        .spi_index = 0,         ///< spi1
        .sck_port = &g_drv_port[kDrvPortNumB],
        .sck_pin  = 3,
        .mosi_port = &g_drv_port[kDrvPortNumB],
        .mosi_pin  = 5,
        .miso_port = &g_drv_port[kDrvPortNumB],
        .miso_pin  = 4,
        .spi_mux   = kPortMuxAlt5,
        .baud_div  = 1,
        .cpol      = 1,
        .cpha      = 1,
    },
    [1] = {
        .spi_index = 1,         ///< spi2
        .sck_port = &g_drv_port[kDrvPortNumB],
        .sck_pin  = 13,
        .mosi_port = &g_drv_port[kDrvPortNumB],
        .mosi_pin  = 15,
        .miso_port = &g_drv_port[kDrvPortNumB],
        .miso_pin  = 14,
        .spi_mux   = kPortMuxAlt5,
        .baud_div  = 1,
        .cpol      = 0,
        .cpha      = 0,
    },
};
