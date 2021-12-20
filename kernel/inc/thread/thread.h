/**
 * @file thread.h
 * @brief 定义Thread类
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

#ifndef _KERNEL_THREAD_H_
#define _KERNEL_THREAD_H_

#include <ofc_object.h>
#include "rtos_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造一个线程类
 * @par 示例
 * @code
    __align(8) static char stack[512];
    TThreadCreate(&self->thread_, kSfThreadPriorityNormal, stack, sizeof(stack));
    self->thread_.receiver = (void *)self;
    self->thread_.OnRunOnce = TMainTaskRunOnce;
    if (TThreadResume(&self->thread_))
    {
		...
	   
    }
   
    TThreadTerminate(&self->thread_);
    TThreadWaitFor(&self->thread_, 3000);
    OfDestroyObject((TOfObject *)&self->thread_);
 * @endcode
 */

typedef struct TThreadTag TThread;
typedef Int32 (*TThreadExecute)(void *sender);

struct TThreadTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    DRtosThreadHandle handle_; ///< 线程句柄
    void *stack_;      ///< 线程的堆栈
    Int32 stack_size_;   ///< 堆栈大小
    Bool terminated_;  ///< 线程结束标记
    TRtosThreadPriority priority_;  ///< 线程优先级

PUBLIC
    void *receiver;  ///< 存放用户数据.一般存放OnExecute或者OnRunOnce的对象实例

    TThreadExecute OnExecute;  ///< 执行事件. 如果这个事件被赋值的话,OnRunOnce事件就无效
    TThreadExecute OnRunOnce;  ///< 执行一次事件.事件的返回值用于告知线程接下来Sleep的时长,单位: ms.
};

/**
 * 线程类的构造函数.
 * 注: 构造函数并没有创建线程,需要在TThreadResume后创建线程
 * @param self: 对象实例指针
 * @param priority: 线程的优先级.**注意**: 高优先级的线程如果不等待,低优先级的线程将得不到运行的机会
 * @param stack: 线程的堆栈,可以为空,用默认的堆栈; 注意: 线程的堆栈必须是8字节对齐的,既: 堆栈的起始地址必须是8的倍数
 * @param stack_size: 线程的堆栈大小,可以为0,用默认的堆栈大小
 * @return
 */
void TThreadCreate(TThread *self, TRtosThreadPriority priority, void *stack, Int32 stack_size);

/**
 * 创建线程对象
 * 注: 构造函数并没有创建线程,需要在TThreadResume后创建线程
 * @param priority: 线程的优先级.**注意**: 高优先级的线程如果不等待,低优先级的线程将得不到运行的机会
 * @param stack: 线程的堆栈,可以为空,用默认的堆栈; 注意: 线程的堆栈必须是8字节对齐的,既: 堆栈的起始地址必须是8的倍数
 * @param stack_size: 线程的堆栈大小,可以为0,用默认的堆栈大小
 * @return 返回创建的对象
 */
TThread *CreateThread(TRtosThreadPriority priority, void *stack, Int32 stack_size);

/**
 * 线程如果在创建时挂起,则恢复运行.如果已经运行则不作操作
 * @param self: 对象实例
 * @return
 */
Bool TThreadResume(TThread *self);

/**
 * 终止线程的允许.事实上只是设置线程的terminated_变量为true,是否真正退出要看执行函数有没有退出
 * @param self: 对象实例指针
 * @return
 */
void TThreadTerminate(TThread *self);

/**
 * 返回线程是否需要终止线程
 * @param self: 对象实例指针
 * @return true: 需要终止; false: 不需要终止
 */
Bool TThreadTerminated(TThread *self);

/**
 * 等待线程退出
 * @param self: 对象实例指针
 * @param timeout: 超时时长,单位: ms; 注: 该参数在RTOS系统上没有用,填0即可
 * @return True: 成功: False: 失败
 */
Bool TThreadWaitFor(TThread *self, UInt32 timeout);

/**
 * 返回线程的句柄
 * @param self: 对象实例指针
 * @return 线程句柄.如果线程没有运行起来,返回的句柄为NULL
 */
DRtosThreadHandle TThreadHandle(TThread *self);

/**
 * 返回线程是否在运行
 * @param self: 对象实例指针
 * @return 线程正在运行,返回true,否则返回false
 */
Bool TThreadRunning(TThread *self);

/**
 * 线程ID,RTX操作系统为句柄,linux为线程Id
 * @param self: 对象指针
 * @return 返回线程ID
 */
long TThreadThreadId(TThread *self);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SFC_THREAD_H_
