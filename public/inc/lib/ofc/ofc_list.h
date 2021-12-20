/**
 * @file         ofc_list.h
 * @brief        List队列,注意:所有接口内部不判断对象参数的有效性，需要外部自己判断
 * @note         多线程不安全，可重入
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */

#ifndef _OFC_LIST_H_ 
#define _OFC_LIST_H_

#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 比较排序方法
 * @param item1: TOfList容器中的对象
 * @param item2: TOfListQuickFind函数的第二个参数
 * @return >0: item1>item2 0:item1==item2 <0: item1<item2
 */
typedef long (*TOfListSortCompare)(void *item1, void *item2);


typedef struct TOfListTag TOfList;

/**
 * 删除通知事件
 * @param sender: 事件发起者
 * @param ptr: 删除内存指针
 * @return 
 */
typedef void (*TOfListOnDelete)(TOfList *sender, void *ptr);

struct TOfListTag 
{
PROTECTED
    TOfObject parent_;  ///< 基类
    TOfVirtualMethod Destroy_;  ///< 析构事件

PUBLIC
    TOfListOnDelete OnDelete;  ///< 删除事件
    void *receiver;  ///< 接收者

PRIVATE
    Int32 capacity_;  ///< 容量
    void **list_;  ///< 数据
    Int32 count_;  ///< 个数
};

/**
 * TOfList构造,内部强制清零
 * @param self: 对象指针
 * @return 
 */
void TOfListCreate(TOfList *self);

/**
 * 创建一个列表
 * @return 返回创建的列表对象
 */
TOfList *OfCreateList(void);

/**
 * 增加一项到队列末尾
 * @param self: 对象指针
 * @param item: 增加的内容
 * @return 增加到所对应的索引,参数错误和增加失败返回(-1)
 */
Int32 TOfListAdd(TOfList *self, void *item);

/**
 * 获取队列中的一项的数据
 * @param self: 对象指针
 * @param index: 获取的索引
 * @return 返回索引所在的内容,参数错误返回NULL
 */
void *TOfListItems(TOfList *self, Int32 index);

/**
 * 设置某一项的数据,删除原数据时,内部会自动调用_OnDelete事件
 * @param self: 对象指针
 * @param index: 设置的索引
 * @param item: 设置的内容
 * @return 
 */
void TOfListSetItem(TOfList *self, Int32 index, void *item);

/**
 * 清除队列数据,删除时会依次调用_OnDelete事件
 * @param self: 对象指针
 * @return 
 */
void TOfListClear(TOfList *self);

/**
 * 删除某一项数据,内部自动调用OnDelete事件
 * @param self: 对象指针
 * @param index: 删除的索引
 * @return 
 */
void TOfListDelete(TOfList *self, Int32 index);

/**
 * 通过指针获取队列内部是否存在所对应的数据
 * @param self: 对象指针
 * @param item: 数据的地址
 * @return 返回数据所在的索引,参数错误和数据不存在时返回(-1)
 */
Int32 TOfListIndexOf(TOfList *self, void *item);

/**
 * 插入一项数据到队列
 * @param self: 对象指针
 * @param index: 插入的索引
 * @param item: 插入的数据
 * @return 插入是否成功
 */
Bool TOfListInsert(TOfList *self, Int32 index, void *item);

/**
 * 删除某一项数据,内部自动调用OnDelete事件
 * @param self: 对象指针
 * @param item: 删除的数据
 * @return 删除所在的索引，若不存在则返回(-1)
 */
Int32 TOfListRemove(TOfList *self, void *item);

/**
 * 获取队列个数
 * @param self: 对象指针
 * @return 
 */
Int32 TOfListCount(TOfList *self);

/**
 * 设置队列的个数
 * @param self: 对象指针
 * @param new_count: 个数
 * @return 设置是否成功
 */
Bool TOfListSetCount(TOfList *self, Int32 new_count);

/**
 * 通过快速查找方法找到所对应的索引，需要先排序好后再查找
 * @param self: 对象指针
 * @param item: 所要查找的数据
 * @param compare: 查找时所用的方法
 * @param index: 查找的所对应地址
 * @return 查找是否成功,成功时才允许用index的返回值
 */
Bool TOfListQuickFind(TOfList *self, void *item, TOfListSortCompare compare, Int32 *index);

/**
 * 列表排序
 * @note 内部会递归，小内存平台慎用，递归深度跟内部对象个数有关系
 * @param self: 对象指针
 * @param compare: 排序方法
 * @return 
 */
void TOfListSort(TOfList *self, TOfListSortCompare compare);

/**
 * 删除列表中从index开始的count项.
 * 删除的项如果不为NULL会调用OnDelete函数
 * @param self: 对象指针
 * @param index: 要删除的第一项的索引; 如果index<0,则从第一项开始删除.如果index>=Count()则不删除
 * @param count: 要删除的项数.超出范围的项不会被删除
 * @return
 */
void TOfListDeleteEx(TOfList *self, Int32 index, Int32 count);

/**
 * 获取列表中的容量
 * @param self: 对象指针
 * @return 容量大小
 */
Int32 TOfListCapacity(TOfList *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_OFC_LIST_H_
