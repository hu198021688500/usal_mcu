/**
 * @file drv_dma_cfg.h
 * @brief DMA驱动配置 
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

#ifndef _DRV_DMA_CFG_H_
#define _DRV_DMA_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvDmaCfgTag TDrvDmaCfg;
struct TDrvDmaCfgTag
{
    UInt32           dma_idx;  ///< DMA选择(0~1)
    UInt32           chn;      ///< 通道号(0~7)
    UInt32           chn_item; ///< 功能项(0~7)
};

#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif ///< _DRV_DMA_CFG_H_
