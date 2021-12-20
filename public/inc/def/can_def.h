/**
 * @file can_def.h
 * @brief CAN通用数据定义
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
#ifndef _CAN_DEF_H_
#define _CAN_DEF_H_

#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 帧Id类型的枚举
 */
typedef enum TCanIdTypeTag TCanIdType;
enum TCanIdTypeTag
{
    kCanIdTypeStd,  ///< 标准ID
    kCanIdTypeExt,  ///< 扩展ID
};

/**
 * Can帧类型的枚举
 */
typedef enum TCanFrameTypeTag TCanFrameType;
enum TCanFrameTypeTag
{
    kCanFrameTypeData,  ///< 数据帧
    kCanFrameTypeRemote,  ///< 远程帧
};

/**
 * 收发的CAN数据定义
 */
typedef struct TCanDataTag TCanData;
struct TCanDataTag
{
    union
    {
        UInt32 recv_tick;  ///< 收到数据时的TICK值(仅接收时有效)
        UInt32 data_id;  ///< 数据发送完成或者失败时是否需要调用回调事件来告知.=0为不需要告知;否则为需要告知,data_id唯一标识一个报文
    } param;

    unsigned char bus_index;  ///< CAN设备索引
    unsigned char length;  ///< 数据长度
    unsigned char id_type;  ///< TCanIdType, 0: 标准帧, 1: 扩展ID
    unsigned char frame_type;  ///< TCanFrameType, 0: 数据帧, 1: 远程帧
    UInt32 id;  ///< CANID; 扩展帧ID: Bit28-Bit0; 标准帧: Bit10-Bit0
    unsigned char data[8];  ///< CAN数据
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _CAN_DEF_H_
