/**
 * @file srt_config.h
 * @brief 存储配置表配置
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

#ifndef _SFC_SRT_CONFIG_H_
#define _SFC_SRT_CONFIG_H_

#include <ofc_types.h>
#ifdef __cplusplus
extern "C" {
#endif
/*
 * 枚举配置 
 */
enum
{
    kSrtPlmSize         = 4096,        ///< 掉电不存储表大小 4k
    kSrtPhmSize         = 4096,        ///< 掉电存储表大小 4k
};
/*
 * 掉电不存储表 
 */
enum
{
    kSrtPlmItemFlag             = 0,   ///< 存储标记(应用勿动) length = 1
    
    
    kSrtPlmItemMaxSize =  kSrtPlmSize, ///< 最大值不能超过
};

/*
 * 掉电存储表
 */
enum
{
    kSrtPhmItemFlag             = 0,   ///< 存储标记(应用勿动) length = 1
    kSrtPhmReserved             = 1,   ///< length = 1
    kSrtPhmAppVersion           = 2,   ///< 程序版本高字节大版本号低字节小版本号 length = 2
    kSrtPhmDevNum               = 4,   ///< 10进制设备编号(2112019999) length = 4
    kSrtPhmSerialNumStr         = 8,   ///< 20字节序列号 length = 20
    kSrtPhmParamTabVersion      = 28,  ///< 参数表版本号高字节大版本号低字节小版本号 length = 2
    kSrtPhmCan0Baud             = 30,  ///< CAN0波特率 length = 4
    kSrtPhmCan1Baud             = 34,  ///< CAN1波特率 length = 4
    kSrtPhmCan2Baud             = 38,  ///< CAN2波特率 length = 4
                                       ///< 预留
    kSrtPhmProjectType          = 60,  ///< 项目类型 length = 1
    kSrtPhmModuleType           = 61,  ///< 模块类型 length = 1
    kSrtPhmModuleCnt            = 62,  ///< 模块数量 length = 1
    kSrtPhmContGroupType        = 62,  ///< 接触器群充类型 length = 1
    kSrtPhmChargeGunCnt         = 62,  ///< 充电枪数量 length = 1
    kSrtPhmChargeGunType        = 62,  ///< 充电枪类型 length = 1
    kSrtPhmBmsType              = 62,  ///< BMS类型 length = 1
    
    
    kSrtPhmItemMaxSize =  kSrtPhmSize, ///< 最大值不能超过
};


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SFC_SRT_CONFIG_H_
