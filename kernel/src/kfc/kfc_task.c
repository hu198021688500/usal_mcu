#include "kfc_task.h"
#include "thread.h"
#include "string.h"

static Int32 TaskExecute(void *sender)
{
    TKfcTask *task = (TKfcTask *)sender;

    if (task->OnCreate != NULL)
        task->OnCreate((TOfObject *)task);

    while (!TThreadTerminated(task->thread_))
    {
        if (task->state_ == kKfcTaskStateStart)
        {
            if (task->OnStart != NULL)
                task->OnStart((TOfObject *)task);

            task->state_ = kKfcTaskStateRunning;

            if (task->OnExecute != NULL)
                task->OnExecute((TOfObject *)task);

            if (task->OnStop != NULL)
                task->OnStop((TOfObject *)task);

            task->state_ = kKfcTaskStateStopped;
        }

        if (!TThreadTerminated(task->thread_))
            RtosSleep(50);
    }
    return 0;
}

static void TKfcTaskDestroy(TOfObject *sender)
{
    TKfcTask *self = (TKfcTask *)sender;

    if (TKfcTaskRunning(self))
        TKfcTaskStop(self);
    if (self->thread_ != NULL)
    {
        TThreadTerminate(self->thread_);
        TThreadWaitFor(self->thread_, 0);
        
        OfFreeObject((TOfObject *)self->thread_);
    }
    self->Destroy_(sender);
}

void TKfcTaskCreate(TKfcTask *self, TKfcTaskPriority priority, void *stack, Int32 stack_size)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TKfcTaskDestroy);
    /// 堆栈检查设置
    memset(stack, 0, stack_size);
    self->stack_ = stack;
    self->stack_size_ = stack_size;
    self->thread_ = (void *)CreateThread((TRtosThreadPriority)priority, stack, stack_size);
    self->OnExecute = NULL;
    self->state_ = kKfcTaskStateStopped;
    if (self->thread_ != NULL)
    {
        ((TThread *)(self->thread_))->receiver = (void *)self;
        ((TThread *)(self->thread_))->OnExecute = TaskExecute;
    }
}

Bool TKfcTaskResume(TKfcTask *self)
{
    if (self->thread_ != NULL)
        return TThreadResume(self->thread_);
    else
        return False;
}

void TKfcTaskStart(TKfcTask *self)
{
    if (self->state_ == kKfcTaskStateStopped)
        self->state_ = kKfcTaskStateStart;
}

void TKfcTaskStop(TKfcTask *self)
{
    if (self->state_ == kKfcTaskStateRunning)
        self->state_ = kKfcTaskStateStop;
}

Bool TKfcTaskRunning(TKfcTask *self)
{
    return self->state_ == kKfcTaskStateRunning;
}

Bool TKfcTaskStopped(TKfcTask *self)
{
    return self->state_ == kKfcTaskStateStopped;
}

Bool TKfcTaskTerminated(TKfcTask *self)
{
    return self->state_ == kKfcTaskStateStop;
}

Bool TKfcTaskWaitFor(TKfcTask *self, Int32 timeout)
{
    while (self->state_ != kKfcTaskStateStopped && timeout > 0)
    {
        timeout--;
        RtosSleep(1);
    }
    
    return self->state_ == kKfcTaskStateStopped;
}


Bool TKfcTaskStackCheck(TKfcTask *self, Int32 *free, Int32 *used)
{
    if (self->stack_ != NULL && free != NULL && used != NULL)
    {
        Int32 i;
        UInt8 *p = (UInt8 *)self->stack_;
        for (i = 4; i < self->stack_size_; i++)
        {
            if (p[i] != 0)
                break;
        }
        *free = i - 4;
        *used = self->stack_size_ - *free;
        
        return True;
    }
    else
        return False;
}
