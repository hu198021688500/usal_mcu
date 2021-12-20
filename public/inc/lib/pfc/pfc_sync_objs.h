/**
 * @file pfc_sync_objs.h
 * @brief 同步对象
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
#ifndef _PFC_SYNC_OBJS_H_
#define _PFC_SYNC_OBJS_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * 自旋锁对象
 * 警告: **** 使用自旋锁的几个线程必须具有相同优先级,否则会死锁 ****
 *       例如: H为高优先级线程,L为低优先级线程.L先进入自旋锁,在L退出自旋锁之前,系统切换到了H线程
 *       H线程等待L退出自旋状态,因为H线程没有调用等待函数或者事件,系统不会切换到L线程,因此2个线程处于死锁状态
 * @par 示例
 * @code
   TPfSpinLock lock;
   TPfSpinLockCreate(&lock);
   TPfSpinLockEnter(&lock);
   ....
   TPfSpinLockLeave(&lock);
   DeleteObject((TOfObject *)&lock);
 * @endcode
*/

typedef struct TPfSpinLockTag TPfSpinLock;
struct TPfSpinLockTag
{
PRIVATE
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

    Int32 value_;
};

/**
 * 自旋锁类的构造函数
 * @param self: 对象实例指针
 * @return
 */
void TPfSpinLockCreate(TPfSpinLock *self);

/**
 * 创建自旋锁对象
 * @return
 */
TPfSpinLock *PfCreateSpinLock(void);

/**
 * 进入自旋锁
 * @param self: 对象实例
 * @return
 */
void TPfSpinLockEnter(TPfSpinLock *self);

/**
 * 退出自旋锁
 * @param self: 对象实例
 * @return
 */
void TPfSpinLockLeave(TPfSpinLock *self);

/**
 * 进入自旋锁
 * @note: 用户需要自行初始化value的值为0
 * @param value: 自旋锁的值
 * @return 
 */
void PfSpinLock(Int32 *value);

/**
 * 退出自旋锁
 * @param value: 自旋锁的值
 * @return 
 */
void PfSpinUnlock(Int32 *value);

/**
 * 比较相等后交换数据
 * 相当于 if (*dst == comparand) *dst = exchange;
 * @param dst: 存放目标值
 * @param exchange: 设置到dst的值
 * @param comparand: 比较值
 * @return
 */
Int32 PfInterlockedCompareExchange(Int32 *dst, Int32 exchange, Int32 comparand);

/**
 * 实现*dst = *dst + value的功能.如果计算过程种*dst被另外线程修改,会重新计算求和
 * @param dst: 输入被加数,输出和
 * @param value: 输入加数
 * @return
 */
void PfInterlockedAdd(Int32 *dst, Int32 value);

/**
 * 实现*dst = *dst - value的功能.如果计算过程种*dst被另外线程修改,会重新计算求差
 * @param dst: 输入被减数,输出差
 * @param value: 输入减数
 * @return
 */
void PfInterlockedSub(Int32 *dst, Int32 value);

/**
 * 实现*dst = *dst + 1的功能
 * @param dst: 需要修改的目标
 * @return 返回本次调用实际赋值给*dst的值(注该值可能和调用后从*dst中取出的值不同)
 */
Int32 PfInterlockedInc(Int32 *dst);

/**
 * 实现*dst = *dst - 1的功能
 * @param dst: 需要修改的目标
 * @return 返回本次调用实际赋值给*dst的值(注该值可能和调用后从*dst中取出的值不同)
 */
Int32 PfInterlockedDec(Int32 *dst);

/**
 * 实现 *dst = *dat | value; 
 * @param dst: 需要修改的目标
 * @param value: 或操作的值
 * @return 
 */
void PfInterlockedOr( Int32 *dst, Int32 value );

/**
 * 实现 *dst = *dat & value; 
 * @param dst: 需要修改的目标
 * @param value: 与操作的值
 * @return 
 */
void PfInterlockedAnd( Int32 *dst, Int32 value );

#ifdef __cplusplus
}  // extern "C"
#endif

#endif// _PFC_SYNC_OBJS_H_
