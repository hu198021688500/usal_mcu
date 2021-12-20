#include "rtos_api.h"
#include <cmsis_os2.h>
#include "string.h"

void RtosKernelInit(void)
{
    osKernelInitialize();
}

Bool RtosKernelReady(void)
{
    return osKernelGetState() == osKernelReady;
}

Bool RtosKernelStart(void)
{
    return osKernelStart() == osOK;
}

void RtosSleep(UInt32 ms)
{
    osDelay(ms);
}

UInt32 RtosGetTickCount(void)
{
    return osKernelGetTickCount();
}

void RtosKernelLock(void)
{
    osKernelLock();
}

void RtosKernelUnlock(void)
{
    osKernelUnlock();
}


UInt32 RtosKernelSuspend(void)
{
    return osKernelSuspend();
}

void RtosKernelResume(UInt32 ms)
{
    osKernelResume(ms);
}

DRtosEventFlagsHandle RtosEventFlagsCreate(void)
{
    return osEventFlagsNew(NULL);
}

Int32 RtosEventFlagsSet(DRtosEventFlagsHandle handle, UInt32 flags)
{
    return osEventFlagsSet(handle, flags);
}

Int32 RtosEventFlagsWait(DRtosEventFlagsHandle handle, UInt32 flags, TRtosEventFlagsOptions option, UInt32 timeout)
{
    return osEventFlagsWait(handle, flags, option, timeout);
}

Bool RtosEventFlagsDestroy(DRtosEventFlagsHandle handle)
{
    return osEventFlagsDelete(handle) == osOK;
}

DRtosMutexHandle RtosMutexCreate(void)
{
    return osMutexNew(NULL);
}

Bool RtosMutexLock(DRtosMutexHandle handle, UInt32 timeout)
{
    return osMutexAcquire(handle, timeout) == osOK;    
}

Bool RtosMutexUnlock(DRtosMutexHandle handle)
{
    return osMutexRelease(handle) == osOK;    
}

Bool RtosMutexDestory(DRtosMutexHandle handle)
{
    return osMutexDelete(handle) == osOK;    
}
/* 优先级映射（这些优先级开放给应用层使用） */
const osPriority_t g_priority_map_[kRtosThreadPriorityMax] = 
{
    osPriorityAboveNormal4, osPriorityAboveNormal5, osPriorityAboveNormal6, osPriorityAboveNormal7,
    osPriorityHigh,         osPriorityHigh1,        osPriorityHigh2,        osPriorityHigh3,
    osPriorityHigh4,        osPriorityHigh5,        osPriorityHigh6,        osPriorityHigh7,
    osPriorityRealtime,     osPriorityRealtime1,    osPriorityRealtime2,    osPriorityRealtime3, 
};

DRtosThreadHandle RtosThreadCreate(TRtosThreadMethodEntry entry, void *custom_param, TRtosThreadPriority priority   \
    , void *stack, UInt32 stack_size)
{
    osThreadAttr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.attr_bits = osThreadJoinable;
    attr.priority = (osPriority_t)g_priority_map_[priority];
    
    attr.stack_mem = stack;
    attr.stack_size = stack_size;
    
    return osThreadNew(entry, custom_param, &attr);
}

Bool RtosThreadExit(void)
{
    osThreadExit();
    return True;
}

Bool RtosThreadJoin(DRtosThreadHandle handle)
{
    return (osThreadJoin(handle) == osOK);
}

Int32 RtosThreadFlagsSet(DRtosThreadHandle handle, UInt32 flags)
{
    return osThreadFlagsSet(handle, flags);
}


Int32 RtosThreadFlagsWait(Int32 flags, TRtosEventFlagsOptions options, Int32 timeout)
{
    return (Int32)osThreadFlagsWait(flags, options, timeout);  
}


TRtosThreadStatus RtosThreadGeStatus(DRtosThreadHandle handle)
{
    TRtosThreadStatus result;
    
    osThreadState_t state = osThreadGetState(handle);
    if (state < osThreadInactive)
        result = kRtosThreadStatusError;
    else
        result = (TRtosThreadStatus)state;
    
    return result;
}

static int32_t critical_section_ref_count = 0;

/* 进入临界区 */
void RtosEnterCriticalSection(void)
{
    // 关闭全局中断
    __disable_irq();

    // 更新引用计数
    critical_section_ref_count++;
}

/* 退出临界区 */
void RtosLeaveCriticalSection(void)
{
    // 校验和更新引用计数
    if (critical_section_ref_count > 0) 
    {
        critical_section_ref_count--;

        if (critical_section_ref_count <= 0)
        {
            // 打开全局中断
            __enable_irq();
        }
    }
}
