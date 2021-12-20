/**
 * @file srt.h
 * @brief 存储表记录
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

#ifndef _KERNEL_SRT_H_
#define _KERNEL_SRT_H_

#include "ofc_types.h"
#include "ofc_object.h"
#include "srt_config.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 存储记录表
 * @par 示例 
 * @code
    TSrtCreate(&g_srt);
    OfDestroyObject((TOfObject *)&g_srt);
 * @endcode
 */
typedef struct TSrtTag TSrt;

struct TSrtTag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    UInt8  plm_tab[kSrtPlmSize];    ///< 掉电不存储表
    UInt8  phm_tab[kSrtPhmSize];    ///< 掉电存储表
};

extern TSrt g_srt;                  ///< 存储单例模式对象

/**
 * 内存类的构造函数
 * @param self: 对象实例指针
 * @return 
 */
void TSrtCreate(TSrt *self);

///-----------------------plm掉电不存储接口------------------------///

/**
 * 设置Plm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param   bit: bit序列(0~31)如果4字节不对齐需要0~7
 * @param value: True或者False
 * @return True:成功 False:失败
 */
Bool TSrtPlmSetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool value);
/**
 * 设置Plm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPlmSetU08(TSrt *self, UInt32 addr, UInt8 value);
/**
 * 设置Plm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPlmSetU16(TSrt *self, UInt32 addr, UInt16 value);
/**
 * 设置Plm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPlmSetU32(TSrt *self, UInt32 addr, UInt32 value);
/**
 * 设置Plm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param  data: 数据地址
 * @param   len: 数据长度
 * @return True:成功 False:失败
 */
Bool TSrtPlmSetMem(TSrt *self, UInt32 addr, UInt8 *data, UInt32 len);


/**
 * 获取Plm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param   bit: bit序列(0~7)
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPlmGetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool* value);
/**
 * 获取Plm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPlmGetU08(TSrt *self, UInt32 addr, UInt8* value);
/**
 * 获取Plm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPlmGetU16(TSrt *self, UInt32 addr, UInt16* value);
/**
 * 获取Plm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPlmGetU32(TSrt *self, UInt32 addr, UInt32* value);
/**
 * 获取Plm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param buffer: 数据存放地址
 * @param  size: 数据存放长度
 * @return 0:失败 >0 :成功获取长度
 */
UInt32 TSrtPlmGetMem(TSrt *self, UInt32 addr, UInt8 *buffer, UInt32 size);
///-----------------------phm掉电存储接口------------------------///
/**
 * 设置Phm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param   bit: bit序列(0~7)
 * @param value: True或者False
 * @return True:成功 False:失败
 */
Bool TSrtPhmSetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool value);
/**
 * 设置Phm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPhmSetU08(TSrt *self, UInt32 addr, UInt8 value);
/**
 * 设置Phm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPhmSetU16(TSrt *self, UInt32 addr, UInt16 value);
/**
 * 设置Phm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param value: 设置值
 * @return True:成功 False:失败
 */
Bool TSrtPhmSetU32(TSrt *self, UInt32 addr, UInt32 value);
/**
 * 设置Phm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 设置地址
 * @param  data: 数据地址
 * @param   len: 数据长度
 * @return True:成功 False:失败
 */
Bool TSrtPhmSetMem(TSrt *self, UInt32 addr, UInt8 *data, UInt32 len);



/**
 * 获取Phm-bit值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param   bit: bit序列(0~7)
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPhmGetBit(TSrt *self, UInt32 addr, UInt32 bit, Bool* value);
/**
 * 获取Phm-单字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPhmGetU08(TSrt *self, UInt32 addr, UInt8* value);
/**
 * 获取Phm-双字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPhmGetU16(TSrt *self, UInt32 addr, UInt16* value);
/**
 * 获取Phm-四字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param value: 数据存放地址
 * @return True:成功 False:失败
 */
Bool TSrtPhmGetU32(TSrt *self, UInt32 addr, UInt32* value);
/**
 * 获取Phm-多字节值
 * @param  self: 对象实例指针
 * @param  addr: 读取地址
 * @param buffer: 数据存放地址
 * @param  size: 数据存放长度
 * @return 0:失败 >0 :成功获取长度
 */
UInt32 TSrtPhmGetMem(TSrt *self, UInt32 addr, UInt8 *buffer, UInt32 size);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_SRT_H_
