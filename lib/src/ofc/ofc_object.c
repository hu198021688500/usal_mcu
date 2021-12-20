#include "ofc_object.h"

static void TOfObjectDestroy(TOfObject *self)
{

}

void TOfObjectCreate( TOfObject *self )
{
    ASSIGN_METHOD(Destroy, TOfObjectDestroy);
}

void OfDestroyObject( TOfObject *self )
{
    if (self != NULL && self->Destroy != NULL)
        self->Destroy(self);
}

void OfFreeObject( TOfObject *self )
{
    if (self != NULL)
    {
        OfDestroyObject(self);
        OfFree(self);
    }
}

TOfObject * OfCreateObject( Int32 object_size )
{
    return (TOfObject *)OfMalloc(object_size);
}
