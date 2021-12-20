/**
 * @file version.c
 * @brief 版本号
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
