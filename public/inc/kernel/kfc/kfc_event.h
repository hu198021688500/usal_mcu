/**
 * @file kfc_event.h
 * @brief 定义TKfcEvent类
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
#ifndef _KFC_EVENT_H_
#define _KFC_EVENT_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

enum
{
    kSfEventAllFlags = 0x7FFFFFFF,  ///< 事件所有有效位的值
};

/**
 * 构造TKfcEvent类,类似于TThreadEvent.跟TThreadEvent的区别在于:
 * TSfEvent可在一个线程中创建多个,而一个线程只能创建一个ThreadEven
 * @par 示例
 * @code
   Thread A:
   TKfcEvent event;
   TKfcEventCreate(&event);
   ...
   if (TKfcEventWait(&event, 3, False, 100) > 0) ... // 等到flag为0或者1的事件产生,最多等到100ms

   Thread B:
   TKfcEventSetFlag(&event, 0);  // flag 0表示B中设置

   中断中:
   TKfcEventSetFlag(&event, 1);  // flag 1表示中断中设置

   OfDestroyObject((TOfObject *)event);
 * @endcode
*/
typedef struct TKfcEventTag TKfcEvent;
struct TKfcEventTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    //DRtosEventFlagsHandle handle_;
    void*   handle_;
};

/**
 * 事件类的构造函数
 * @param self: 对象实例指针
 * @return
 */
void TKfcEventCreate(TKfcEvent *self);

/**
 * 设置事件标记
 * 注: 这个函数可以在任何地方调用,包括在中断中调用
 * @param self: 对象实例
 * @param flag_bit_index: 需要设置的事件标记索引.取值范围为: 0-30
 * @return 返回true表示设置该标记值时,等待函数正在等这个标记.否则表示等待函数没有在等待该标记
 */
Bool TKfcEventSetFlag(TKfcEvent *self, Int32 flag_bit_index);

/**
 * 设置事件的多个标记
 * 注: 这个函数可以在任何地方调用,包括在中断中调用
 * @param self: 对象实例
 * @param flags: 需要设置的事件标记集合; 每个bit为一个事件标记.Bit31不允许设置;
 * @return 返回true表示设置该标记值时,等待函数正在等这个标记.否则表示等待函数没有在等待该标记
 */
Bool TKfcEventSetFlags(TKfcEvent *self, UInt32 flags);

/**
 * 等待flags指定的一个或者多个事件.
 * 注: !!!!!!!!!!!!!!!!!!!!!!!!!!!
 *   如果event的flags为1011(二进制), 如果参数flags为1001(二进制),则返回值为1011, 之后event的flags变成0010
 *   注意,这里bit1的1没有被清除.之后如果有设置了flags为1100, 则event的flags=1110, 然后等待的参数flags为1000
 *   则函数返回值为1110,event的flags为0110; 切记,参数flags不设置为1的bit,如果event的falgs对应bit为1,则该bit会一直为1
 * @param self: 对象实例
 * @param flags: 需要等待的事件集合,仅bit0-bit30有效,bit31不得设置为1.比如flags=9(=1001B),表示等待flag=0和(或)3的事件
 * @param all: 是否需要等到flags指定的所有标记都置位才返回.
 *             true: 表示flags指定的所有事件都等到才返回; false: 表示只需要等到flags指定的任意一个事件置位就返回
 * @param timeout: 等待的时长,单位: ms. 0: 表示不等待,立即返回, -1: 表示永久等待,直到事件产生
 * @return 返回等到要求时的事件集合, <0表示超时或者出错
 */
Int32 TKfcEventWait(TKfcEvent *self, Int32 flags, Bool all, Int32 timeout);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_EVENT_H_
