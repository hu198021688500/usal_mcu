#include "kernel_init.h"
#include <ofc_object.h>
#include <kfc_task.h>
#include "test_task.h"
#include "kfc_system.h"

/**
 * 主任务
 */
typedef struct TMainTaskTag TMainTask;
struct TMainTaskTag
{
PROTECTED
    TKfcTask parent_;                ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    TTestTask *test_task_;
};

/// 主任务对象
TMainTask g_main_task;

/**
 * 创建对象
 * @param object: 对象实例
 * @return 
 */
static void TMainTaskOnTaskCreate(TOfObject *object)
{
    TMainTask *self = (TMainTask *)object;
    self->test_task_ = &g_test_task;
    
    TTestTaskCreate(self->test_task_);           ///< 测试任务创建
}

/**
 * 任务开始
 * @param object: 对象实例
 * @return 
 */
static void TMainTaskOnTaskStart(TOfObject *object)
{
    TMainTask *self = (TMainTask *)object;
    
    TKfcTaskStart((TKfcTask *)self->test_task_);   ///< 测试任务启动
}

/**
 * 服务执行函数
 * @param object: 对象实例
 * @return 
 */
static void TMainTaskOnTaskExecute(TOfObject *object)
{
    TMainTask *self = (TMainTask *)object;

    while (!TKfcTaskTerminated((TKfcTask *)self))
    {
        KernelRunOnce();
        KfcSleep(2);
    }
}

/**
 * 析构函数
 * @param object: 对象实例
 * @return 
 */
static void TMainTaskDestroy(TOfObject *object)
{
    TMainTask *self = (TMainTask *)object;
    
    self->Destroy_(object);
}

void TMainTaskCreate(TMainTask *self)
{
#if defined (__GNUC__)
    static char stack[512] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[512];
#endif

    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));
    OVERRIDE_METHOD(TOfObject, Destroy, TMainTaskDestroy);
    self->parent_.OnCreate = TMainTaskOnTaskCreate;
    self->parent_.OnStart = TMainTaskOnTaskStart;
    self->parent_.OnExecute = TMainTaskOnTaskExecute;
    
    TKfcTaskResume(&self->parent_);
}

int main()
{
    /// 内核初始化
    KernelInit();
    /// 主任务创建和启动
    TMainTaskCreate(&g_main_task);
    TKfcTaskStart((TKfcTask *)&g_main_task);
    
    KernelStart();
    /// TODO:不应该走到这，需要加异常处理
    while(1);
}
