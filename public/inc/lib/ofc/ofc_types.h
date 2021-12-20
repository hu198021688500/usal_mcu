/**
 * @file ofc_types.h
 * @brief 基础定义(在陆凯锋的基础上修改)
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

#ifndef _OFC_TYPES_H_
#define _OFC_TYPES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/* 常量列表 */

/// 空指针值
#ifndef NULL
#define NULL                    ((void *)0)
#endif

/// 常量值
#ifndef True
/// 布尔值假
#define False                   0
/// 布尔值真
#define True                    1
#endif

#ifndef TRUE
/// 布尔值假
#define FALSE                   0
/// 布尔值真
#define TRUE                    1
#endif

#ifndef OFFSET_OF
#define OFFSET_OF(T, I) ((long)&((T*)0)->I)
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/// 用于表示数据大小的类型
// armcc: 32位4字节,64位8字节
// gcc: 32位4字节,64位8字节
#if defined(__CC_ARM)
#if (__sizeof_ptr == 8)
    typedef unsigned long TSize;
#else
    typedef unsigned int TSize;
#endif
#else
#if (__SIZEOF_LONG__ == 8)
    typedef long unsigned int TSize;
#else
    typedef unsigned int TSize;
#endif
#endif

/* 类型定义 */
#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS) || defined(WINCE)
    typedef unsigned __int64 ___uint64_t;
    typedef __int64 ___int64_t;
#elif defined(__linux)
    typedef long long ___int64_t;
    typedef unsigned long long ___uint64_t;
#else
    typedef long long ___int64_t;
    typedef unsigned long long ___uint64_t;
#endif

/// 有符号整型
typedef ___int64_t Int64;
typedef signed int Int32;
typedef signed short Int16;
typedef signed char Int8;

/// 无符号整型
typedef  ___uint64_t UInt64;
typedef unsigned int UInt32;      
typedef unsigned short UInt16;       
typedef unsigned char UInt8; 

/// 无符号整型
typedef ___uint64_t QWord;
typedef unsigned int DWord;
typedef unsigned short Word;
typedef unsigned char Byte;

/// 布尔类型
typedef unsigned char Bool;

/// 用于表示时间的类型(同time_t)
typedef long TTime;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_OFC_TYPES_H_
