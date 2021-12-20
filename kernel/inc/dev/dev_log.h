/**
 * @file dev_log.h
 * @brief 串口日志层
 * @note
 * @author zgq
 * @date 2020-08-04
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2020-08-04  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */

#ifndef _KERNEL_DEV_LOG_H_
#define _KERNEL_DEV_LOG_H_

#include "ofc_types.h"
#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
/**
 * can设备初始化
 */  
void DevLogInit(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_DEV_LOG_H_

