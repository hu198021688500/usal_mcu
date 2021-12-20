/**
 * @file drv_pmu.h
 * @brief 电源管理驱动层
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
#ifndef _DRV_PMU_H_
#define _DRV_PMU_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 系统进入工作(刚启动调用一次)
 * @param 
 * @return
 */
void DrvPmuWork(void);

/**
 * 系统进入休眠模式
 * @param 
 * @return
 */
void DrvPmuSleep(void);

/**
 * 系统软件复位
 * @param 
 * @return
 */
void DrvPmuSoftReset(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_PMU_H_
