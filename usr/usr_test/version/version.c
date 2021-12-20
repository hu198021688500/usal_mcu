/**
 *  version.c
 *
 *  概述：Bootloader 版本
 *  作者：lukf
 *  版本：
 *  修订记录
 *  2018-03-09       lukf        创建文件
 *
 */

/// #include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    kVersionAddress              = 0xCFF8,
};

typedef struct
{
    unsigned char  major;
    unsigned char  minor;
    unsigned char  test;
    unsigned char  compile;
    unsigned int   compile_time;
}TVersion;

const TVersion bootloader_version __attribute__((at(kVersionAddress))) = 
{
    .major    = 0x02,
    .minor    = 0x05,
    .test     = 0x00,
    .compile  = 0x00,
    .compile_time  = 1573000888,
};

#ifdef __cplusplus
}
#endif
