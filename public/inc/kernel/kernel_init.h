/**
 * @file kernel_init.h
 * @brief 内核系统初始化
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

#ifndef _KERNEL_INIT_H_
#define _KERNEL_INIT_H_

#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 内核初始化
 * @param
 * @return
 */
void KernelInit(void);

/**
 * 内核调度开始
 * @param
 * @return True:成功 False:失败
 */ 
Bool KernelStart(void);

/**
 * 内核循环执行
 * @param
 * @return True:成功 False:失败
 */ 
void KernelRunOnce(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _APP_BASE_H_
