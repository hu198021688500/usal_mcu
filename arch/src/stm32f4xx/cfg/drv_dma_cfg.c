/**
 * @file drv_dma_cfg.c
 * @brief 用于dma配置
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
const TDrvDmaCfg g_drv_dma_cfgs[] = 
{
    [0] = {
        .dma_idx  = 1,
        .chn      = 0,
        .chn_item = 0,          ///< ADC1功能
    },
    [1] = {
        .dma_idx = 1,
        .chn     = 2,
        .chn_item = 1,          ///< ADC2功能
    },
    [2] = {
        .dma_idx = 1,
        .chn     = 1,
        .chn_item = 2,          ///< ADC3功能
    },
};
