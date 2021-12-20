/**
 * @file pfc_irq_buf.h
 * @brief 中断和应用之间的数据缓冲区
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
#ifndef _PFC_IRQ_BUF_H_
#define _PFC_IRQ_BUF_H_

#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 构造一个中断中写入,非中断中读取的缓冲区对象
 * @par 示例
 * @code
   char buf[1024];
   TPfIrqWriteBuf wbuf;
   TPfIrqWriteBufCreate(&wbuf, buf, sizeof(buf));
   // ...
   DestroyObject((TOfObject *)&wbuf);

 irq:
   Int32 size;
   char *buf = TPfIrqWriteBufWriteBegin(&wbuf, &size, 0);
   // write data to buf
   TPfIrqWriteBufWriteEnd(&wbuf, write_size);

 thread:
   if (TPfIrqWriteBufDirty(&wbuf))
   {
        Int32 size;
        char *buf = TPfIrqWriteBufReadBegin(&wbuf, &size);
        // process data in buf
        TPfIrqWriteBufReadEnd(&wbuf, read_size);
    }
 * @endcode
*/

typedef struct TPfIrqWriteBufTag TPfIrqWriteBuf;
struct TPfIrqWriteBufTag
{
PRIVATE
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

    char *buf_;
    Int32 buf_size_;
    Int32 read_pos_;
    Int32 write_pos_;
    Int32 clean_pos_;
    Bool reading_;
};

/**
 * 缓冲区在中断中写入的类的构造函数
 * @param self: 对象实例指针
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
void TPfIrqWriteBufCreate(TPfIrqWriteBuf *self, void *buf, Int32 buf_size);

/**
 * 创建缓冲区对象
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
TPfIrqWriteBuf *PfCreateIrqWriteBuf(void *buf, Int32 buf_size);

/**
 * 在中断中获取写入缓冲区地址和空闲缓冲区大小,数据写入后需要调用TPfIrqWriteBufWriteEnd告知实际写入的数据数
 * 注1: 仅在中断中写入,写入后必须在同一次中断中调用TPfIrqWriteBufWriteEnd
 * 注2: 调用该函数时,如果已经有数据被读取>=pack_size,会移动未读取的数据到缓冲区头部,即所谓的"压缩"
 * 注3: 调用后必须掉TPfIrqWriteBufWriteEnd
 * @param self: 对象实例
 * @param free_size: 返回空闲缓冲区的大小,不可以为NULL,如果没有空闲缓冲区返回0.
 * @param pack_size: 浪费空间>=这个值就执行压缩; <=0不压缩
 * @return 返回写入缓冲区指针地址
 */
void *TPfIrqWriteBufWriteBegin(TPfIrqWriteBuf *self, Int32 *free_size, Int32 pack_size);

/**
 * 告知往缓冲区中写入了多少数据
 * 注: 仅在中断中使用
 * @param self: 对象实例
 * @param data_size: 实际写入缓冲区的数据数.(注: 不校验数据长度的有效性)
 * @return
 */
void TPfIrqWriteBufWriteEnd(TPfIrqWriteBuf *self, Int32 data_size);

/**
 * 获取读取缓冲区地址和数据大小
 * 注1: 仅用于单线程操作
 * 注2: 调用前尽可能先调用TPfIrqWriteBufDirty判断是否有新数据写入
 * 注3: 调用后必须掉TPfIrqWriteBufReadEnd
 * @param self: 对象实例
 * @param data_size: 返回可读取的数据数
 * @return 返回读取指针
 */
void *TPfIrqWriteBufReadBegin(TPfIrqWriteBuf *self, Int32 *data_size);

/**
 * 设置已经读取的数据数量
 * @param self: 对象实例
 * @param data_size: 实际读出的数据数(注: 不校验数据长度的有效性)
 * @return
 */
void TPfIrqWriteBufReadEnd(TPfIrqWriteBuf *self, Int32 data_size);

/**
 * 最后一次调用TPfIrqWriteBufReadBegin后有没有新的数据写入
 * @param self: 对象实例
 * @return 有新数据写入,返回True,否则返回False
 */
Bool TPfIrqWriteBufDirty(TPfIrqWriteBuf *self);

/**
 * 清除所有写入数据,并复位
 * 注: 调用该函数的过程中如果中断中调用TPfIrqWriteBufWriteBegin或者TPfIrqWriteBufWriteEnd有肯定会导致意想不到的结果
 * @param self: 对象实例
 * @return
 */
void TPfIrqWriteBufClear(TPfIrqWriteBuf *self);

/**
 * 腾出缓冲区空间用于写入.
 * 注: 仅在中断中调用
 * @param self: 对象实例
 * @param room_size: 需要腾出空间的大小
 * @return 返回实际腾出空间的大小(仅在缓冲区的所有空间小于room_size时返回的值才会不等于room_size)
 */
Int32 TPfIrqWriteBufMakeRoom(TPfIrqWriteBuf *self, Int32 room_size);

/**
 * 返回缓冲区的大小
 * @param self: 对象实例
 * @return 返回实际的缓冲区大小
 */
Int32 TPfIrqWriteBufSize(TPfIrqWriteBuf *self);

/**
 * 返回当前缓冲区是否有数据可以读取
 * @param self: 对象实例
 * @return 有数据可以读取返回True,没有数据读取返回False
 */
Bool TPfIrqWriteBufReadable(TPfIrqWriteBuf *self);

/**
 * 构造一个中断中读出,非中断中写入的缓冲区对象
 * @par 示例
 * @code
   char buf[1024];
   TPfIrqReadBuf wbuf;
   TPfIrqReadBufCreate(&wbuf, buf, sizeof(buf));
   // ...
   DestroyObject((TOfObject *)&wbuf);

 thread:
   Int32 size;
   char *buf = TPfIrqReadBufWriteBegin(&wbuf, &size);
   // write data to buf
   TPfIrqReadBufWriteEnd(&wbuf, write_size);

 irq:
    Int32 size;
    char *buf = TPfIrqReadBufReadBegin(&wbuf, &size);
    // process data in buf
    TPfIrqReadBufReadEnd(&wbuf, read_size, 0); // 只要有数据读出,就压缩空间
 * @endcode
*/
typedef struct TPfIrqReadBufTag TPfIrqReadBuf;
struct TPfIrqReadBufTag
{
PRIVATE
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

    char *buf_;
    Int32 buf_size_;
    Int32 read_pos_;
    Int32 write_pos_;
    Bool writing_;
};

/**
 * 缓冲区在中断中读取的类的构造函数
 * @param self: 对象实例指针
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
void TPfIrqReadBufCreate(TPfIrqReadBuf *self, void *buf, Int32 buf_size);

/**
 * 创建缓冲区对象
 * @param buf: 用于读写的缓冲区
 * @param buf_size: 缓冲区大小
 * @return
 */
TPfIrqReadBuf *PfCreateIrqReadBuf(void *buf, Int32 buf_size);

/**
 * 获取写入缓冲区地址和空闲缓冲区大小,数据写入后需要调用TPfIrqReadBufWriteEnd告知实际写入的数据数
 * 注1: 仅用于单线程操作
 * 注2: 调用后必须掉TPfIrqReadBufWriteEnd
 * @param self: 对象实例
 * @param free_size: 返回空闲缓冲区的大小,不可以为NULL,如果没有空闲缓冲区返回0.
 * @return 返回写入缓冲区指针地址
 */
void *TPfIrqReadBufWriteBegin(TPfIrqReadBuf *self, Int32 *free_size);

/**
 * 告知往缓冲区中写入了多少数据
 * @param self: 对象实例
 * @param data_size: 实际写入缓冲区的数据数.(注: 不校验数据长度的有效性)
 * @return
 */
void TPfIrqReadBufWriteEnd(TPfIrqReadBuf *self, Int32 data_size);

/**
 * 中断中获取读取缓冲区地址和数据大小
 * 注1: 仅在中断中读取,读取后必须在同一次中断中调用TPfIrqReadBufReadEnd
 * 注2: 调用后必须掉TPfIrqReadBufReadEnd
 * @param self: 对象实例
 * @param data_size: 返回可读取的数据数
 * @return 返回读取指针
 */
void *TPfIrqReadBufReadBegin(TPfIrqReadBuf *self, Int32 *data_size);

/**
 * 设置已经读取的数据数量
 * 注1: 仅在中断中使用
 * 注2: 如果本次操作数据被读空,会重置read_pos_和write_pos_
 *      如果没有读空,且没有在写,且浪费空间超过pack_size时,会把已经读取位置的缓冲区压缩掉,即:未读取数据前移
 * @param self: 对象实例
 * @param data_size: 实际读出的数据数(注: 不校验数据长度的有效性)
 * @param pack_size: 浪费空间超过这个值就执行压缩
 * @return
 */
void TPfIrqReadBufReadEnd(TPfIrqReadBuf *self, Int32 data_size, Int32 pack_size);

/**
 * 清除所有写入数据,并复位
 * 注: 调用该函数的过程中如果中断中调用TPfIrqReadBufReadBegin或者TPfIrqReadBufReadEnd有肯定会导致意想不到的结果
 * @param self: 对象实例
 * @return
 */
void TPfIrqReadBufClear(TPfIrqReadBuf *self);

/**
 * 返回缓冲区的大小
 * @param self: 对象实例
 * @return 返回缓冲区大小
 */
Int32 TPfIrqReadBufSize(TPfIrqReadBuf *self);

/**
 * 返回当前缓冲区是否可以写入
 * @param self: 对象实例
 * @return 有空间可以写入返回True,否则返回False
 */
Bool TPfIrqReadBufWriteable(TPfIrqReadBuf *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif// _PFC_IRQ_BUF_H_
