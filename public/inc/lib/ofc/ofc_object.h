/**
 * @file ofc_object.h
 * @brief 基础类定义
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
#ifndef _OFC_OBJECT_H_
#define _OFC_OBJECT_H_

#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASSIGN_METHOD(name, method) \
    self->name = method;

#define OVERRIDE_METHOD(ancestor_class, name, method)\
    self->name##_ = ((ancestor_class *)self)->name;\
    ((ancestor_class *)self)->name = method;

typedef struct TOfObjectTag TOfObject;
typedef void (*TOfVirtualMethod)(TOfObject *self);

/**
 * TOfNotifyEvent事件
 * @param sender: 调用者对象指针
 * @return 
 */
typedef void (*TOfNotifyEvent)(TOfObject *sender);

/**
 * TOfDoNotify事件
 * @param sender: 调用者对象指针
 * @return 
 */
typedef void (*TOfDoNotify)(void *sender);

/**
 * TOfOnNotify事件
 */
typedef struct TOfOnNotifyTag TOfOnNotify;
struct TOfOnNotifyTag
{
    void *object;
    TOfDoNotify Method;
};

#define PRIVATE
#define PROTECTED
#define PUBLIC

#define VIRTUAL

/**
 * Ofc内存分配函数
 * @param size: 分配内存的大小
 * @return 内存分配成功,返回分配的内存指针,否则返回NULL
 */
void *OfMalloc(Int32 size);

/**
 * Ofc内存释放函数
 * @param ptr: 释放内存的指针
 * @return 
 */
void OfFree(void *ptr);

/**
 * 所有对象的基类
 */
struct TOfObjectTag
{
PUBLIC
    TOfVirtualMethod Destroy;
};

/**
 * TOfObject的构造函数
 * @param self: 对象实例指针
 * @return
 */
void TOfObjectCreate(TOfObject *self);

/**
 * 所有的对象销毁,需要调用该函数.
 * @note 该函数只负责销毁对象,而不会释放self的内存.如果需要销毁并释放内存的话用FreeObject函数
 * 如果self不是malloc出来的可以调用这个函数
 * @param self: 对象实例指针
 * @return
 */
void OfDestroyObject(TOfObject *self);

/**
 * 创建对象实例的函数.该函数仅仅是分配了对象占用的内存.
 * @param object_size: 对象实例的大小
 * @return 返回创建的对象(注: 该对象为调用任何构造函数)
 */
TOfObject *OfCreateObject(Int32 object_size);

/**
 * 调用对象的析构函数,并释放self指向的内存.
 * 仅动态分配出来的对象用这个函数,否则用DestroyObject函数
 * @param self: 对象实例指针
 * @return
 */
void OfFreeObject(TOfObject *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _OFC_OBJECT_H_
