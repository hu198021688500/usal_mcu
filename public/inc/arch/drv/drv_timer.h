/**
 * @file drv_timer.h
 * @brief 定时器驱动,用于产生周期性触发事件
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

#ifndef _DRV_TIMER_H_
#define _DRV_TIMER_H_

#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

///// 最多支持的定时器个数
//#define MAX_TIMER_COUNT 16

//typedef struct TDrvTimerSrvTag TDrvTimerSrv;

///**
// * 定时器,用于产生周期性触发事件
// * 注:
// * 1. 创建定时器TDrvTimer前必须先要创建TDrvTimerSrv的实例(仅需一次即可)
// * 2. 创建的定时器不允许删除
// * @par 示例
// * @code
//   TDrvTimer *timer = TDrvTimerSrvCreateTimer(&g_drv_timer_server);
//   timer->receiver = self;
//   timer->OnTimer = OnTimer;
//   TDrvTimerSetInterval(timer, 10);
//   TDrvTimerStart(timer);
//   ...
//   TDrvTimerStop(timer);
//   // OfDestroyObject((TOfObject *)&timer); // 注: 创建的定时器不允许删除
// * @endcode
//*/
//typedef struct TDrvTimerTag TDrvTimer;

///**
// * 定时器定时事件的函数类型
// * @param sender: 事件的产生者
// * @return
// */
//typedef void (*TDrvTimerOnTimerEvent)(TDrvTimer *sender);

//struct TDrvTimerTag
//{
//PROTECTED
//    TOfObject parent_;         ///< 继承的父类
//    TOfVirtualMethod Destroy_; ///< 析构函数

//PUBLIC
//    void *receiver;                ///< 事件的接收者
//    TDrvTimerOnTimerEvent OnTimer; ///< 定时事件; 注: 该事件在中断中调用

//PRIVATE
//    UInt32 interval_;   ///< 定时器间隔
//    UInt32 tick_count_; ///< 当前计数值
//    Bool enabled_;            ///< 定时器是否使能
//};


///**
// * 获取定时器的定时间隔
// * @param self: 对象实例指针
// * @return
// */
//UInt32 TDrvTimerInterval(TDrvTimer *self);

///**
// * 设置定时间隔
// * @param self: 对象实例指针
// * @param ms: 定时间隔,单位ms
// * @return
// */
//void TDrvTimerSetInterval(TDrvTimer *self, UInt32 ms);

///**
// * 启动定时器
// * 注: 启动后首次OnTimer事件会发生在定时间隔减一到定时间隔之间; 如果定时间隔设置为1,则会在定时器使能时刻到1ms时触发OnTimer
// * @param self: 对象实例
// * @return
// */
//void TDrvTimerStart(TDrvTimer *self);

///**
// * 停止定时器
// * @param self: 对象实例
// * @return
// */
//void TDrvTimerStop(TDrvTimer *self);

///**
// * 定时器服务,用于管理其它TDrvTimer
// * 注:创建定时器TDrvTimer前必须先要创建TDrvTimerSrv的实例(仅需一次即可
// * @par 示例
// * @code
//   TDrvTimerSrvCreate(&g_drv_timer_srv);
//   TDrvTimerSrvOpen(&g_drv_timer_srv);
//   ...
//   TDrvTimerSrvClose(&g_drv_timer_srv);
//   OfDestroyObject((TOfObject *)&g_drv_timer_srv);
// * @endcode
//*/
//struct TDrvTimerSrvTag
//{
//    TOfObject parent_;  ///< 继承的父类
//    TOfVirtualMethod Destroy_;  ///< 析构函数

//PRIVATE
//    //FTM_Type *base_;  ///< 定时器寄存器组的基地址
//    
//    Int32 timer_index_;     ///< 定时器序列
//    
//    TDrvTimer timers_[MAX_TIMER_COUNT];  ///< 定时器列表
//    Int32 count_;  ///< 当前的定时器个数
//    Int32 index_;  ///< 可取的索引值
//    Bool is_open_;  ///< 是否已经打开
//};

///**
// * 定时服务器器构造函数
// * @param self: 对象实例指针
// * @return
// */
//void TDrvTimerSrvCreate(TDrvTimerSrv *self);

///**
// * 打开定时服务器器
// * @param self: 对象实例指针
// * @return
// */
//void TDrvTimerSrvOpen(TDrvTimerSrv *self);

///**
// * 关闭定时服务器器(同时会停止所有定时器)
// * @param self: 对象实例指针
// * @return
// */
//void TDrvTimerSrvClose(TDrvTimerSrv *self);

///**
// * 创建定时器对象.
// * 注:
// *   1. 定时器一旦创建,无法释放
// *   2. 当定时器个数超过MAX_TIMER_COUNT时,创建失败
// * @param self: 对象实例指针
// * @return 创建的定时器对象, 该定时器不允许外部释放.如果创建失败返回NULL
// */
//TDrvTimer *TDrvTimerSrvCreateTimer(TDrvTimerSrv *self);

//extern TDrvTimerSrv g_drv_timer_srv;  ///< 定时服务器对象

//extern volatile UInt32 g_timer_srv_tick;  ///< 定时服务器产生的tick值.仅在定时器服务器创建后有效(TDrvTimerSrvCreate)


typedef struct TDrvTimerTag TDrvTimer;


/**
 * 中断事件函数
 * @note: 由中断中调用
 * @param sender: 事件的产生者
 * @return
 */
typedef void (*TDrvTimerIrq)(void *sender);

struct TDrvTimerTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PUBLIC
    void            *recevier;      ///< 事件接收者
    TDrvTimerIrq     MethodOnIrq;   ///< 中断事件;
    
PRIVATE
    
    Bool             is_open_;      ///< 是否已经打开
    Int32            timer_index_;  ///< 定时器序列
};

/**
 * 构造函数
 * @param self: 对象实例指针
 * @param index: 项索引: 0..2
 * @return True:成功 False:失败
 */
Bool TDrvTimerCreate(TDrvTimer *self);

/**
 * 驱动打开
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvTimerOpen(TDrvTimer *self);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvTimerClose(TDrvTimer *self);


extern TDrvTimer g_drv_timer;

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_TIMER_H_
