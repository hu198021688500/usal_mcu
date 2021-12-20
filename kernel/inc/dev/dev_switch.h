/**
 * @file dev_switch.h
 * @brief 开关量设备层
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

#ifndef _KERNEL_DEV_SWITCH_H_
#define _KERNEL_DEV_SWITCH_H_

#include "ofc_types.h"
#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif

    
    
/**
 * 开关量设备初始化
 */  
void DevSwitchInit(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_DEV_SWITCH_H_

