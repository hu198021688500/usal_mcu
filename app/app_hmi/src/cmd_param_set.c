#include "cmd_param_set.h"
#include "hmi_protocol.h"
#include "kfc_memory.h"
#include "hmi_param_tab0.h"

#pragma pack(1)
/// 主机参数设置命令消息头
typedef struct THmiProtocolHostParamSetMsgHeadTag THmiProtocolHostParamSetMsgHead;
struct THmiProtocolHostParamSetMsgHeadTag
{
    UInt8       param_tab_type;         ///< 参数表类型
    UInt8       reserved;               ///< 保留
    UInt8       param_cnt;              ///< 参数个数
    UInt8       data_len;               ///< 数据长度
};

/// 主机参数设置命令参数项消息体
typedef struct THmiProtocolHostParamSetMsgBodyTag THmiProtocolHostParamSetMsgBody;
struct THmiProtocolHostParamSetMsgBodyTag
{
    UInt8       param_id;              ///< 参数id
    UInt8       param_len;             ///< 参数长度
    ///<        param_data             ///< 跟具体参数值
};

/// 从机参数设置应答消息头
typedef struct THmiProtocolSlaveParamSetRespMsgHeadTag THmiProtocolSlaveParamSetRespMsgHead;
struct THmiProtocolSlaveParamSetRespMsgHeadTag
{
    UInt8       param_tab_type;         ///< 参数表类型
    UInt8       reserved;               ///< 保留
    UInt8       param_cnt;              ///< 参数个数
    UInt8       data_len;               ///< 数据长度
};
/// 从机参数设置应答参数项消息体
typedef struct THmiProtocolSlaveParamSetRespMsgBodyTag THmiProtocolSlaveParamSetRespMsgBody;
struct THmiProtocolSlaveParamSetRespMsgBodyTag
{
    UInt8       param_id;              ///< 参数id
    UInt8       set_result;          ///< 设置结果 0:成功 1:不支持该参数 2:内容错误 其他:失败
};
#pragma pack()

/** 
 * 二分法查数据项id
 * @param id: 命令项
 * @return >= 0:成功 < 0:失败
 */
static Int32 TCmdParamSetProcessFindItem(TCmdParamSetProcess *self, TParamItem *tab, UInt8 item_cnt, UInt8 id)
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

static TParamItem* TCmdParamSetProcessGetTab(TCmdParamSetProcess *self, UInt8 tab_id, UInt8 *item_cnt)
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
static void TCmdParamSetProcessDestroy(TOfObject *object)
{
    TCmdParamSetProcess *self = (TCmdParamSetProcess *)object;
    self->Destroy_(object);
}

void TCmdParamSetProcessCreate(TCmdParamSetProcess *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TCmdParamSetProcessDestroy);
    
    self->method_msg_send_ = NULL;
    self->receiver_ = NULL;
}

Bool TCmdParamSetProcessNotifyMsg(TCmdParamSetProcess *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    Bool result = False;
    if (data != NULL && data_len > sizeof(THmiProtocolHostParamSetMsgHead))
    {
        THmiProtocolHostParamSetMsgHead *set_head = (THmiProtocolHostParamSetMsgHead *)data;
        /// 再次匹配消息长度是否符合
        if (set_head->data_len + sizeof(THmiProtocolHostParamSetMsgHead) == data_len)
        {
            UInt32 i = 0;
            THmiParamCmdResult item_result;
            Int32 idx;
            
            UInt32 resp_len = sizeof(THmiProtocolSlaveParamSetRespMsgHead) + set_head->param_cnt * sizeof(THmiProtocolSlaveParamSetRespMsgBody);
            THmiProtocolSlaveParamSetRespMsgHead *resp_head = (THmiProtocolSlaveParamSetRespMsgHead *)KfcMalloc(resp_len);
            /// 如果应答消息分配成功
            if (resp_head != NULL)
            {
                THmiProtocolHostParamSetMsgBody *set_body = (THmiProtocolHostParamSetMsgBody *)(set_head + 1);
                THmiProtocolSlaveParamSetRespMsgBody *resp_body = (THmiProtocolSlaveParamSetRespMsgBody *)(resp_head + 1);
                
                resp_head->param_tab_type = set_head->param_tab_type;
                resp_head->reserved = 0;
                resp_head->param_cnt = set_head->param_cnt;
                resp_head->data_len = resp_len - sizeof(THmiProtocolSlaveParamSetRespMsgHead);
                
                UInt8 tab_item_cnt = 0;
                TParamItem *param_tab = TCmdParamSetProcessGetTab(self, set_head->param_tab_type, &tab_item_cnt);
                if (param_tab != NULL)
                {
                    while(i < set_head->param_cnt)
                    {
                        /// 根据id查找具体项
                        idx = TCmdParamSetProcessFindItem(self, param_tab, tab_item_cnt, set_body->param_id);
                        /// 能查到具体参数项
                        if (idx >= 0)
                        {
                            TParamItem *item = &param_tab[idx];
                            if (item->param_len == set_body->param_len)
                            {
                                item_result = item->call_back(True, (UInt8 *)(set_body + 1), item->param_len);
                            }
                            else
                                item_result = kHmiParamCmdResultParamErr;
                        }
                        else
                            item_result = kHmiParamCmdResultIdErr;
                        /// 设置应答结果
                        resp_body->param_id = set_body->param_id;
                        resp_body->set_result = item_result;
                        /// 参数和应答都后移
                        set_body = (THmiProtocolHostParamSetMsgBody *)((UInt8 *)set_body
                                    + sizeof(THmiProtocolHostParamSetMsgBody) + set_body->param_len);
                        resp_body++;
                        i++;
                    }
                }
                else
                {
                    resp_head->param_cnt = 0;
                    resp_head->data_len = 0;
                    resp_len = sizeof(THmiProtocolSlaveParamSetRespMsgHead);
                }
                
                
                /// 发送应答
                if (self->method_msg_send_ != NULL)
                    self->method_msg_send_(self->receiver_, host_source, ser_num, kHmiProtocolCmdSlaveParamSetResp, (UInt8 *)resp_head, resp_len);
                
                KfcFree((UInt8 *)resp_head);
                
                result = True;
            }
        }
    }
    return result;
}
