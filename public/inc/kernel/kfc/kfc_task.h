/**
 * @file kfs_task.h
 * @brief 定义Task类
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

#ifndef _KFC_TASK_H_
#define _KFC_TASK_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造一个任务基类
 * 注: 任务基类不得直接使用,需要派生覆盖Execute_方法后才可以使用.
 * @par 示例
 * @code
    typedef struct TMainTaskTag TMainTask;
    struct TMainTaskTag
    {
    PRIVATE
        TKfcTask parent_;
        TOfVirtualMethod Destroy_;
        
    };

    static void TMainTaskDestroy(TOfObject *sender)
    {
        TMainTask *self = (TMainTask *)sender;
        // ...
        self->Destroy_(sender);
    }

    static void TMainTaskExecute(TOfObject *sender)
    {
        TMainTask *self = (TMainTask *)sender;

        while (!TKfcTaskTerminated((TKfcTask *)self))
        {
            // ...
            SfSleep(10);
        }
    }
    void TMainTaskCreate(TMainTask *self)
    {
        __align(8) static char stack[512];

        TKfcTaskCreate((TKfcTask *)self, kKfcTaskPriority1, stack, sizeof(stack));
        OverrideMethod(TOfObject, Destroy, TMainTaskDestroy);
        self->parent_.OnExecute = TMainTaskExecute;
        
        TKfcTaskStart(&self->parent_);
    }
 * @endcode
*/

typedef enum TKfcTaskStateTag TKfcTaskState;
enum TKfcTaskStateTag
{
    kKfcTaskStateStopped,  ///< 任务处于停止状态
    kKfcTaskStateStart,  ///< 任务开始执行
    kKfcTaskStateRunning,  ///< 任务处于运行状态
    kKfcTaskStateStop,  ///< 任务停止执行
    kKfcTaskStateStopping,  ///< 任务正在停止
};

   
/**
 * 任务优先级的枚举(正向,数字越大,任务优先级越高)
 */
typedef enum TKfcTaskPriorityTag TKfcTaskPriority;
enum TKfcTaskPriorityTag
{
    kKfcTaskPriority0                = 0,            ///< 优先级0
    kKfcTaskPriority1,
    kKfcTaskPriority2,
    kKfcTaskPriority3,
    kKfcTaskPriority4,
    kKfcTaskPriority5,
    kKfcTaskPriority6,
    kKfcTaskPriority7,
    kKfcTaskPriority8,
    kKfcTaskPriority9,
    kKfcTaskPriority10,
    kKfcTaskPriority11,
    kKfcTaskPriority12,
    kKfcTaskPriority13,
    kKfcTaskPriority14,
    kKfcTaskPriority15,
    
    kKfcTaskPriorityMax,
};

typedef struct TKfcTaskTag TKfcTask;
struct TKfcTaskTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PUBLIC
    TOfNotifyEvent OnCreate;  ///< 线程启动后立即执行的事件
    TOfNotifyEvent OnStart;  ///< 任务开始时执行的事件
    TOfNotifyEvent OnExecute;  ///< 任务运行事件
    TOfNotifyEvent OnStop;  ///< 任务停止后执行的事件

PRIVATE
    void         *thread_;  ///< 任务的执行线程
    TKfcTaskState state_;   ///< 任务状态
    void         *stack_;   ///< 任务堆栈地址
    UInt32        stack_size_;  ///< 任务堆栈大小
};

/**
 * 任务类的构造函数(使用需保证OfMalloc和OfFree可以使用)
 * @param self: 对象实例指针
 * @param priority: 任务线程的优先级
 * @param stack: 任务线程的堆栈,可以为空,用默认的堆栈; 注意: 线程的堆栈必须是8字节对齐的,既: 堆栈的起始地址必须是8的倍数
 * @param stack_size: 任务线程的堆栈大小,可以为0,用默认的堆栈大小
 * @return
 */
void TKfcTaskCreate(TKfcTask *self, TKfcTaskPriority priority, void *stack, Int32 stack_size);

/**
 * 任务在创建后挂起,使任务恢复运行
 * @param self: 对象实例
 * @return
 */
Bool TKfcTaskResume(TKfcTask *self);

/**
 * 任务开始运行.
 * @param self: 对象实例
 * @return
 */
void TKfcTaskStart(TKfcTask *self);

/**
 * 停止任务.
 * @note: 需要TKfcTaskWaitFor来等待任务结束
 * @param self: 对象实例
 * @return
 */
void TKfcTaskStop(TKfcTask *self);

/**
 * 返回当前任务是否在运行
 * @param
 * @return 任务在运行返回true,否则为false
 */
Bool TKfcTaskRunning(TKfcTask *self);

/**
 * 返回当前任务是否已停止
 * @param
 * @return 任务在运行返回true,否则为false
 */
Bool TKfcTaskStopped(TKfcTask *self);

/**
 * 判断当前任务是否需要结束
 * @note: 本函数应该在OnExecute中调用来判断是否需要停止
 * @param
 * @return 任务需要结束返回true,否则返回false
 */
Bool TKfcTaskTerminated(TKfcTask *self);

/**
 * 等待任务停止.
 * @note: 需要TKfcTaskWaitFor来等待任务结束
 * @param self: 对象实例
 * @param timeout: 等待时长,单位: ms
 * @return
 */
Bool TKfcTaskWaitFor(TKfcTask *self, Int32 timeout);

/**
 * 堆栈检查
 * @param self: 对象实例
 * @param free: 剩余堆栈大小
 * @param used: 最大使用大小
 * @return
 */
Bool TKfcTaskStackCheck(TKfcTask *self, Int32 *free, Int32 *used);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_TASK_H_
