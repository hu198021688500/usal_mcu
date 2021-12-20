#include "cmd_ctrl.h"
#include "hmi_protocol.h"

enum
{
    kHmiProtocolCtrlCmdHmiCtl    = 0,       ///< hmi交互使能
};

enum
{
    kHmiProtocolCtrlCmdResultOk    = 0,       ///< 应答结果成功
    kHmiProtocolCtrlCmdResultIdErr,           ///< 应答结果参数id错误
    kHmiProtocolCtrlCmdResultParamErr,        ///< 应答结果内容错误
};

#pragma pack(1)
/// 主机控制命令消息体
typedef struct THmiProtocolHostCtrlMsgTag THmiProtocolHostCtrlMsg;
struct THmiProtocolHostCtrlMsgTag
{
    UInt8       ctrl_cmd;              ///< 控制命令
    UInt8       reserved[3];           ///< 保留3字节
    UInt16      param1;                ///< 参数1
    UInt16      param2;                ///< 参数2
};
/// 从机控制应答消息体
typedef struct THmiProtocolSlaveCtrlRespMsgTag THmiProtocolSlaveCtrlRespMsg;
struct THmiProtocolSlaveCtrlRespMsgTag
{
    UInt8       ctrl_cmd;              ///< 控制命令
    UInt8       result;                ///< 控制结果0:成功 1:命令不支持 2:参数错误 其他:失败
    UInt8       reserved[2];           ///< 保留2字节
};
#pragma pack()

/**
 * 控制命令-交互命令
 * @param self: 对象实例
 * @param param1: 参数1
 * @param param2: 参数2
 * @return 
 */
static UInt8 TCmdCtrlProcessHmiCtrl(TCmdCtrlProcess *self, UInt16 param1, UInt16 param2)
{
    return 0;
}

/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TCmdCtrlProcessDestroy(TOfObject *object)
{
    TCmdCtrlProcess *self = (TCmdCtrlProcess *)object;
    self->Destroy_(object);
}

void TCmdCtrlProcessCreate(TCmdCtrlProcess *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TCmdCtrlProcessDestroy);
    self->method_msg_send_ = NULL;
    self->receiver_ = NULL;
}

Bool TCmdCtrlProcessNotifyMsg(TCmdCtrlProcess *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    THmiProtocolSlaveCtrlRespMsg resp_msg;
    resp_msg.ctrl_cmd = cmd;
    resp_msg.reserved[0] = 0;
    resp_msg.reserved[1] = 0;
    resp_msg.result = 1;
    if (data != NULL && data_len >= sizeof(THmiProtocolHostCtrlMsg))
    {
        THmiProtocolHostCtrlMsg *ctrl_msg = (THmiProtocolHostCtrlMsg *)data;
        
        if (ctrl_msg->ctrl_cmd == kHmiProtocolCtrlCmdHmiCtl)
        {
            resp_msg.result = TCmdCtrlProcessHmiCtrl(self, ctrl_msg->param1, ctrl_msg->param2);
        }
    }
    
    if (self->method_msg_send_ != NULL)
    {
        self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveCtrlResp, (UInt8 *)&resp_msg, sizeof(resp_msg));
    }
    
    return True;
}
