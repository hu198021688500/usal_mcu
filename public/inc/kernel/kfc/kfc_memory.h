/**
 * @file kfc_memory.h
 * @brief 动态内存分配接口
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

#ifndef _KFC_MEMORY_H_
#define _KFC_MEMORY_H_

#include <ofc_types.h>
#include <ofc_object.h>
#include "dev_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 系统内存分配函数
 * @param size: 分配内存的大小
 * @return 内存分配成功,返回分配的内存指针,否则返回NULL
 */
void *KfcMalloc(Int32 size);

/**
 * 系统内存释放函数
 * @param ptr: 释放内存的指针
 * @return 
 */
void KfcFree(void *ptr);

    
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_MEMORY_H_
