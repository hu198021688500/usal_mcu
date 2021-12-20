#include "memory.h"
#include "cmsis_compiler.h"

TMemMemory g_memory;  ///< 内存单例模式对象

/**
 * ofc_object.c中需要用到
 * @param
 * @return
 */
void *OfMalloc(Int32 size)
{
    return Malloc(size);
}

/**
 * ofc_object.c中需要用到
 * @param
 * @return
 */
void OfFree(void *ptr)
{
    Free(ptr);
}

/**
 * 内存管理的全局加锁
 * @note 静态方法
 * @param object: 对象实例
 * @return 
 */
static void TMemoryLock(void)
{
   /// 关闭总中断
    __disable_irq();
}

/**
 * 内存管理的全局解锁
 * @note 静态方法
 * @param object: 对象实例
 * @return 
 */
static void TMemoryUnlock(void)
{
   /// 开启总中断
    __enable_irq();
}

/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TMemMemoryDestroy(TOfObject *object)
{
    TMemMemory *self = (TMemMemory *)object;

    ///< 释放内存管理
    KYMemory_Fini();

    self->Destroy_(object);
}

void TMemMemoryCreate(TMemMemory *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TMemMemoryDestroy);

    /// 初始化内存管理
    self->block_.Items = self->items_;
    self->block_.Indexs = self->indexs_;
    self->block_.Count = sizeof(self->indexs_) / sizeof(self->indexs_[0]);
    KYMemory_Init(TMemoryLock, TMemoryUnlock, NULL, NULL, &self->block_, sizeof(self->block_)/sizeof(self->block_));
}
