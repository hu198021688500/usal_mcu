/**
 * @file hmi_task.h
 * @brief 交互主任务
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

#ifndef _APP_HMI_TASK_H_
#define _APP_HMI_TASK_H_

#include <ofc_object.h>
#include <kfc_task.h>
#include "hmi_protocol.h"
#include "cmd_ctrl.h"
#include "cmd_param_set.h"
#include "cmd_param_req.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 主任务类
 * @par 示例 
 * @code
    THmiTask test;

    THmiTaskCreate(&test);
    OfDestroyObject((TOfObject *)&test);
 * @endcode
 */
typedef struct THmiTaskTag THmiTask;
struct THmiTaskTag
{
PROTECTED
    TKfcTask parent_;                ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    
    THmiProtocol    hmi_protocol_;  ///< 协议处理
    UInt8           protocol_buffer_[512]; ///< 协议解析缓存buffer
    UInt8           protocol_send_buffer_[200]; ///< 协议发送缓存
    UInt8           com_read_buf_[512];
    
    TCmdCtrlProcess     cmd_ctrl_process_;
    TCmdParamSetProcess cmd_param_set_process_;
    TCmdParamReqProcess cmd_param_req_process_;
};
/// 主任务对象
extern THmiTask g_hmi_task;

/**
 * 主任务创建
 * @param self: 对象实例指针
 * @return 
 */
void THmiTaskCreate(THmiTask *self);


#ifdef __cplusplus
}
#endif 

#endif //_APP_UART_TASK_H_
