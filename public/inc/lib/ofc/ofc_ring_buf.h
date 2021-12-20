/**
 * @file ofc_ring_buf.h
 * @brief 数据循环缓冲区,一读一写线程安全,无需加锁
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
#ifndef _OFC_RING_BUF_H_
#define _OFC_RING_BUF_H_

#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造一个循环缓冲区对象
 * @par 示例
 * @code
   char buf[1024];
   TOfRingBuf wbuf;
   TOfRingBufCreate(&wbuf, buf, sizeof(buf));
   // ...
   DestroyObject((TOfObject *)&wbuf);

 thread1:
   Int32 size = TOfRingBufWrite(&wbuf, data, data_size);

 thread2:
    char buf[1024]
    Int32 size = TOfRingBufRead(&wbuf, buf, sizeof(buf));
    // process data in buf
    ...
 * @endcode
*/
typedef struct TOfRingBufTag TOfRingBuf;
struct TOfRingBufTag
{
PRIVATE
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

    char *buf_;
    char *end_pos_;
    char *read_pos_;
    char *write_pos_;
    UInt32 write_size_;
    UInt32 read_size_;
};

/**
 * 类的构造函数
 * @param self: 对象实例指针
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
void TOfRingBufCreate(TOfRingBuf *self, void *buf, Int32 buf_size);

/**
 * 创建一个环形队列
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
TOfRingBuf *OfCreateRingBuf(void *buf, Int32 buf_size);


/**
 * 写入数据到缓冲区
 * @note 仅用于单线程(中断)操作
 * @param self: 对象实例
 * @param data: 写入的数据缓冲区
 * @param data_size: 数据大小
 * @return 返回实际写入数据的大小
 */
Int32 TOfRingBufWrite(TOfRingBuf *self, const void *data, Int32 data_size);

/**
 * 读取数据
 * @note 仅用于单线程(中断)操作
 * @param self: 对象实例
 * @param buf: 存放数据的缓冲区
 * @param buf_size: 缓冲区大小
 * @return 返回读取数据的大小
 */
Int32 TOfRingBufRead(TOfRingBuf *self, void *buf, Int32 buf_size);

/**
 * 读取数据(读取后读取点位置不变,而调用TOfRingBufRead后读取点位置后移)
 * @note 仅用于单线程(中断)操作
 * @param self: 对象实例
 * @param buf: 存放数据的缓冲区
 * @param buf_size: 缓冲区大小
 * @return 返回读取数据的大小
 */
Int32 TOfRingBufPeek(TOfRingBuf *self, void *buf, Int32 buf_size);

/**
 * 清除所有写入数据
 * @note 仅用于单线程(中断)操作,且必须和调用TOfRingBufRead的线程(中断)是同一个线程(中断)
 * @param self: 对象实例
 * @return
 */
void TOfRingBufClear(TOfRingBuf *self);

/**
 * 返回缓冲区的大小
 * @param self: 对象实例
 * @return 返回缓冲区大小
 */
Int32 TOfRingBufSize(TOfRingBuf *self);

/**
 * 返回空闲缓冲区大小
 * @param self: 对象实例
 * @return 返回空闲缓冲区大小
 */
Int32 TOfRingBufFreeSize(TOfRingBuf *self);

/**
 * 返回当前缓冲区可以读取的数据数
 * @param self: 对象实例
 * @return 返回读取到的数据数
 */
Int32 TOfRingBufDataSize(TOfRingBuf *self);

/**
 * 获取当前可以读取数据的缓冲区起始位置和可读取的数据大小
 * 注:
 *   1. 函数仅可在读取线程调用.
 *   2. 用户通过返回指针读取数据后,可以通过TOfRingBufPop函数来去除已经读取的数据
 *   3. 如果缓冲区发生转弯,需要分两次调用.第一次调用读取当前读取点到缓冲区末尾的数据,之后调用TOfRingBufPop,丢弃这些数据后再次调用
 * @param self: 对象实例
 * @param readable_size: 返回当前读取点有多少数据可以读取(如果缓冲区发生转弯,则返回值仅到缓冲区末尾); 可以为NULL
 * @return 如果有数据可以读取,则返回读取点的指针,否则返回NULL
 */
const char *TOfRingBufGetReadPos(TOfRingBuf *self, Int32 *readable_size);

/**
 * 从当前读取点开始,丢弃size个字节(内部自动处理转弯)
 * @note 仅读取线程可以操作该函数
 * @param self: 对象实例
 * @param size: 需要丢弃的字节数; 如果size比缓冲区中的字节数大,则全部丢弃
 * @return
 */
void TOfRingBufPop(TOfRingBuf *self, Int32 size);

/**
 * 获取当前可以写入数据的缓冲区起始位置和可写入的数据大小
 * 注:
 *   1. 函数仅可在写入线程调用.
 *   2. 用户通过返回指针写入数据后,可以通过TOfRingBufPush函数来修改实际写入的字节数
 *   3. 如果缓冲区发生转弯,需要分两次调用.第一次调用获取当前写入点到缓冲区末尾的空间,之后调用TOfRingBufPush,修改缓冲区中的字节数
 * @param self: 对象实例
 * @param writeable_size: 返回当前写入点有多少数据可以写入(如果缓冲区发生转弯,则返回值仅到缓冲区末尾); 可以为NULL
 * @return 如果有空间可以写入,则返回写入点的指针,否则返回NULL
 */
char *TOfRingBufGetWritePos(TOfRingBuf *self, Int32 *writeable_size);

/**
 * 修改当前写入点到当前位置+size的地方(内部自动处理转弯)
 * @note 仅写入取线程可以操作该函数
 * @param self: 对象实例
 * @param size: 需要增加的字节数;
 * @return
 */
void TOfRingBufPush(TOfRingBuf *self, Int32 size);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif// _OFC_RING_BUF_H_
