/**
 * @file com_procol.h
 * @brief 串口协议解析
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
#ifndef _COM_PROTOCOL_H_
#define _COM_PROTOCOL_H_

#include <ofc_object.h>
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif


#pragma pack(1)

/// 控制接口控制体(下行)
typedef struct TProtocolPackBodyCtlTag TProtocolPackBodyCtl;
struct TProtocolPackBodyCtlTag
{
    uint8_t value;      ///< 0:关闭 1:打开
    uint8_t res;        ///< 保留
};
/// 输出消息包
typedef struct TProtocolPackBodyPowerTag TProtocolPackBodyPower;
struct TProtocolPackBodyPowerTag
{
    uint8_t idx;        ///< 哪个io(0起始)
    uint8_t value;      ///< 0或者1
};

/// 输入接口控制体(上行,1s间隔)
typedef struct TProtocolPackBodySwitchTag TProtocolPackBodySwitch;
struct TProtocolPackBodySwitchTag
{
    uint32_t value;         ///< 数据值(0~31掩码)
    uint32_t flag;          ///< 数据是否有效(0~31掩码)
};

/// AD值接口控制体(上行,1s间隔)
typedef struct TProtocolPackBodyAdcHeadTag TProtocolPackBodyAdcHead;
struct TProtocolPackBodyAdcHeadTag
{
    uint32_t cnt;         ///< AD个数
    //uint32_t value...;  ///< 如下为多个4字节AD值
};

/// can接口控制体消息头(下行)
typedef struct TProtocolPackBodyCanHeadTag TProtocolPackBodyCanHead;
struct TProtocolPackBodyCanHeadTag
{
    uint8_t idx;        ///< 第几路CAN
    uint8_t cmd;        ///< 0:发送 1:发送结果 2:接收 3:接收结果
};

/// can接口发送命令控制体(下行)
typedef struct TProtocolPackBodyCanSendTag TProtocolPackBodyCanSend;
struct TProtocolPackBodyCanSendTag
{
    TProtocolPackBodyCanHead can_head;  ///< can消息头
    uint8_t  send_cmd;                  ///< 0:停止 1:发送
    uint8_t  res;                       ///< 保留
    uint32_t send_id;                   ///< 发送can_id
    uint32_t send_inv;                  ///< 发送can间隔
    uint32_t send_cnt;                  ///< 发送个数
};

/// can接口发送通知结果控制体(上行)
typedef struct TProtocolPackBodyCanSendResultTag TProtocolPackBodyCanSendResult;
struct TProtocolPackBodyCanSendResultTag
{
    TProtocolPackBodyCanHead can_head;  ///< can消息头
    uint32_t send_cnt;                  ///< 发送成功个数
    uint32_t send_fail_cnt;             ///< 发送失败个数
};

/// can接口接收命令控制体(下行)
typedef struct TProtocolPackBodyCanRecTag TProtocolPackBodyCanRec;
struct TProtocolPackBodyCanRecTag
{
    TProtocolPackBodyCanHead can_head;  ///< can消息头
    uint8_t  rec_cmd;                   ///< 0:停止 1:接收
    uint8_t  res;                       ///< 保留
    uint32_t rec_id;                    ///< 发送can_id
};

/// can接口接收通知结果控制体(上行)
typedef struct TProtocolPackBodyCanRecResultTag TProtocolPackBodyCanRecResult;
struct TProtocolPackBodyCanRecResultTag
{
    TProtocolPackBodyCanHead can_head;  ///< can消息头
    uint32_t rec_cnt;                   ///< 接收成功个数
};

/// can接口设置波特率控制体(下行)
typedef struct TProtocolPackBodyCanSetBaudTag TProtocolPackBodyCanSetBaud;
struct TProtocolPackBodyCanSetBaudTag
{
    TProtocolPackBodyCanHead can_head;  ///< can消息头
    uint32_t baud;                      ///< 波特率值
    uint32_t res;                       ///< 保留
};

/// 串口接口控制体消息头(下行)
typedef struct TProtocolPackBodyUartHeadTag TProtocolPackBodyUartHead;
struct TProtocolPackBodyUartHeadTag
{
    uint8_t idx;        ///< 第几路串口
    uint8_t cmd;        ///< 0:发送 1:发送结果 2:接收 3:接收结果
};

/// 串口接口发送命令控制体(下行)
typedef struct TProtocolPackBodyUartSendTag TProtocolPackBodyUartSend;
struct TProtocolPackBodyUartSendTag
{
    TProtocolPackBodyUartHead uart_head;  ///< 串口消息头
    uint8_t  send_cmd;                  ///< 0:停止 1:发送
    uint8_t  res;                       ///< 保留
    uint32_t send_size;                 ///< 单包大小
    uint32_t send_inv;                  ///< 发送can间隔
    uint32_t send_cnt;                  ///< 发送个数
};

/// 串口接口发送通知结果控制体(上行)
typedef struct TProtocolPackBodyUartSendResultTag TProtocolPackBodyUartSendResult;
struct TProtocolPackBodyUartSendResultTag
{
    TProtocolPackBodyUartHead uart_head;  ///< 串口消息头
    uint32_t send_cnt;                  ///< 发送成功个数
    uint32_t send_fail_cnt;             ///< 发送失败个数
};

/// 串口接口接收命令控制体(下行)
typedef struct TProtocolPackBodyUartRecTag TProtocolPackBodyUartRec;
struct TProtocolPackBodyUartRecTag
{
    TProtocolPackBodyUartHead uart_head;  ///< 串口消息头
    uint8_t  rec_cmd;                  ///< 0:停止 1:接收
    uint8_t  res;                      ///< 保留
};

/// 串口接口接收通知结果控制体(上行)
typedef struct TProtocolPackBodyUartRecResultTag TProtocolPackBodyUartRecResult;
struct TProtocolPackBodyUartRecResultTag
{
    TProtocolPackBodyUartHead uart_head;  ///< 串口消息头
    uint32_t rec_cnt;                     ///< 接收成功个数
    uint32_t rec_fail_cnt;                ///< 接收失败个数
};

/// 串口接口设置波特率控制体(下行)
typedef struct TProtocolPackBodyUartSetBaudTag TProtocolPackBodyUartSetBaud;
struct TProtocolPackBodyUartSetBaudTag
{
    TProtocolPackBodyUartHead uart_head;  ///< 串口消息头
    uint32_t baud;                        ///< 波特率值
    uint32_t res;                         ///< 保留
};
#pragma pack()

enum TComProtocolDirTag
{
    kComProtocolDirMaster        = 0x00,     ///< 控制端发送
    kComProtocolDirSlave,                    ///< 被控端发送
};


/// 数据传输命令
enum TComProtocolCmdTag
{
    kComProtocolCmdCtl        = 0x00,     ///< 控制命令
    kComProtocolCmdPower      = 0x01,     ///< 输出设备
    kComProtocolCmdSwitch     = 0x02,     ///< 输入设备
    kComProtocolCmdAdc        = 0x03,     ///< adc
    kComProtocolCmdCan        = 0x04,     ///< can
    kComProtocolCmdUart       = 0x05,     ///< uart

};
typedef enum TComProtocolCmdTag TComProtocolCmd;

/// CAN传输命令
enum TComProtocolCmdCanCmdTag
{
    kComProtocolCmdCanCmdSend       = 0,        ///< can发送命令(下行)
    kComProtocolCmdCanCmdSendResult,            ///< can发送结果(上行)
    kComProtocolCmdCanCmdRec,                   ///< can接收(下行)
    kComProtocolCmdCanCmdRecResult,             ///< can接收结果(上行)
    kComProtocolCmdCanCmdSetBaud,               ///< can设置波特率(下行)
};
typedef enum TComProtocolCmdCanCmdTag TComProtocolCmdCanCmd;


/// 串口数据传输命令
enum TComProtocolCmdUartCmdTag
{
    kComProtocolCmdUartCmdSend       = 0,        ///< 串口发送命令(下行)
    kComProtocolCmdUartCmdSendResult,            ///< 串口发送结果(上行)
    kComProtocolCmdUartCmdRec,                   ///< 串口接收(下行)
    kComProtocolCmdUartCmdRecResult,             ///< 串口接收结果(上行)
    kComProtocolCmdUartCmdSetBaud,               ///< 串口设置波特率(下行)
};
typedef enum TComProtocolCmdCanCmdTag TComProtocolCmdCanCmd;


typedef struct TComProtocolTag TComProtocol;
/**
 * 命令通知函数
 * @param receiver: 接口所有者
 * @param cmd: 收到的命令
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return 
 */
typedef void (*TComProtocolMsgNotify)(void *receiver, uint8_t cmd, uint8_t* data, uint32_t data_len);

/**
 * 消息实际发送函数
 * @param receiver: 接口所有者
 * @param data: 发送数据地址
 * @param len: 发送数据长度
 * @return 0:失败 >0:发送成功个数
 */
typedef uint32_t (*TComProtocolDataSend)(void *receiver, uint8_t *data, uint32_t len);

struct TComProtocolTag
{
///PUBLIC
    TComProtocolMsgNotify     method_notify_;     ///< 状态通知方法(调用者实现)
    TComProtocolDataSend      method_send_;       ///< 发送消息方法(调用者实现)
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
bool TComProtocolCreate(TComProtocol *self, uint8_t* buffer, uint32_t buffer_size, uint8_t* send_buffer, uint32_t send_buffer_size);
/**
 * 析构函数
 * @param self: 对象实例指针
 * @return
 */
void TComProtocolDestroy(TComProtocol *self);

/**
 * 传入数据(解析后会调用通知函数)
 * @param self: 对象实例指针
 * @param data: 获取到的数据
 * @param data_len: 获取到的数据长度
 * @return 无
 */
void TComProtocolTransfer(TComProtocol *self, uint8_t *data, uint32_t data_len);

/**
 * 发送输入量(上行)
 * @param self: 对象实例指针
 * @param switch_body: 输入量内容
 * @return 无
 */
bool TComProtocolSendSwitchValue(TComProtocol *self, TProtocolPackBodySwitch *switch_body);

/**
 * 发送ADC值(上行)
 * @param self: 对象实例指针
 * @param value: ad值
 * @param cnt: ad值个数
 * @return 无
 */
bool TComProtocolSendAdcValue(TComProtocol *self, UInt32 *value, UInt32 cnt);
/**
 * 发送CAN发送结果(上行)
 * @param self: 对象实例指针
 * @param idx: can通道
 * @param send_cnt: can发送个数
 * @param fail_cnt: can失败个数
 * @return 
 */
bool TComProtocolSendCanSendResult(TComProtocol *self, UInt8 idx, UInt32 send_cnt, UInt32 fail_cnt);

/**
 * 发送CAN接收结果(上行)
 * @param self: 对象实例指针
 * @param idx: can通道
 * @param rec_cnt: can接收个数
 * @return 
 */
bool TComProtocolSendCanRecResult(TComProtocol *self, UInt8 idx, UInt32 rec_cnt);
/**
 * 发送串口发送结果(上行)
 * @param self: 对象实例指针
 * @param idx: 串口通道
 * @param send_cnt: can发送个数
 * @param fail_cnt: can失败个数
 * @return 
 */
bool TComProtocolSendUartSendResult(TComProtocol *self, UInt8 idx, UInt32 send_cnt, UInt32 fail_cnt);

/**
 * 发送串口接收结果(上行)
 * @param self: 对象实例指针
 * @param idx: 串口通道
 * @param rec_cnt: 串口接收个数
 * @param rec_fail_cnt: 串口接收失败个数
 * @return 
 */
bool TComProtocolSendUartRecResult(TComProtocol *self, UInt8 idx, UInt32 rec_cnt, UInt32 rec_fail_cnt);

#ifdef __cplusplus
}
#endif
#endif
