#include "hmi_protocol.h"
#include "string.h"
#include "ofc_crc16.h"

/// 数据传输命令
enum
{
    kProtocolStartFlag       = 0x5A,     ///< 起始符
    kProtocolSdevCcu         = 0x01,     ///< ccu设备码
};

#pragma pack(1)
/// 整体消息包头
typedef struct TProtocolPackHeadTag TProtocolPackHead;
struct TProtocolPackHeadTag
{
    UInt8       start_tag;             ///< 包头标志0x5A
    UInt8       s_dev;                 ///< 特殊设备 0x01:ccu
    UInt16      crc16;                 ///< Crc16值(host_source开始到消息结束)
    UInt8       host_source;           ///< 主机数据源 0x00:电脑串口
    UInt8       ser_num;               ///< 流水号
    UInt8       reserve;               ///< 保留
    UInt8       cmd;                   ///< 命令码
    UInt16      data_len;              ///< 数据长度
    UInt16      data_len_n;            ///< 数据长度反码
    //UInt8       data...              ///< 数据体,命令不同数据不同
};
#pragma pack()

/**
 * 传输消息结构体
 */
typedef struct TTransferMsgTag TTransferMsg;
struct TTransferMsgTag
{
    UInt8      *msg_;           ///< 消息指针
    UInt32    msg_len;        ///< 消息长度
};
/**
 * 协议解析
 * @param buffer: 数据地址
 * @param size: 数据长度
 * @param pop_size: 要丢弃的长度
 * @param min_size: 需要的最小长度
 * @param msg: 解析出的消息体
 * @return True: msg有效 False:msg无效
 */
static Bool THmiProtocolParse(UInt8 *buffer, UInt32 size, UInt32* pop_size, UInt32 *min_size, TTransferMsg *msg)
{
    Bool result = False;
    *min_size = sizeof(TProtocolPackHead);
    if (buffer != NULL && size >= *min_size)
    {
        /// 查找start消息头
        UInt8 *m_head = memchr(buffer, kProtocolStartFlag, size);
        if (m_head)
        {
            *pop_size = m_head - buffer;

            TProtocolPackHead *pack_head = (TProtocolPackHead *)m_head;

            if (pack_head->s_dev == kProtocolSdevCcu)
            {
                UInt32 crc_len = pack_head->data_len + sizeof(TProtocolPackHead) - 4;
                /// 当消息长度足够
                if (crc_len + 4 <= size - *pop_size)
                {
                    UInt16 now_crc = OfCalcCrc16((UInt8 *)&pack_head->host_source, crc_len);  ///< CRC校验
                    /// 累加和校验成功
                    if (now_crc == pack_head->crc16)
                    {
                        msg->msg_ = m_head;
                        msg->msg_len = pack_head->data_len + sizeof(TProtocolPackHead);
                        (*pop_size) = pack_head->data_len + sizeof(TProtocolPackHead);
                        result = True;
                    }
                    else
                    {
                        (*pop_size)++;
                    }
                }
                else
                {
                    *min_size = crc_len + 4;
                }
            }
            /// 命令字无效丢弃当前消息头
            else
            {
                (*pop_size)++;
            }

        }
        else
        {
            *pop_size = size;
        }
    }
    return result;
}
/**
 * 消息处理
 * @param self: 对象指针
 * @param msg: 解析出的消息体
 * @return True: msg有效 False:msg无效
 */
static void THmiProtocolProcess(THmiProtocol *self, TTransferMsg *msg)
{
    if (msg != NULL)
    {
        TProtocolPackHead *head= (TProtocolPackHead *)msg->msg_;
        /// 比较设备号，匹配才通知
        if (self->receiver_ != NULL && self->method_notify_ != NULL)
        {
            self->method_notify_(self->receiver_, head->host_source, head->ser_num, head->cmd, (UInt8 *)(head + 1), head->data_len);
        }
    }
}
/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void THmiProtocolDestroy(TOfObject *object)
{
    THmiProtocol *self = (THmiProtocol *)object;

    self->now_size_ = 0;
    self->Destroy_(object);
}
/**
 * 对象创建
 * @param self: 对象实例指针
 * @param buffer: 数据缓存区
 * @param buffer_size: 数据缓存区大小
 * @return 如果创建成功返回True,否则返回False
 */
Bool THmiProtocolCreate(THmiProtocol *self, UInt8* buffer, UInt32 buffer_size, UInt8* send_buffer, UInt32 send_buffer_size)
{
    Bool result = False;
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, THmiProtocolDestroy);
    
    self->method_notify_ = NULL;
    self->method_send_ = NULL;
    self->receiver_ = NULL;
    self->now_size_ = 0;

    if (buffer != NULL && buffer_size > 0
            && send_buffer != NULL && send_buffer_size > 0)
    {
        self->buffer_ = buffer;
        self->buffer_size_ = buffer_size;

        self->send_buffer_ = send_buffer;
        self->send_buffer_size_ = send_buffer_size;
        result = True;
    }
    return result;
}

void THmiProtocolTransfer(THmiProtocol *self, UInt8 *data, UInt32 data_len)
{
    if (self->buffer_ != NULL && data != NULL && data_len > 0 && data_len < self->buffer_size_)
    {
        if (self->now_size_ + data_len < self->buffer_size_)
        {
            memcpy(&self->buffer_[self->now_size_], data, data_len);
            self->now_size_ += data_len;
        }
        /// 如果存放不行，清空之前的，重置
        else
        {
            memcpy(&self->buffer_[0], data, data_len);
            self->now_size_ = data_len;
        }

        UInt32 pop_size = 0;
        UInt32 min_size = sizeof(TProtocolPackHead) + 1;        ///< 消息头+1字节结束符
        TTransferMsg msg;
        while (self->now_size_ - pop_size >= min_size)
        {
            UInt32 now_pop = 0;

            if (THmiProtocolParse(self->buffer_ + pop_size, self->now_size_ - pop_size, &now_pop, & min_size, &msg))
            {
                THmiProtocolProcess(self, &msg);
            }
            if (now_pop > 0)
            {
                pop_size += now_pop;
                if (pop_size > self->now_size_)
                    pop_size = self->now_size_;
            }
        }

        if (pop_size >= self->now_size_)
            self->now_size_ = 0;
        else if (pop_size > 0)
        {
            memmove(self->buffer_, self->buffer_ + pop_size, self->now_size_ - pop_size);
            self->now_size_ -= pop_size;
        }
    }
}

Bool THmiProtocolSendMsg(THmiProtocol *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    Bool result = False;

    UInt32 total_len = sizeof(TProtocolPackHead) + data_len;
    if (self->send_buffer_size_ >= total_len)
    {
        TProtocolPackHead *head = (TProtocolPackHead *)&self->send_buffer_[0];
        UInt8 *body = (UInt8 *)(head + 1);
        head->start_tag = kProtocolStartFlag;
        head->s_dev = kProtocolSdevCcu;
        head->host_source = host_source;
        head->ser_num = ser_num;
        head->reserve = 0;
        head->cmd = cmd;
        head->data_len = data_len;
        head->data_len_n = ~data_len;
        memcpy(body, data, data_len);

        head->crc16 = OfCalcCrc16(&head->host_source, total_len - 4);
        
        if (self->method_send_(self->receiver_, &self->send_buffer_[0], total_len) > 0)
            result = True;
    }
    return result;
}
