/**
 * @file cmd_ctrl.h
 * @brief 命令-控制类
 * @note
 * @author
 * @date 2021-7-2
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-7-2    <td>1.0                   <td>                <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */
#ifndef _CMD_CTRL_H_
#define _CMD_CTRL_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 消息发送函数
 * @param receiver: 接口所有者
 * @param host_source: 主机数据源
 * @param ser_num: 流水号
 * @param cmd: 收到的命令
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return True:成功 False:失败
 */
typedef Bool (*TCmdCtrlProcessSendMsg)(void *receiver, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len);

typedef struct TCmdCtrlProcessTag TCmdCtrlProcess;
struct TCmdCtrlProcessTag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数
PUBLIC
    TCmdCtrlProcessSendMsg           method_msg_send_;     ///< 发送消息方法(调用者实现)
    void                            *receiver_;            ///< 接收者
PRIVATE
    
};

/**
 * 对象创建
 * @param self: 对象实例指针
 * @return 如果创建成功返回True,否则返回False
 */
void TCmdCtrlProcessCreate(TCmdCtrlProcess *self);

/**
 * 通知控制消息
 * @param self: 对象实例指针
 * @return
 */
Bool TCmdCtrlProcessNotifyMsg(TCmdCtrlProcess *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len);

#ifdef __cplusplus
}
#endif
#endif
