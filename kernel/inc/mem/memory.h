/**
 * @file memory.h
 * @brief 内存分配
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

#ifndef _KERNEL_MEMORY_H_
#define _KERNEL_MEMORY_H_

#include "ofc_types.h"
#include "ofc_object.h"
#include "mfc_memory.h"
#include "mem_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 内存类
 * @par 示例 
 * @code
    TMemMemoryCreate(&g_memory);
    OfDestroyObject((TOfObject *)&g_memory);
 * @endcode
 */
typedef struct TMemMemoryTag TMemMemory;

struct TMemMemoryTag
{
PROTECTED
    TOfObject parent_;  ///< 继承的父类
    TOfVirtualMethod Destroy_;  ///< 析构函数

PRIVATE
    T1KBIndex  indexs_[kMemMemory1kCnt];  ///< 索引项的全局占用空间
    T1KBItem   items_[kMemMemory1kCnt];  ///< 内存项的全局占用空间
    TMemBlock  block_;  ///< 内存块列表
};

extern TMemMemory g_memory;  ///< 内存单例模式对象

/**
 * 内存类的构造函数
 * @param self: 对象实例指针
 * @return 
 */
void TMemMemoryCreate(TMemMemory *self);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_MEMORY_H_
