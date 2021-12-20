/**
 * @file rtos_api.h
 * @brief 操作系统相关的函数以及重新定义的变量类型和常量等
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

#ifndef _RTOS_API_H_
#define _RTOS_API_H_

#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *      DRtosThreadHandle;        ///< 线程句柄
typedef void *      DRtosEventFlagsHandle;    ///< 事件句柄
typedef void *      DRtosMutexHandle;         ///< 互斥锁句柄

/// 线程入口函数指针
typedef void (*TRtosThreadMethodEntry) (void *custom_param);
    
/**
 * 线程优先级的枚举
 */
typedef enum TRtosThreadPriorityTag TRtosThreadPriority;
enum TRtosThreadPriorityTag
{
    kRtosThreadPriority0                = 0,            ///< 优先级0
    kRtosThreadPriority1,
    kRtosThreadPriority2,
    kRtosThreadPriority3,
    kRtosThreadPriority4,
    kRtosThreadPriority5,
    kRtosThreadPriority6,
    kRtosThreadPriority7,
    kRtosThreadPriority8,
    kRtosThreadPriority9,
    kRtosThreadPriority10,
    kRtosThreadPriority11,
    kRtosThreadPriority12,
    kRtosThreadPriority13,
    kRtosThreadPriority14,
    kRtosThreadPriority15,
    
    kRtosThreadPriorityMax,

};

/**
 * 线程状态枚举
 */
typedef enum TRtosThreadStatusTag TRtosThreadStatus;
enum TRtosThreadStatusTag
{
    kRtosThreadStatusInactive                = 0,         ///< 未活跃
    kRtosThreadStatusReady,                               ///< 准备就绪
    kRtosThreadStatusRunning,                             ///< 运行中
    kRtosThreadStatusBlocked,                             ///< 已阻塞
    kRtosThreadStatusTerminated,                          ///< 已销毁
    kRtosThreadStatusError,                               ///< 错误
    
    kRtosThreadStatusMax,
};

/**
 * 事件配置
 */
typedef enum TRtosEventFlagsOptionsTag TRtosEventFlagsOptions;
enum TRtosEventFlagsOptionsTag
{
    kRtosEventFlagsOptionsWaitAny        = 0,        ///< 等待任何事件
    kRtosEventFlagsOptionsWaitAll,                   ///< 等待所有事件
};

/**
 * RTOS系统初始化
 * @param 
 * @return
 */
void RtosKernelInit(void);

/**
 * RTOS系统是否ready
 * @param 
 * @return True: 成功 False:失败
 */
Bool RtosKernelReady(void);

/**
 * RTOS系统启动
 * @param 
 * @return True: 成功 False:失败
 */
Bool RtosKernelStart(void);

/**
 * 线程休眠函数
 * @param ms: 休眠时长,单位: ms
 * @return
 */
void RtosSleep(UInt32 ms);

/**
* 得到当前的tick数,单位: 毫秒
* @return tick数
*/
UInt32 RtosGetTickCount(void);

/**
 * 内核锁定,禁止线程调度
 * 注:
 *   1. 不支持嵌套调用
 *   2. 锁定后应当快速处理完成,并立即调用RtosKernelUnlock
 *   3. 不得在中断中使用
 * @param
 * @return
 */
void RtosKernelLock(void);

/**
 * 内核解锁
 * @param
 * @return
 */
void RtosKernelUnlock(void);
/**
 * 内核挂起
 * @param
 * @return
 */
UInt32 RtosKernelSuspend(void);
/**
 * 内核挂起恢复
 * @param
 * @return
 */
void RtosKernelResume(UInt32 ms);

/**
 * 事件创建
 * @param
 * @return 事件句柄
 */
DRtosEventFlagsHandle RtosEventFlagsCreate(void);

/**
 * 事件设置
 * @param handle: 事件句柄
 * @param flags: 事件值
 * @return > 0事件值  < 0错误
 */
Int32 RtosEventFlagsSet(DRtosEventFlagsHandle handle, UInt32 flags);
/**
 * 事件等待
 * @param handle: 事件句柄
 * @param flags: 事件值
 * @return > 0事件值  < 0错误
 */
Int32 RtosEventFlagsWait(DRtosEventFlagsHandle handle, UInt32 flags, TRtosEventFlagsOptions option, UInt32 timeout);

/**
 * 事件销毁
 * @param handle: 事件句柄
 * @return True:成功 False:失败
 */
Bool RtosEventFlagsDestroy(DRtosEventFlagsHandle handle);
/**
 * 互斥锁创建
 * @param
 * @return 锁句柄
 */
DRtosMutexHandle RtosMutexCreate(void);

/**
 * 互斥锁申请锁
 * @param handle : 锁句柄
 * @param timeout : 申请超时
 * @return True:成功 False:失败
 */
Bool RtosMutexLock(DRtosMutexHandle handle, UInt32 timeout);

/**
 * 互斥锁释放锁
 * @param handle : 锁句柄
 * @return True:成功 False:失败
 */
Bool RtosMutexUnlock(DRtosMutexHandle handle);
/**
 * 互斥锁销毁
 * @param handle:锁句柄
 * @return 
 */
Bool RtosMutexDestory(DRtosMutexHandle handle);
/**
 * 创建线程
 * @param entry : 入口函数
 * @param custom_param : 自定义参数
 * @param priority : 优先级
 * @param stack : 堆栈地址
 * @param stack_size: 堆栈大小
 * @return 非空为线程句柄
 */
DRtosThreadHandle RtosThreadCreate(TRtosThreadMethodEntry entry, void *custom_param, TRtosThreadPriority priority   \
    , void *stack, UInt32 stack_size);
/**
 * 当前线程退出
 * @param
 * @return True:成功 False:失败
 */
Bool RtosThreadExit(void);
/**
 * 线程等待
 * @param handle : 线程句柄
 * @return True:成功 False:失败
 */
Bool RtosThreadJoin(DRtosThreadHandle handle);
/**
 * 线程事件设置
 * @param handle : 线程句柄
 * @param flags : 标记值
 * @return > 0事件值  < 0错误
 */
Int32 RtosThreadFlagsSet(DRtosThreadHandle handle, UInt32 flags);
/**
 * 当前线程进入等待事件
 * @param flags : 标记值
 * @param options : 配置
 * @param timeout : 超时时间
 * @return > 0事件值  < 0错误
 */
Int32 RtosThreadFlagsWait(Int32 flags, TRtosEventFlagsOptions options, Int32 timeout);

/**
 * 线程状态获取
 * @param handle : 线程句柄
 * @return 当前状态
 */
TRtosThreadStatus RtosThreadGeStatus(DRtosThreadHandle handle);
/**
 * 进入临界区(关中断)
 * @param
 * @return
 */
void RtosEnterCriticalSection(void);
/**
 * 退出临界区(开中断)
 * @param
 * @return
 */
void RtosLeaveCriticalSection(void);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _RtosC_API_H_
