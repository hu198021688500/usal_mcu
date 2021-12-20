/**
 * @file kfc_mutex.h
 * @brief 定义互斥锁类
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

#ifndef _KFC_MUTEX_H_
#define _KFC_MUTEX_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造加锁类
 * @par 示例
 * @code
   Thread A:
   TKfcMutex Mutex;
   TKfcMutexCreate(&Mutex);
   ...
   TKfcMutexLock(&Mutex, 100) ... // 申请锁最多等待100ms

   TKfcMutexUnlock(&Mutex);
    
   OfDestroyObject((TOfObject *)Mutex);
 * @endcode
*/
typedef struct TKfcMutexTag TKfcMutex;
struct TKfcMutexTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    void    *handle_;
};

/**
 * 临界区类的构造函数
 * @param self: 对象实例指针
 * @return True: 成功; False: 失败
 */
Bool TKfcMutexCreate(TKfcMutex *self);

/**
 * 获得临界区.当前线程如果获得临界区,则当前线程就是临界区的所有者,直到当前线程调用TMutexRelease释放临界区
 * @param self: 对象实例
 * @param timeout: 等待的时长,单位: ms. 0: 表示不等待,立即返回, -1: 表示永久等待,直到得到这个临界区
 * @return 返回true表示获得临界区成功.否则表示获得临界区失败或者超时
 */
Bool TKfcMutexLock(TKfcMutex *self, Int32 timeout);

/**
 * 释放临界区
 * @param self: 对象实例
 * @return
 */
void TKfcMutexUnlock(TKfcMutex *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_MUTEX_H_
