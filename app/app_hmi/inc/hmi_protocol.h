/**
 * @file hmi_procol.h
 * @brief 交互协议解析
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
#ifndef _HMI_PROTOCOL_H_
#define _HMI_PROTOCOL_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 数据传输命令
typedef enum THmiProtocolCmdTag THmiProtocolCmd;
enum THmiProtocolCmdTag
{
    kHmiProtocolCmdHostCtrl             = 0x00,     ///< 主机控制命令
    kHmiProtocolCmdHostFile             = 0x01,     ///< 主机文件传输命令
    kHmiProtocolCmdHostParamSet         = 0x02,     ///< 主机参数设置命令
    kHmiProtocolCmdHostParamReq         = 0x03,     ///< 主机参数查询命令
    kHmiProtocolCmdHostProductTest      = 0x04,     ///< 主机生产测试命令
    
    kHmiProtocolCmdSlaveCtrlResp        = 0x80,     ///< 从机控制命令应答
    kHmiProtocolCmdSlaveFile            = 0x81,     ///< 从机文件传输
    kHmiProtocolCmdSlaveParamSetResp    = 0x82,     ///< 从机参数设置应答
    kHmiProtocolCmdSlaveParamReqResp    = 0x83,     ///< 从机参数查询应答
    kHmiProtocolCmdSlaveProductTest     = 0x84,     ///< 从机生产测试命令
};


typedef struct THmiProtocolTag THmiProtocol;
/**
 * 命令通知函数
 * @param receiver: 接口所有者
 * @param host_source: 主机数据源
 * @param ser_num: 流水号
 * @param cmd: 收到的命令
 * @param data: 命令数据指针
 * @param data_len: 命令数据长度
 * @return 
 */
typedef void (*THmiProtocolMsgNotify)(void *receiver, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len);

/**
 * 消息实际发送函数
 * @param receiver: 接口所有者
 * @param data: 发送数据地址
 * @param len: 发送数据长度
 * @return 0:失败 >0:发送成功个数
 */
typedef UInt32 (*THmiProtocolDataSend)(void *receiver, UInt8 *data, UInt32 len);

struct THmiProtocolTag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数
    
PUBLIC
    THmiProtocolMsgNotify     method_notify_;     ///< 状态通知方法(调用者实现)
    THmiProtocolDataSend      method_send_;       ///< 发送消息方法(调用者实现)
    void                     *receiver_;          ///< 接收者
PRIVATE
    UInt8           *buffer_;                         ///< buffer缓冲区指针
    UInt8           *send_buffer_;                    ///< 发送缓冲区指针
    UInt32           buffer_size_;                     ///< buffer缓冲区大小
    UInt32           send_buffer_size_;                ///< 发送缓冲区大小
    UInt32           now_size_;                        ///< 当前数据长度
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
Bool THmiProtocolCreate(THmiProtocol *self, UInt8* buffer, UInt32 buffer_size, UInt8* send_buffer, UInt32 send_buffer_size);

/**
 * 传入数据(解析后会调用通知函数)
 * @param self: 对象实例指针
 * @param data: 获取到的数据
 * @param data_len: 获取到的数据长度
 * @return 无
 */
void THmiProtocolTransfer(THmiProtocol *self, UInt8 *data, UInt32 data_len);

/**
 * 发送消息
 * @param self: 对象实例指针
 * @param host_source: 主机数据源
 * @param ser_num: 流水号
 * @param cmd: 发送命令命令
 * @param data: 命令数据地址
 * @param data_len: 命令数据长度
 * @return True:成功 False:失败
 */
Bool THmiProtocolSendMsg(THmiProtocol *self, UInt8 host_source, UInt8 ser_num, UInt8 cmd, UInt8* data, UInt32 data_len);

#ifdef __cplusplus
}
#endif
#endif
