/**
 * @file test_procol.h
 * @brief RS485测试协议
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
#ifndef _TEST_PROTOCOL_H_
#define _TEST_PROTOCOL_H_

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TTestProtocolTag TTestProtocol;
/**
 * 命令通知函数
 * @param receiver: 接口所有者
 * @param serial: 流水号
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return 
 */
typedef void (*TTestProtocolMsgNotify)(void *receiver, uint8_t serial, uint8_t* data, uint32_t data_len);

/**
 * 消息实际发送函数
 * @param receiver: 接口所有者
 * @param data: 发送数据地址
 * @param len: 发送数据长度
 * @return 0:失败 >0:发送成功个数
 */
typedef uint32_t (*TTestProtocolDataSend)(void *receiver, uint8_t *data, uint32_t len);

struct TTestProtocolTag
{
///PUBLIC
    TTestProtocolMsgNotify     method_notify_;     ///< 状态通知方法(调用者实现)
    TTestProtocolDataSend      method_send_;       ///< 发送消息方法(调用者实现)
    void                       *receiver_;          ///< 接收者
///PRIVATE
    uint8_t           *buffer_;                         ///< buffer缓冲区指针
    uint8_t           *send_buffer_;                    ///< 发送缓冲区指针
    uint32_t           buffer_size_;                     ///< buffer缓冲区大小
    uint32_t           send_buffer_size_;                ///< 发送缓冲区大小
    uint32_t           now_size_;                        ///< 当前数据长度
};

/**
 * 对象创建
 * @param self: 对象实例指针
 * @param buffer: 数据缓存区
 * @param buffer_size: 数据缓存区大小
 * @param send_buffer: 发送数据缓存区
 * @param send_buffer_size: 发送数据缓存区大小
 * @return 如果创建成功返回True,否则返回False
 */
bool TTestProtocolCreate(TTestProtocol *self, uint8_t* buffer, uint32_t buffer_size, uint8_t* send_buffer, uint32_t send_buffer_size);
/**
 * 析构函数
 * @param self: 对象实例指针
 * @return
 */
void TTestProtocolDestroy(TTestProtocol *self);

/**
 * 传入数据(解析后会调用通知函数)
 * @param self: 对象实例指针
 * @param data: 获取到的数据
 * @param data_len: 获取到的数据长度
 * @return 无
 */
void TTestProtocolTransfer(TTestProtocol *self, uint8_t *data, uint32_t data_len);

/**
 * 发送数据
 * @param self: 对象实例指针
 * @param serial_num: 流水号
 * @param data: 数据
 * @param len: 数据长度
 * @return True:成功 False:失败
 */
bool TTestProtocolSendData(TTestProtocol *self, uint8_t serial_num, uint8_t *data, uint8_t len);

#ifdef __cplusplus
}
#endif
#endif
