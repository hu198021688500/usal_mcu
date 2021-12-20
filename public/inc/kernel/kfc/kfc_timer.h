/**
 * @file kfs_timer.h
 * @brief 定义Timer类
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

#ifndef _KFC_TIMER_H_
#define _KFC_TIMER_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif


/// 最多支持的定时器个数
#define KFC_MAX_TIMER_COUNT 16

/**
 * 定时器,用于产生周期性触发事件
 * 注:
 * 1. 创建定时器TDrvTimer前必须先要创建TDrvTimerSrv的实例(仅需一次即可)
 * 2. 创建的定时器不允许删除
 * @par 示例
 * @code
   TKfcTimer *timer = TKfcTimerSrvCreateTimer(&g_drv_timer_server);
   timer->receiver = self;
   timer->MethodOnTimerIrq = OnTimer;
   TKfcTimerSetInterval(timer, 10);
   TKfcTimerStart(timer);
   ...
   TKfcTimerStop(timer);
   // OfDestroyObject((TOfObject *)&timer); // 注: 创建的定时器不允许删除
 * @endcode
*/
typedef struct TKfcTimerTag TKfcTimer;

/**
 * 定时器定时事件的函数类型
 * @param sender: 事件的产生者
 * @return
 */
typedef void (*TKfcTimerOnTimerIrq)(void *receiver);

struct TKfcTimerTag
{
PROTECTED
    TOfObject parent_;         ///< 继承的父类
    TOfVirtualMethod Destroy_; ///< 析构函数

PUBLIC
    void *receiver;                ///< 事件的接收者
    TKfcTimerOnTimerIrq MethodOnTimerIrq; ///< 定时事件; 注: 该事件在中断中调用

PRIVATE
    UInt32 interval_;   ///< 定时器间隔
    UInt32 tick_count_; ///< 当前计数值
    Bool enabled_;            ///< 定时器是否使能
};


/**
 * 获取定时器的定时间隔
 * @param self: 对象实例指针
 * @return
 */
UInt32 TKfcTimerInterval(TKfcTimer *self);

/**
 * 设置定时间隔
 * @param self: 对象实例指针
 * @param ms: 定时间隔,单位ms
 * @return
 */
void TKfcTimerSetInterval(TKfcTimer *self, UInt32 ms);

/**
 * 启动定时器
 * 注: 启动后首次OnTimer事件会发生在定时间隔减一到定时间隔之间; 如果定时间隔设置为1,则会在定时器使能时刻到1ms时触发OnTimer
 * @param self: 对象实例
 * @return
 */
void TKfcTimerStart(TKfcTimer *self);

/**
 * 停止定时器
 * @param self: 对象实例
 * @return
 */
void TKfcTimerStop(TKfcTimer *self);




typedef struct TKfcTimerSrvTag TKfcTimerSrv;
struct TKfcTimerSrvTag
{
    TOfObject parent_;  ///< 继承的父类
    TOfVirtualMethod Destroy_;  ///< 析构函数

PRIVATE
    
    Int32 timer_index_;     ///< 定时器序列
    
    TKfcTimer timers_[KFC_MAX_TIMER_COUNT];  ///< 定时器列表
    Int32 count_;  ///< 当前的定时器个数
    Int32 index_;  ///< 可取的索引值
    Bool is_open_;  ///< 是否已经打开
};

/**
 * 定时服务器器构造函数
 * @param self: 对象实例指针
 * @return
 */
void TKfcTimerSrvCreate(TKfcTimerSrv *self);

/**
 * 打开定时服务器器
 * @param self: 对象实例指针
 * @return
 */
void TKfcTimerSrvOpen(TKfcTimerSrv *self);

/**
 * 关闭定时服务器器(同时会停止所有定时器)
 * @param self: 对象实例指针
 * @return
 */
void TKfcTimerSrvClose(TKfcTimerSrv *self);

/**
 * 创建定时器对象.
 * 注:
 *   1. 定时器一旦创建,无法释放
 *   2. 当定时器个数超过MAX_TIMER_COUNT时,创建失败
 * @param self: 对象实例指针
 * @return 创建的定时器对象, 该定时器不允许外部释放.如果创建失败返回NULL
 */
TKfcTimer *TKfcTimerSrvCreateTimer(TKfcTimerSrv *self);


extern TKfcTimerSrv g_kfc_timer_srv;  ///< 定时服务器对象


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_TIMER_H_
