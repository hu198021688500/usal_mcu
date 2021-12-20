#include "test_protocol.h"
#include "string.h"
#include "ofc_crc16.h"
#include "stdio.h"

/// 数据传输命令
enum
{
    kProtocolStartFlag       = 0x5A,     ///< 起始符
};

#pragma pack(1)
/// 整体消息包头
typedef struct TProtocolPackHeadTag TProtocolPackHead;
struct TProtocolPackHeadTag
{
    uint8_t       start_tag;             ///< 包头标志0x5A
    uint8_t       serial_num;            ///< 流水号
    uint8_t       data_len;              ///< 数据长度
    uint8_t       data_len_neg;          ///< 数据长度取反
    //uint8_t       data...              ///< 数据体,命令不同数据不同
    //uint16_t      crc;                 ///< crc校验码(start_tag开始-数据结束)
};
#pragma pack()

/**
 * 传输消息结构体
 */
typedef struct TTransferMsgTag TTransferMsg;
struct TTransferMsgTag
{
    uint8_t    *msg_;           ///< 消息指针
    uint32_t    msg_len;        ///< 消息长度
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
static bool TTestProtocolParse(uint8_t *buffer, uint32_t size, uint32_t* pop_size, uint32_t *min_size, TTransferMsg *msg)
{
    bool result = false;
    *min_size = sizeof(TProtocolPackHead) + 2;      ///< 消息头+2字节crc
    if (buffer != NULL && size >= *min_size)
    {
        /// 查找start消息头
        uint8_t *m_head = memchr(buffer, kProtocolStartFlag, size);
        if (m_head)
        {
            *pop_size = m_head - buffer;

            TProtocolPackHead *pack_head = (TProtocolPackHead *)m_head;
            uint8_t data_neg = ~pack_head->data_len;
            if (data_neg == pack_head->data_len_neg)
            {
                uint32_t crc_len = pack_head->data_len + sizeof(TProtocolPackHead);
                /// 当消息长度足够
                if (crc_len + 2 <= size - *pop_size)
                {
                    uint16_t msg_crc = *(uint16_t *)(m_head + crc_len);

                    uint16_t now_crc = OfCalcCrc16((uint8_t *)&pack_head->start_tag, crc_len);  ///< CRC校验
                    /// 累加和校验成功
                    if (msg_crc == now_crc)
                    {
                        msg->msg_ = m_head;
                        msg->msg_len = pack_head->data_len + sizeof(TProtocolPackHead) + 2;
                        (*pop_size) = pack_head->data_len + sizeof(TProtocolPackHead) + 2;
                        result = true;
                    }
                    else
                    {
                        (*pop_size)++;
                    }
                }
                else
                {
                    *min_size = crc_len + 2;
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
static void TTestProtocolProcess(TTestProtocol *self, TTransferMsg *msg)
{
    if (msg != NULL)
    {
        TProtocolPackHead *head= (TProtocolPackHead *)msg->msg_;
        /// 比较设备号，匹配才通知
        if (self->receiver_ != NULL && self->method_notify_ != NULL)
        {
            self->method_notify_(self->receiver_, head->serial_num, (uint8_t *)(head + 1), head->data_len);
        }
    }
}

/**
 * 对象创建
 * @param self: 对象实例指针
 * @param buffer: 数据缓存区
 * @param buffer_size: 数据缓存区大小
 * @return 如果创建成功返回True,否则返回False
 */
bool TTestProtocolCreate(TTestProtocol *self, uint8_t* buffer, uint32_t buffer_size, uint8_t* send_buffer, uint32_t send_buffer_size)
{
    bool result = false;

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
        result = true;
    }
    return result;
}

void TTestProtocolDestroy(TTestProtocol *self)
{
    self->now_size_ = 0;
}

void TTestProtocolTransfer(TTestProtocol *self, uint8_t *data, uint32_t data_len)
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

        uint32_t pop_size = 0;
        uint32_t min_size = sizeof(TProtocolPackHead) + 1;        ///< 消息头+1字节结束符
        TTransferMsg msg;
        while (self->now_size_ - pop_size >= min_size)
        {
            uint32_t now_pop = 0;

            if (TTestProtocolParse(self->buffer_ + pop_size, self->now_size_ - pop_size, &now_pop, & min_size, &msg))
            {
                TTestProtocolProcess(self, &msg);
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

bool TTestProtocolSendData(TTestProtocol *self, uint8_t serial_num, uint8_t *data, uint8_t len)
{
    bool result = false;

    uint32_t total_len = sizeof(TProtocolPackHead) + len + 2;
    if (self->send_buffer_size_ > total_len)
    {
        TProtocolPackHead *head = (TProtocolPackHead *)&self->send_buffer_[0];
        
        UInt8 *body = (UInt8 *)(head + 1);
        uint16_t crc;

        head->start_tag = kProtocolStartFlag;
        head->serial_num = serial_num;
        head->data_len = len;
        head->data_len_neg = ~len;

        memcpy(body, data, len);
        crc = OfCalcCrc16(&head->start_tag, total_len - 2);

        memcpy((uint8_t *)(body + len), (uint8_t *)&crc, 2);

        if (self->method_send_(self->receiver_, &self->send_buffer_[0], total_len) > 0)
            result = true;
    }
    return result;
}

