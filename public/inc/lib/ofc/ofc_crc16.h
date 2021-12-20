/**
 * @file         ofc_crc16.h
 * @brief        CRC16相关的函数.
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
#ifndef _OFC_CRC16_H_
#define _OFC_CRC16_H_

#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 计算CRC16值(CRC-16/MODBUS算法, 多项式: 0x8005, 初始值0xFFFF)
 * @param data: 需要计算CRC的数据
 * @param size: 数据大小
 * @note 本函数计算结果与原RFC库中的结果高低字节相反
 * @return 计算出来的crc值
 */
UInt16 OfCalcCrc16(UInt8 *data, Int32 size); 

/**
 * 计算CRC16值(CRC-16算法/MODBUS算法, 多项式: 0x8005)
 * @param data: 需要计算CRC的数据
 * @param size: 数据大小
 * @param seed: 种子,即计算的起始值
 * @return 计算出来的crc值
 */
UInt16 OfCalcCrc16s(UInt8 *data, Int32 size, UInt16 seed); 

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _OFC_CRC16_H_
