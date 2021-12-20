#include "kfc_memory.h"
void *KfcMalloc(Int32 size)
{
    return OfMalloc(size);
}

void KfcFree(void *ptr)
{
    OfFree(ptr);
}


