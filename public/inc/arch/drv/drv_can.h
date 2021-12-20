/**
 * @file drv_can.h
 * @brief CAN驱动
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
#ifndef _DRV_CAN_H_
#define _DRV_CAN_H_
#include <ofc_object.h>
#include <ofc_types.h>
#include <ofc_ring_buf.h>
#include "can_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Can驱动波特率的枚举
 */
typedef enum TDrvCanBaudRateTag TDrvCanBaudRate;
enum TDrvCanBaudRateTag
{
    kCanBaudRate250k    = 0,  ///< 波特率为250K
    kCanBaudRate500k,         ///< 波特率为500K
    kCanBaudRate1000k,        ///< 波特率为1M
    kCanBaudRate100k,         ///< 波特率为100K
    kCanBaudRate125k,         ///< 波特率为125K
    
    kCanBaudRateMax,
};

 /**
  * 构造一个Can控制器对象.默认波特率为250K
  * @par 示例
  * @code
    TDrvCan Can;

    TDrvCanCreate(&Can);
    TDrvCanOpen(&Can);

    // TDrvCanWrite...
    // ...
    TDrvCanClose(&Can);
    OfDestroyObject((TOfObject *)&Can);
  * @endcode
 */
typedef struct TDrvCanTag TDrvCan;

struct TDrvCanTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;
PRIVATE
    const void *cfg_;  ///< CAN配置信息
    TOfRingBuf rx_buf_;  ///< 接收数据缓冲区
    TOfRingBuf tx_buf_;  ///< 发送数据缓冲区
    UInt32 tx_data_id_;  ///< 当前发送数据的数据id
    Int32 send_error_count_;  ///< 当前发送报文发送失败次数

    TDrvCanBaudRate baud_rate_;
    Bool is_open_;  ///< 是否已经打开
    Bool sending_;  ///< 数据正在发送中
    Bool auto_recover_;  ///< 自动从Bus Off状态恢复(默认为自动恢复)
    Bool baud_rate_detect_mode_;  ///< 波特率自动侦测模式
    Byte index_;  ///< CAN模块索引; 0: 第一路CAN...
    Byte preemptive_state_;  ///< 优先发送数据的状态(参见TPreemptiveStateTag)
    //TCanData preemptive_data_;  ///< 优先发送的数据


PUBLIC
     /// 存放事件接收者
//     void *receiver;

//     /// 报文发送完成事件
//     /// 注1: 仅data_id不为0的报文会产生这个回调
//     /// 注2: 数据发送的结果可能是失败
//     /// 注3: 如果数据未发完,进入BusOff状态,则该事件会在BusOff恢复后,正常发送完成或者发送超时后收到.
//     /// 注4: 该事件在中断中调用,故需要尽快退出
//     TDrvCanSentEvent OnSent;

//     /// 收到报文事件.注: 该事件在中断中调用,故需要尽快退出
//     TDrvCanRecvEvent OnRecv;

//     /// 总线错误事件.注: 该事件在中断中调用,故需要尽快退出; 如果不是自动从BusOff状态恢复,需要调用TDrvCanBusOffRecover函数
//     TDrvCanBusOffEvent OnBusOff;

//     /// 波特率侦测结果事件. 注: 该事件在中断中调用,故需要尽快退出; 如果检测成功表示有数据收到,可以调用读取函数读取数据
//     TDrvCanDetectEvent OnDetect;
};


/**
 * CAN驱动构造函数
 * @param self: 对象实例指针
 * @param index: CAN控制器索引,范围: 0..2
 * @param tx_buf: 发送缓冲区,不允许为空
 * @param tx_buf_size: 发送缓冲区的大小.缓冲区大小必须是sizeof(TCanData)的整数倍
 * @return
 */
void TDrvCanCreate(TDrvCan *self, Int32 index, void *tx_buf, Int32 tx_buf_size, void *rx_buf, Int32 rx_buf_size);

/**
 * 打开CAN驱动
 * @note: 
 * @param self: 对象实例指针
 * @param baud_index: 波特率配置项
 * @return 打开成功返回true,否则为false
 */
Bool TDrvCanOpen(TDrvCan *self, TDrvCanBaudRate baud_index);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return
 */
void TDrvCanClose(TDrvCan *self);

/**
 * 读取收到的CAN数据
 * @note: 接收报文的时间戳来自g_timer_srv的tick,故打开CAN驱动前需要先初始化g_timer_srv
 * @param self: 对象实例指针
 * @param buf: 存放CAN数据的缓冲区
 * @param count: Buf能容纳多少个TCANVData类型的数据
 * @return 返回实际读取到的TCANVData类型的数据个数
 */
Int32 TDrvCanRead(TDrvCan *self, TCanData *buf, Int32 count);

/**
 * 写CAN数据到发送队列.写完后会自动开始发送
 * @note: 如果是波特率侦测模式,则数据无法发送
 * @param self: 对象实例指针
 * @param data: 需要发送的CAN数据
 * @return True:成功 False:失败
 */
Bool TDrvCanWrite(TDrvCan *self, const TCanData *data);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif /// _DRV_CAN_H_
