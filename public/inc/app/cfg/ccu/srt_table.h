/**
 * @file srt_table.h
 * @brief CCU 存储配置表配置
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

#ifndef _SRT_TABLE_H_
#define _SRT_TABLE_H_

#include <ofc_types.h>
#include "srt_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
* 掉电不保存配置表 
 */
typedef enum TSrtPlmItemTag TSrtPlmItem;
enum TSrtPlmItemTag
{
    kSrtPlmItemFlag             = 0,  ///< flag标记0x5A       len = 1
    
    
    kSrtPlmItemMax = kSrtPlmSize,
    
};

/*
* 掉电保存配置表 
 */
typedef enum TSrtPhmItemTag TSrtPhmItem;
enum TSrtPhmItemTag
{
    kSrtPhmItemFlag             = 0,  ///< flag标记0x5A       len = 1
    
    
    kSrtPhmItemMax = kSrtPhmSize,
    
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SRT_TABLE_H_
