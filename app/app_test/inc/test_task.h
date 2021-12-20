/**
 * @file uart_task.h
 * @brief 串口主任务
 * @note 
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */

#ifndef _APP_TEST_TASK_H_
#define _APP_TEST_TASK_H_

#include <ofc_object.h>
#include <kfc_task.h>
#include "com_protocol.h"
#include "kfc_timer.h"
#include "can_def.h"
#include "test_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/// can处理
typedef struct TTestTaskCanProcessTag TTestTaskCanProcess;
struct TTestTaskCanProcessTag
{
    uint8_t  is_send;               ///< 是否发送
    uint8_t  is_rec;                ///< 是否接收
    uint32_t  send_inv;             ///< 发送间隔
    uint32_t  send_total;           ///< 发送总数
    uint32_t  send_cnt;             ///< 当前发送总数
    uint32_t  send_fail_cnt;        ///< 发送失败总数
    uint32_t  send_tick;            ///< 发送间隔
    TCanData  send_data;            ///< 发送数据
    
    uint32_t  rec_id;               ///< 接收id
    uint32_t  rec_cnt;              ///< 接收个数
};
/// uart处理
typedef struct TTestTaskUartProcessTag TTestTaskUartProcess;
struct TTestTaskUartProcessTag
{
    
    TTestProtocol  test_protocol_;    ///< ccu协议解析对象
    UInt8         protocol_buffer_[512]; ///< 协议解析缓存buffer
    UInt8         protocol_send_buffer_[200]; ///< 协议发送缓存
    
    uint8_t  is_send;               ///< 是否发送
    uint8_t  is_rec;                ///< 是否接收
    uint32_t  send_size;            ///< 发送单包大小
    uint32_t  send_inv;             ///< 发送间隔
    uint32_t  send_total;           ///< 发送总数
    uint32_t  send_cnt;             ///< 当前发送总数
    uint32_t  send_fail_cnt;        ///< 发送失败总数
    uint32_t  send_tick;            ///< 发送间隔
    
    uint32_t  rec_cnt;              ///< 接收个数
    uint32_t  rec_fail_cnt;         ///< 接收失败个数
    
    uint32_t  send_status;          ///< 发送状态 0:空闲 1:发送 2:接收
    uint32_t  wait_resp_tick;       ///< 等待应答间隔 1s
    uint32_t  resp_result;          ///< 0:无应答 1:成功 2:失败
};
/**
 * 主任务类
 * @par 示例 
 * @code
    TTestTask test;

    TTestTaskCreate(&test);
    OfDestroyObject((TOfObject *)&test);
 * @endcode
 */
typedef struct TTestTaskTag TTestTask;
struct TTestTaskTag
{
PROTECTED
    TKfcTask parent_;                ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    
    TComProtocol  com_protocol_;    ///< ccu协议解析对象
    UInt8         protocol_buffer_[512]; ///< 协议解析缓存buffer
    UInt8         protocol_send_buffer_[200]; ///< 协议发送缓存
    UInt8         com_read_buf_[512];
    
    Bool          com_is_open_;     ///< 测试串口是否打开
    
    TKfcTimer    *start_timer_;     ///< 启动定时器
    UInt32        start_idx_;       ///< 启动项
    TKfcTimer    *switch_timer_;    ///< 输入上报定时器
    TKfcTimer    *adc_timer_;       ///< adc上报定时器
    TKfcTimer    *can_timer_;       ///< can上报定时器
    TKfcTimer    *uart_timer_;      ///< uart上报定时器
    
    TKfcTimer    *run_timer_;       ///< 运行定时器
    
    TTestTaskCanProcess can_process_[3];    ///< can发送接收处理
    TCanData     can_rec_data_[20];
    
    Int32       adc_value_[16];        ///< adc值
    
    TTestTaskUartProcess uart_process_[2];  ///< 串口处理
    UInt8        uart_send_data_[100];      ///< 串口发送缓存
    UInt8        uart_rec_data_[100];      ///< 串口接收缓存
    
    UInt32       led_status_;               ///< 运行灯状态
};
/// 主任务对象
extern TTestTask g_test_task;

/**
 * 主任务创建
 * @param self: 对象实例指针
 * @return 
 */
void TTestTaskCreate(TTestTask *self);


#ifdef __cplusplus
}
#endif 

#endif //_APP_UART_TASK_H_
