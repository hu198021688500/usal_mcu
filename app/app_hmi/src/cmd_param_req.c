#include "cmd_param_req.h"
#include "kfc_memory.h"
#include "hmi_protocol.h"
#include "hmi_param_tab0.h"

enum
{
    kHmiProtocolParamReqRespMsgMaxLen = 250,      ///< 单包消息数据体最大长度
};

#pragma pack(1)
/// 主机参数查询命令消息头
typedef struct THmiProtocolHostParamReqMsgHeadTag THmiProtocolHostParamReqMsgHead;
struct THmiProtocolHostParamReqMsgHeadTag
{
    UInt8       param_tab_type;       ///< 参数表类型
    UInt8       req_cnt;              ///< 查询个数
};

/// 主机参数查询命令参数项消息体
typedef struct THmiProtocolHostParamReqMsgBodyTag THmiProtocolHostParamReqMsgBody;
struct THmiProtocolHostParamReqMsgBodyTag
{
    UInt8       param_id;              ///< 参数id
};

/// 从机参数查询应答消息头
typedef struct THmiProtocolSlaveParamReqRespMsgHeadTag THmiProtocolSlaveParamReqRespMsgHead;
struct THmiProtocolSlaveParamReqRespMsgHeadTag
{
    UInt8       param_tab_type;       ///< 参数表类型
    UInt8       req_cnt;              ///< 查询个数
    UInt8       resp_cnt;             ///< 应答个数
    UInt8       resp_len;             ///< 应答数据长度
};
/// 从机参数查询应答参数项消息体
typedef struct THmiProtocolSlaveParamReqRespMsgBodyTag THmiProtocolSlaveParamReqRespMsgBody;
struct THmiProtocolSlaveParamReqRespMsgBodyTag
{
    UInt8       param_id;              ///< 参数id
    UInt8       req_result;            ///< 查询结果 0:成功 1:不支持该参数 2:内容错误 其他:失败
    UInt8       reserved;              ///< 保留
    UInt8       param_len;             ///< 参数长度
    ///.........param_data;            ///< 参数值
};
#pragma pack()

/** 
 * 二分法查数据项id
 * @param id: 命令项
 * @return >= 0:成功 < 0:失败
 */
static Int32 TCmdParamReqProcessFindItem(TCmdParamReqProcess *self, TParamItem *tab, UInt8 item_cnt, UInt8 id)
{
    Int32 result = -1;
    Int32 cnt = item_cnt;
    Int32 m;
    Int32 b = 0;
    Int32 e;

    e = cnt - 1;

    while (b <= e)
    {
        m = ((b + e) >> 1);

        if (tab[m].param_id == id) 
        {
            result = m;
            break;
        }

        if (tab[m].param_id > id) 
        {
            e = m - 1;
        }
        else 
        {
            b = m + 1;
        }
    }

    return result;
}

static TParamItem* TCmdParamReqProcessGetTab(TCmdParamReqProcess *self, UInt8 tab_id, UInt8 *item_cnt)
{
    if (tab_id == 0)
    {
        *item_cnt = sizeof(g_param_tab0) / sizeof(g_param_tab0[0]);
        return &g_param_tab0[0];
    }
    else
        return NULL;
}

/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TCmdParamReqProcessDestroy(TOfObject *object)
{
    TCmdParamReqProcess *self = (TCmdParamReqProcess *)object;
    self->Destroy_(object);
}

void TCmdParamReqProcessCreate(TCmdParamReqProcess *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TCmdParamReqProcessDestroy);
    
    self->method_msg_send_ = NULL;
    self->receiver_ = NULL;
}


Bool TCmdParamReqProcessNotifyMsg(TCmdParamReqProcess *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    Bool result = False;
    if (data != NULL && data_len >= sizeof(THmiProtocolHostParamReqMsgHead))
    {
        THmiProtocolHostParamReqMsgHead *req_head = (THmiProtocolHostParamReqMsgHead *)data;
        ///< 查询所有
        if (req_head->req_cnt == 0)
        {
            UInt32 i = 0;
            UInt32 now_len = 0;
            UInt32 body_len = 0;
            UInt32 body_cnt = 0;
            Bool is_resp = False;
            /// 先分配单条消息最大250字节
            UInt32 resp_len = sizeof(THmiProtocolSlaveParamReqRespMsgHead) + kHmiProtocolParamReqRespMsgMaxLen;
            THmiProtocolSlaveParamReqRespMsgHead *resp_head = (THmiProtocolSlaveParamReqRespMsgHead *)KfcMalloc(resp_len);
            if (resp_head != NULL)
            {
                THmiProtocolSlaveParamReqRespMsgBody *resp_body = (THmiProtocolSlaveParamReqRespMsgBody *)(resp_head + 1);
                
                resp_head->param_tab_type = req_head->param_tab_type;
                resp_head->req_cnt = req_head->req_cnt;
                
                UInt8 tab_item_cnt = 0;
                TParamItem *param_tab = TCmdParamReqProcessGetTab(self, req_head->param_tab_type, &tab_item_cnt);
                if (param_tab != NULL)
                {
                    while(i < tab_item_cnt)
                    {
                        if (now_len + sizeof(THmiProtocolSlaveParamReqRespMsgBody) + param_tab[i].param_len < kHmiProtocolParamReqRespMsgMaxLen)
                        {
                            resp_body->param_id = param_tab[i].param_id;
                            if (param_tab[i].call_back(False, (UInt8 *)(resp_body + 1), param_tab[i].param_len) == kHmiParamCmdResultOk)
                            {
                                resp_body->param_len = param_tab[i].param_len;
                                resp_body->req_result = kHmiParamCmdResultOk;
                            }
                            else
                            {
                                resp_body->param_len = 0;
                                resp_body->req_result = kHmiParamCmdResultParamErr;
                            }
                            
                            body_len = sizeof(THmiProtocolSlaveParamReqRespMsgBody) + resp_body->param_len;
                            
                            resp_body = (THmiProtocolSlaveParamReqRespMsgBody *)((UInt8 *)resp_body + body_len);
                            now_len += body_len;  
                            body_cnt++;
                        }
                        /// 会溢出,发送当前帧
                        else
                        {
                            resp_head->resp_cnt = body_cnt;
                            resp_head->resp_len = now_len;
                            if (self->method_msg_send_ != NULL && body_cnt > 0)
                            {
                                self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveParamReqResp, (UInt8 *)resp_head, sizeof(THmiProtocolSlaveParamReqRespMsgHead) + now_len);
                            }
                            body_cnt = 0;
                            now_len = 0;
                            is_resp = True;
                        }
                        i++;
                    }
                }
                
                if (body_cnt > 0 || !is_resp)
                {
                    resp_head->resp_cnt = body_cnt;
                    resp_head->resp_len = now_len;
                    if (self->method_msg_send_ != NULL && body_cnt > 0)
                    {
                        self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveParamReqResp, (UInt8 *)resp_head, sizeof(THmiProtocolSlaveParamReqRespMsgHead) + now_len);
                    }
                    body_cnt = 0;
                    now_len = 0;
                    is_resp = True;
                }
                KfcFree((UInt8 *)resp_head);
                result = True;
            }
        }
        else
        {
            /// 先分配单条消息最大250字节
            UInt32 resp_len = sizeof(THmiProtocolSlaveParamReqRespMsgHead) + kHmiProtocolParamReqRespMsgMaxLen;
            THmiProtocolSlaveParamReqRespMsgHead *resp_head = (THmiProtocolSlaveParamReqRespMsgHead *)KfcMalloc(resp_len);
            
            if (resp_head != NULL)
            {
                UInt32 i = 0;
                Int32 idx;
                UInt32 now_len = 0;
                UInt32 body_len = 0;
                UInt32 body_cnt = 0;
                Bool is_resp = False;
                THmiProtocolHostParamReqMsgBody *req_body = (THmiProtocolHostParamReqMsgBody *)(req_head + 1);
                THmiProtocolSlaveParamReqRespMsgBody *resp_body = (THmiProtocolSlaveParamReqRespMsgBody *)(resp_head + 1);
                
                resp_head->param_tab_type = req_head->param_tab_type;
                resp_head->req_cnt = req_head->req_cnt;
                
                UInt8 tab_item_cnt = 0;
                TParamItem *param_tab = TCmdParamReqProcessGetTab(self, req_head->param_tab_type, &tab_item_cnt);
                
                while(i < req_head->req_cnt)
                {
                    idx = TCmdParamReqProcessFindItem(self, param_tab, tab_item_cnt, req_body->param_id);
                    if (idx >= 0)
                    {
                        if (now_len + sizeof(THmiProtocolSlaveParamReqRespMsgBody) + param_tab[idx].param_len < kHmiProtocolParamReqRespMsgMaxLen)
                        {
                            resp_body->param_id = param_tab[idx].param_id;
                            if (param_tab[idx].call_back(False, (UInt8 *)(resp_body + 1), param_tab[idx].param_len) == kHmiParamCmdResultOk)
                            {
                                resp_body->param_len = param_tab[idx].param_len;
                                resp_body->req_result = kHmiParamCmdResultOk;
                            }
                            else
                            {
                                resp_body->param_len = 0;
                                resp_body->req_result = kHmiParamCmdResultParamErr;
                            }
                            
                            body_len = sizeof(THmiProtocolSlaveParamReqRespMsgBody) + resp_body->param_len;
                            
                            resp_body = (THmiProtocolSlaveParamReqRespMsgBody *)((UInt8 *)resp_body + body_len);
                            now_len += body_len;  
                            body_cnt++;
                        }
                        /// 会溢出,发送当前帧
                        else
                        {
                            resp_head->resp_cnt = body_cnt;
                            resp_head->resp_len = now_len;
                            if (self->method_msg_send_ != NULL && body_cnt > 0)
                            {
                                self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveParamReqResp, (UInt8 *)resp_head, sizeof(THmiProtocolSlaveParamReqRespMsgHead) + now_len);
                            }
                            body_cnt = 0;
                            now_len = 0;
                            is_resp = True;
                        }
                    }
                    /// 不支持id
                    else
                    {
                        resp_body->param_id = req_body->param_id;
                        resp_body->param_len = 0;
                        resp_body->req_result = kHmiParamCmdResultIdErr;
                        now_len += sizeof(*resp_body);
                        resp_body++;
                        body_cnt++;
                    }
                    req_body++;
                    i++;
                }
                /// 如果有数据或者一直无应答过
                if (body_cnt > 0 || !is_resp)
                {
                    resp_head->resp_cnt = body_cnt;
                    resp_head->resp_len = now_len;
                    if (self->method_msg_send_ != NULL && body_cnt > 0)
                    {
                        self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveParamReqResp, (UInt8 *)resp_head, sizeof(THmiProtocolSlaveParamReqRespMsgHead) + now_len);
                    }
                    body_cnt = 0;
                    now_len = 0;
                    is_resp = True;
                }
                KfcFree((UInt8 *)resp_head);
                result = True;
            }            
        }
    }
    return result;
}
