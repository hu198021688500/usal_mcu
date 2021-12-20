/**
 * @file         ofc_crc32.h
 * @brief        CRC32相关的函数.
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

#ifndef _OFC_CRC32_H_
#define _OFC_CRC32_H_

#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * CRC32计算函数.初始值为0xFFFFFFFF
 * @param src: 需要计算crc的数据起始地址
 * @param size: 字节数
 * @note 此接口获取的非标准的CRC码，取反后=标准的CRC32码
 * @return 计算的crc结果码
 */
UInt32 OfCalcCrc32(UInt8 *src, Int32 size);

/**
 * CRC32计算函数.初始值由crc输入,结果输出到crc
 * @param src: 需要计算crc的数据起始地址
 * @param size: 字节数
 * @param seed: 输入crc的起始值,输出最终的crc值
 * @note 此接口获取的非标准的CRC码，取反后=标准的CRC32码
 * @return 计算的crc结果码
 */
UInt32 OfCalcCrc32s(UInt8 *src, Int32 size, UInt32 seed);


#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _OFC_CRC32_H_
