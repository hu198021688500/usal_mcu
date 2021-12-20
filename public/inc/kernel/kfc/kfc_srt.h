/**
 * @file kfc_srt.h
 * @brief 设置存储配置表接口
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

#ifndef _KFC_SRT_H_
#define _KFC_SRT_H_

#include <ofc_types.h>
#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

///-----------------------plm掉电不存储接口------------------------///

/**
 * 设置Plm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param   bit: bit序列(0~7)
 * @param value: True或者False
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmSetBit(UInt32 addr, UInt32 bit, Bool value);
/**
 * 设置Plm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmSetU08(UInt32 addr, UInt8 value);
/**
 * 设置Plm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmSetU16(UInt32 addr, UInt16 value);
/**
 * 设置Plm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmSetU32(UInt32 addr, UInt32 value);
/**
 * 设置Plm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param  data: 数据地址
 * @param   len: 数据长度
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmSetMem(UInt32 addr, UInt8 *data, UInt32 len);




/**
 * 获取Plm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param   bit: bit序列(0~7)
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmGetBit(UInt32 addr, UInt32 bit, Bool* value);
/**
 * 获取Plm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmGetU08(UInt32 addr, UInt8* value);
/**
 * 获取Plm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmGetU16(UInt32 addr, UInt16* value);
/**
 * 获取Plm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPlmGetU32(UInt32 addr, UInt32* value);
/**
 * 获取Plm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param buffer: 数据存放地址
 * @param  size: 数据存放长度
 * @return 0:失败 >0 :成功获取长度
 */
UInt32 KfcSrtPlmGetMem(UInt32 addr, UInt8 *buffer, UInt32 size);
///-----------------------phm掉电存储接口------------------------///

/**
 * 设置Phm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param   bit: bit序列(0~7)
 * @param value: True或者False
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmSetBit(UInt32 addr, UInt32 bit, Bool value);
/**
 * 设置Phm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmSetU08(UInt32 addr, UInt8 value);
/**
 * 设置Phm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmSetU16(UInt32 addr, UInt16 value);
/**
 * 设置Phm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmSetU32(UInt32 addr, UInt32 value);
/**
 * 设置Phm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param  data: 数据地址
 * @param   len: 数据长度
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmSetMem(UInt32 addr, UInt8 *data, UInt32 len);



/**
 * 获取Phm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param   bit: bit序列(0~7)
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmGetBit(UInt32 addr, UInt32 bit, Bool* value);
/**
 * 获取Phm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmGetU08(UInt32 addr, UInt8* value);
/**
 * 获取Phm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmGetU16(UInt32 addr, UInt16* value);
/**
 * 获取Phm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool KfcSrtPhmGetU32(UInt32 addr, UInt32* value);
/**
 * 获取Phm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param buffer: 数据存放地址
 * @param  size: 数据存放长度
 * @return 0:失败 >0 :成功获取长度
 */
UInt32 KfcSrtPhmGetMem(UInt32 addr, UInt8 *buffer, UInt32 size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_SRT_H_
