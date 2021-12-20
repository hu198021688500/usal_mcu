/**
 * @file drv_timer_cfg.h
 * @brief 定时器驱动配置 
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

#ifndef _DRV_TIMER_CFG_H_
#define _DRV_TIMER_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvTimerCfgTag TDrvTimerCfg;
struct TDrvTimerCfgTag
{
    Int32      timer_index;            ///< 定时器序列(0起始)
};

extern TDrvTimerCfg g_drv_timer_cfgs;

#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif ///< _DRV_TIMER_CFG_H_
