#include "thread_event.h"

static void TThreadEventDestroy(TOfObject *sender)
{
    TThreadEvent *self = (TThreadEvent *)sender;
    self->Destroy_(sender);
}

void TSfThreadEventCreate( TThreadEvent *self, TThread *thread )
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TThreadEventDestroy);

    self->handle_ = TThreadHandle(thread);
}

Bool TThreadEventSet(TThreadEvent *self, Int32 flag)
{
    Int32 flags = 1 << flag;
    Int32 r = RtosThreadFlagsSet(self->handle_, flags);

    return r >= 0 ? (r & flags) == 0 : False;
}

Bool TThreadEventWait(TThreadEvent *self, Int32 flags, Bool all, Int32 timeout)
{
    return (Int32)RtosThreadFlagsWait(flags, all ? kRtosEventFlagsOptionsWaitAll : kRtosEventFlagsOptionsWaitAny, timeout) > 0;    
}

