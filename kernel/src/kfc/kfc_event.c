#include "kfc_event.h"
#include "rtos_api.h"


static void TSfEventDestroy(TOfObject *sender)
{
    TKfcEvent *self = (TKfcEvent *)sender;
    RtosEventFlagsDestroy((DRtosEventFlagsHandle)self->handle_);
    self->Destroy_(sender);
}

void TKfcEventCreate(TKfcEvent *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TSfEventDestroy);

    self->handle_ = RtosEventFlagsCreate();
}

Bool TKfcEventSetFlag(TKfcEvent *self, Int32 flag_bit_index )
{
    Int32 flags = 1 << flag_bit_index;
    Int32 r = RtosEventFlagsSet((DRtosEventFlagsHandle)self->handle_, flags);

    return r >= 0 ? (r & flags) == 0 : False;
}

Bool TKfcEventSetFlags(TKfcEvent *self, UInt32 flags)
{
    Int32 r = RtosEventFlagsSet((DRtosEventFlagsHandle)self->handle_, flags);

    return r >= 0 ? ((UInt32)r & flags) == 0 : False;
}

Int32 TKfcEventWait(TKfcEvent *self, Int32 flags, Bool all, Int32 timeout)
{
    return (Int32)RtosEventFlagsWait((DRtosEventFlagsHandle)self->handle_, flags, all ? kRtosEventFlagsOptionsWaitAll : kRtosEventFlagsOptionsWaitAny, timeout);    
}

