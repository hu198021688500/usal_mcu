#include "hmi_task.h"
#include "kfc_system.h"
#include "kfc_dev.h"

/// 主任务对象
THmiTask g_hmi_task;

/**
 * 消息处理
 * @param self: 对象实例
 * @param host_source: 主机数据源
 * @param ser_num: 流水号
 * @param cmd: 收到的命令
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return 无
 */
static void THmiTaskMsgProcess(THmiTask *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    switch(cmd)
    {
        case kHmiProtocolCmdHostCtrl:
            TCmdCtrlProcessNotifyMsg(&self->cmd_ctrl_process_, host_source, ser_num, cmd, data, data_len);
            break;
        case kHmiProtocolCmdHostParamSet:
            TCmdParamSetProcessNotifyMsg(&self->cmd_param_set_process_, host_source, ser_num, cmd, data, data_len);
            break;
        case kHmiProtocolCmdHostParamReq:
            TCmdParamReqProcessNotifyMsg(&self->cmd_param_req_process_, host_source, ser_num, cmd, data, data_len);
            break;
        default:
            break;
    };
}

/**
 * 协议消息回调
 * @param receiver: 接口所有者
 * @param host_source: 主机数据源
 * @param ser_num: 流水号
 * @param cmd: 收到的命令
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return 无
 */
static void THmiTaskMsgNotify(void *receiver, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    THmiTask *self = (THmiTask *)receiver;
    THmiTaskMsgProcess(self, host_source, ser_num, cmd, data, data_len);
}
/**
 * 协议流发送
 */
static UInt32 THmiTaskStreamSend(void *receiver, UInt8 *data, UInt32 len)
{
    THmiTask *self = (THmiTask *)receiver;

    return KfcDevWrite(kSfDevNumLog, data, len);
}

/**
 * 消息发送
 */
static Bool THmiTaskMsgSend(void *receiver, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    THmiTask *self = (THmiTask *)receiver;
    return THmiProtocolSendMsg(&self->hmi_protocol_, host_source, ser_num, cmd, data, data_len);
}

/**
 * 创建对象
 * @param object: 对象实例
 * @return 
 */
static void THmiTaskOnTaskCreate(TOfObject *object)
{
    THmiTask *self = (THmiTask *)object;
    THmiProtocolCreate(&self->hmi_protocol_, &self->protocol_buffer_[0], sizeof(self->protocol_buffer_),
        &self->protocol_send_buffer_[0], sizeof(self->protocol_send_buffer_));
    self->hmi_protocol_.receiver_ = self;
    self->hmi_protocol_.method_notify_ = THmiTaskMsgNotify;
    self->hmi_protocol_.method_send_ = THmiTaskStreamSend;
    
    TCmdCtrlProcessCreate(&self->cmd_ctrl_process_);
    self->cmd_ctrl_process_.receiver_ = self;
    self->cmd_ctrl_process_.method_msg_send_ = THmiTaskMsgSend;
    
    TCmdParamSetProcessCreate(&self->cmd_param_set_process_);
    self->cmd_param_set_process_.receiver_ = self;
    self->cmd_param_set_process_.method_msg_send_ = THmiTaskMsgSend;
    
    TCmdParamReqProcessCreate(&self->cmd_param_req_process_);
    self->cmd_param_req_process_.receiver_ = self;
    self->cmd_param_req_process_.method_msg_send_ = THmiTaskMsgSend;
}

/**
 * 启动
 * @param object: 对象实例
 * @return 
 */
static void THmiTaskOnTaskStart(TOfObject *object)
{
    THmiTask *self = (THmiTask *)object;
    KfcDevOpen(kSfDevNumLog);
}

/**
 * 停止
 * @param object: 对象实例
 * @return 
 */
static void THmiTaskOnTaskStop(TOfObject *object)
{
//    THmiTask *self = (THmiTask *)object;
}

/**
 * 服务执行函数
 * @param object: 对象实例
 * @return 
 */
static void THmiTaskOnTaskExecute(TOfObject *object)
{
    THmiTask *self = (THmiTask *)object;

    while (!TKfcTaskTerminated((TKfcTask *)self))
    {
        UInt32 read_len;
        
        read_len = KfcDevRead(kSfDevNumLog, &self->com_read_buf_[0], sizeof(self->com_read_buf_));
        /// 循环解析
        if (read_len > 0)
        {
            THmiProtocolTransfer(&self->hmi_protocol_, &self->com_read_buf_[0], read_len);
        }
        
        KfcSleep(1);
    }
}

/**
 * 析构函数
 * @param object: 对象实例
 * @return 
 */
static void THmiTaskDestroy(TOfObject *object)
{
    THmiTask *self = (THmiTask *)object;
    
    OfDestroyObject((TOfObject *)(&self->cmd_ctrl_process_));
    OfDestroyObject((TOfObject *)(&self->cmd_param_set_process_));
    OfDestroyObject((TOfObject *)(&self->cmd_param_req_process_));
    
    self->Destroy_(object);
}

void THmiTaskCreate(THmiTask *self)
{
#if defined (__GNUC__)
    static char stack[768] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[768];
#endif
    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));
    OVERRIDE_METHOD(TOfObject, Destroy, THmiTaskDestroy);
    self->parent_.OnCreate = THmiTaskOnTaskCreate;
    self->parent_.OnStart = THmiTaskOnTaskStart;
    self->parent_.OnStop = THmiTaskOnTaskStop;
    self->parent_.OnExecute = THmiTaskOnTaskExecute;
    
    TKfcTaskResume(&self->parent_);
}
