/**
 * @file dev_flash.h
 * @brief flash设备层
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

#ifndef _KERNEL_DEV_FLASH_H_
#define _KERNEL_DEV_FLASH_H_

#include "ofc_types.h"
#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    
/**
 * flash设备初始化
 */  
void DevFlashInit(void);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_DEV_FLASH_H_

