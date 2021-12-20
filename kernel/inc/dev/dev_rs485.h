/**
 * @file dev_rs485.h
 * @brief RS485设备
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

#ifndef _KERNEL_DEV_RS485_H_
#define _KERNEL_DEV_RS485_H_

#include "ofc_types.h"
#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
/**
 * can设备初始化
 */  
void DevRs485Init(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_DEV_RS485_H_

