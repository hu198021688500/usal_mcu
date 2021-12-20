#include <kfc_mutex.h>
#include "rtos_api.h"

static void TKfcMutexDestroy(TOfObject *sender)
{
    TKfcMutex *self = (TKfcMutex *)sender;
    RtosMutexDestory((DRtosMutexHandle)self->handle_);
    self->Destroy_(sender);
}

Bool TKfcMutexCreate(TKfcMutex *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TKfcMutexDestroy);

    self->handle_ = RtosMutexCreate();

    return self->handle_ != NULL;
}

Bool TKfcMutexLock(TKfcMutex *self, Int32 timeout)
{
    return RtosMutexLock((DRtosMutexHandle)self->handle_, timeout);    
}

void TKfcMutexUnlock(TKfcMutex *self)
{
    RtosMutexUnlock((DRtosMutexHandle)self->handle_);
}

