#include "thread.h"
#include <ofc_string.h>

/**
 * 线程的函数体
 * @param param: 对象实例指针
 * @return 
 */
static void ThreadProc(void *param)
{
    TThread *self = (TThread *)param;

    if (self->OnExecute != NULL && self->receiver != NULL)
        self->OnExecute(self->receiver);
    else
    {
        while (!self->terminated_)
        {
            if (self->OnRunOnce != NULL && self->receiver != NULL)
            {
                Int32 ms = self->OnRunOnce(self->receiver);

                if (ms > 0)
                    RtosSleep(ms);
            }
            else
                RtosSleep(50);
        }
    }
    RtosThreadExit();
}

static void TThreadDestroy(TOfObject *sender)
{
    TThread *self = (TThread *)sender;

    if (self->handle_ != NULL)
    {
        TThreadTerminate(self);
        TThreadWaitFor(self, 5000);
    }

    self->Destroy_(sender);
}

static void TThreadCreateThread(TThread *self)
{
    self->handle_ = RtosThreadCreate(ThreadProc, (void *)self, self->priority_, self->stack_, self->stack_size_);
}

void TThreadCreate(TThread *self, TRtosThreadPriority priority, void *stack, Int32 stack_size)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TThreadDestroy);

    self->receiver = NULL;
    self->OnExecute = NULL;
    self->OnRunOnce = NULL;
    self->handle_ = NULL;
    self->terminated_ = False;
    self->priority_ = priority;
    self->stack_ = stack;
    self->stack_size_ = stack_size;
}

Bool TThreadResume(TThread *self)
{
    if (self->handle_ == NULL)
    {
        self->terminated_ = False;  // 这样可以重复Resume(需要先调用TThreadWaitFor,把句柄设置为NULL)
        TThreadCreateThread(self);
    }

    return self->handle_ != NULL;
}

void TThreadTerminate(TThread *self)
{
    self->terminated_ = True;
}

Bool TThreadTerminated(TThread *self)
{
    return self->terminated_;
}

Bool TThreadWaitFor(TThread *self, UInt32 timeout)
{
    if (self->handle_ != NULL)
    {
        if (TThreadRunning(self))
            RtosThreadJoin(self->handle_);

        self->handle_ = NULL;
    }
	
	return True;
}

DRtosThreadHandle TThreadHandle(TThread *self)
{
    return self->handle_;
}

Bool TThreadRunning(TThread *self)
{
    Bool result;

    if (self->handle_ != NULL)
    {
        TRtosThreadStatus state = RtosThreadGeStatus(self->handle_);
        result = (state == kRtosThreadStatusReady || state == kRtosThreadStatusBlocked || state == kRtosThreadStatusRunning);
    }
    else
        result = False;

    return result;
}

long TThreadThreadId(TThread *self)
{
    return (long)self->handle_;
}

TThread * CreateThread(TRtosThreadPriority priority, void *stack, Int32 stack_size)
{
    TThread *result = (TThread *)OfCreateObject(sizeof(*result));

    if (result != NULL)
        TThreadCreate(result, priority, stack, stack_size);

    return result;
}
