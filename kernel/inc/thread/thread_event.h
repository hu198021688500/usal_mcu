/**
 * @file thread_event.h
 * @brief 定义TThreadEvent类
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

#ifndef _KERNEL_THREAD_EVENT_H_
#define _KERNEL_THREAD_EVENT_H_

#include <ofc_object.h>
#include "thread.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造TThreadEvent类
 * 线程事件类仅构造时指定的线程可以拥有该对象,并且可调用该对象的所有方法.
 * 其它线程和中断中不得调用这些方法,仅TThreadEventSet方法可在其它线程和中断中调用.
 *
 * 线程事件机制:
 * 线程事件相当于线程有一个且仅有一个标记集合(设为F),F包含31个标记(由一个32bit的整型来存放).
 * F的每个bit为一个flag,从bit0到bit30,共计31个flag.
 *
 * 当其它线程或者中断调用TThreadEventSet时,该函数的参数flag用于指示标记集合中的哪个标记被置位.
 * flag的取值范围为0到30. 这里的flag实际上为F中的位的索引
 * 例如:
 * F = 0x00000000
 * TThreadEventSet(xx, 0); F: 0x00 -> 0x01
 * TThreadEventSet(xx, 1); F: 0x01 -> 0x03
 * TThreadEventSet(xx, 4); F: 0x03 -> 0x13
 *
 * TSfThreadEventWait函数用于等待事件.该函数的flags用于指示需要等待哪些标志置位.
 * @par 示例
 * @code
   Thread A:
   TThreadEvent event;
   TThreadEventCreate(&event, self);
   ...
   if (TThreadEventWait(event, 3, False, 100) ... // 等到flag为0或者1的事件产生,最多等到100ms

   Thread B:
   TThreadEventSet(&event, 0);  // flag 0表示B中设置

   中断中:
   TThreadEventSet(&event, 1);  // flag 1表示中断中设置

   OfDestroyObject((TOfObject *)event);
 * @endcode
*/
typedef struct TThreadEventTag TThreadEvent;
struct TThreadEventTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    DRtosThreadHandle handle_;
};

/**
 * 线程事件类的构造函数
 * @param self: 对象实例指针
 * @param thread: 拥有该事件的线程
 * @return
 */
void TThreadEventCreate(TThreadEvent *self, TThread *thread);

/**
 * 设置事件标记
 * 注: 这个函数可以在任何地方调用,包括在中断中调用
 * @param self: 对象实例
 * @param flag: 需要设置的事件标记索引.flag的取值范围为: 0-30
 * @return 返回true表示设置该标记值时,等待函数正在等这个标记.否则表示等待函数没有在等待该标记
 */
Bool TThreadEventSet(TThreadEvent *self, Int32 flag);

/**
 * 等待flags指定的一个或者多个事件.
 * @param self: 对象实例
 * @param flags: 需要等待的事件集合,仅bit0-bit30有效,bit31不得设置为1.比如flags=9(=1001B),表示等待flag=0和(或)3的事件
 * @param all: 是否需要等到flags指定的所有标记都置位才返回.
 *             true: 表示flags指定的所有事件都等到才返回; false: 表示只需要等到flags指定的任意一个事件置位就返回
 * @param timeout: 等待的时长,单位: ms. 0: 表示不等待,立即返回, -1: 表示永久等待,直到事件产生
 * @return 返回true表示等到要求的事件, false表示超时或者出错
 */
Bool TThreadEventWait(TThreadEvent *self, Int32 flags, Bool all, Int32 timeout);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SFC_THREAD_EVENT_H_
