/**
 * @file mem_config.h
 * @brief 内存管理配置
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

#ifndef _SFC_MEM_CONFIG_H_
#define _SFC_MEM_CONFIG_H_

#include <ofc_types.h>
#ifdef __cplusplus
extern "C" {
#endif
/*
 * 枚举配置 
 */
enum
{
    kMemMemory1kCnt         = 40,        ///< 内存分配占用几个1K空间
};
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SFC_MEM_CONFIG_H_
