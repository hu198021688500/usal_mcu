#include "pfc_sync_objs.h"

#if defined (__GNUC__)
Int32 PfInterlockedCompareExchange(Int32 *dst, Int32 exchange, Int32 comparand) 
{
    Int32 result;

    __asm__ __volatile__ (
        "push     {r4}\n\t"
        "1:\n\t"
        "ldrex    r3, [r0]\n\t"       // r3 = *dst, 并设置dst为内存监视
        "cmp      r3, r2\n\t"         // *dst == comparand?   
        "movne    r4, #0\n\t"         // != ? r4 = 0
        "strexeq  r4, r1, [r0]\n\t"   // == ? r4 = dst内存监视 ? 0, *dst = exchange : 1
        "cmp      r4, #0\n\t"         // 不相等或者赋值成功,退出
        "bne      1b\n\t"
        "pop      {r4}\n\t"
        "mov      %[rst], r3"         // 返回原先的*dst
        :[rst]"=r"(result)
    );
    return result;
}

void PfInterlockedAdd( Int32 *dst, Int32 value )
{
    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r2, [r0]\n\t"
        "add     r2, r1\n\t"
        "strex   r3, r2, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r3, #0\n\t"
        "bne     1b"
    );
}

void PfInterlockedSub( Int32 *dst, Int32 value )
{
    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r2, [r0]\n\t"
        "sub     r2, r1\n\t"
        "strex   r3, r2, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r3, #0\n\t"
        "bne     1b"
    );
}

Int32 PfInterlockedInc( Int32 *dst )
{
    Int32 result;

    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r1, [r0]\n\t"
        "add     r1, #1\n\t"
        "strex   r2, r1, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r2, #0\n\t"
        "bne     1b\n\t"
        "mov     %[rst], r1"          // 返回本次调用实际存入*dst的值
        :[rst]"=r"(result)
    );

    return result;
}

Int32 PfInterlockedDec( Int32 *dst )
{
    Int32 result;

    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r1, [r0]\n\t"
        "sub     r1, #1\n\t"
        "strex   r2, r1, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r2, #0\n\t"
        "bne     1b\n\t"
        "mov     %[rst], r1"          // 返回本次调用实际存入*dst的值
        :[rst]"=r"(result)
    );

    return result;
}

static void Lock(volatile Int32 *data)
{
    __asm__ __volatile__ (
        "mov      r1, #1\n\t"             // 设置初始值
        "1:\n\t"
        "ldrex    r2, [r0]\n\t"           // r2 = *data; // 取出data的值，并设置data指向的地址为监视区域
        "cmp      r2, #0\n\t"             // 如果*data不为0,表示已经锁定
        "bne      1b\n\t"                // *data不为0,重新取值,相当于等待
        "strex    r2, r1, [r0]\n\t"       // 如果data指向的地址处于监视状态,则*data=R1(=1),并设置R2为0,取消data指向地址的监视状态,否则仅设置R2为1
        "cmp      r2, #0\n\t"             // 如果赋值成功则返回,否则重新等待 
        "bne      1b"
    );
}

void PfInterlockedOr( Int32 *dst, Int32 value )
{
    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r2, [r0]\n\t"
        "orr     r2, r1\n\t"
        "strex   r3, r2, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r3, #0\n\t"
        "bne     1b"
    );
}

void PfInterlockedAnd( Int32 *dst, Int32 value )
{
    __asm__ __volatile__ (
        "1:\n\t"
        "ldrex   r2, [r0]\n\t"
        "and     r2, r1\n\t"
        "strex   r3, r2, [r0]\n\t"    // 如果写入失败,表示被中断中改掉,重新修改值
        "cmp     r3, #0\n\t"
        "bne     1b"
    );
}
#else
__asm Int32 PfInterlockedCompareExchange(Int32 *dst, Int32 exchange, Int32 comparand) 
{
    push     {r4}
1
    ldrex    r3, [r0]       // r3 = *dst, 并设置dst为内存监视    
    cmp      r3, r2         // *dst == comparand?   
    movne    r4, #0         // != ? r4 = 0
    strexeq  r4, r1, [r0]   // == ? r4 = dst内存监视 ? 0, *dst = exchange : 1
    cmp      r4, #0         // 不相等或者赋值成功,退出
    bne      %B1                
    
    pop      {r4}
    mov      r0, r3         // 返回原先的*dst
    bx       lr                 
}

__asm void PfInterlockedAdd( Int32 *dst, Int32 value )
{
1
    ldrex   r2, [r0]
    add     r2, r1
    strex   r3, r2, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r3, #0
    bne     %B1
    bx      lr                 
}
__asm void PfInterlockedSub( Int32 *dst, Int32 value )
{
1
    ldrex   r2, [r0]
    sub     r2, r1
    strex   r3, r2, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r3, #0
    bne     %B1
    bx      lr                 
}

__asm Int32 PfInterlockedInc( Int32 *dst )
{
1
    ldrex   r1, [r0]
    add     r1, #1
    strex   r2, r1, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r2, #0
    bne     %B1
    mov     r0, r1          // 返回本次调用实际存入*dst的值
    bx      lr                 
}

__asm Int32 PfInterlockedDec( Int32 *dst )
{
1
    ldrex   r1, [r0]
    sub     r1, #1
    strex   r2, r1, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r2, #0
    bne     %B1
    mov     r0, r1          // 返回本次调用实际存入*dst的值
    bx      lr                 
}

__asm static void Lock(volatile Int32 *data)
{
    mov      r1, #1             // 设置初始值
1
    ldrex    r2, [r0]           // r2 = *data; // 取出data的值，并设置data指向的地址为监视区域
    cmp      r2, #0             // 如果*data不为0,表示已经锁定
    bne      %B1                // *data不为0,重新取值,相当于等待
    strex    r2, r1, [r0]       // 如果data指向的地址处于监视状态,则*data=R1(=1),并设置R2为0,取消data指向地址的监视状态,否则仅设置R2为1
    cmp      r2, #0             // 如果赋值成功则返回,否则重新等待 
    bne      %B1                // 
    bx       lr                 // 返回
}

__asm void PfInterlockedOr( Int32 *dst, Int32 value )
{
1
    ldrex   r2, [r0]
    orr     r2, r1
    strex   r3, r2, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r3, #0
    bne     %B1
    bx      lr                 
}

__asm void PfInterlockedAnd( Int32 *dst, Int32 value )
{
1
    ldrex   r2, [r0]
    and     r2, r1
    strex   r3, r2, [r0]    // 如果写入失败,表示被中断中改掉,重新修改值
    cmp     r3, #0
    bne     %B1
    bx      lr                 
}
#endif

static void TPfSpinLockDestroy(TOfObject *self)
{
    ((TPfSpinLock *)self)->Destroy_(self);
}

void TPfSpinLockCreate( TPfSpinLock *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TPfSpinLockDestroy);
    
    self->value_ = 0;
}

void TPfSpinLockEnter( TPfSpinLock *self )
{
    Lock((volatile Int32 *)&self->value_);
}

void TPfSpinLockLeave( TPfSpinLock *self )
{
    self->value_ = 0;
}

TPfSpinLock * PfCreateSpinLock( void )
{
    TPfSpinLock *result = (TPfSpinLock *)OfCreateObject(sizeof(*result));
    TPfSpinLockCreate(result);

    return result;
}

void PfSpinLock( Int32 *value )
{
    Lock((volatile Int32 *)value);
}

void PfSpinUnlock( Int32 *value )
{
    *value = 0;
}
