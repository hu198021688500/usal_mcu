#include "drv_int.h"
#include "stm32f4xx.h"

#define AIRCR_VECTKEY_MASK    ((UInt32)0x05FA0000)


///优先级组配置0 - 0抢占优先级 16个响应优先级
///优先级组配置1 - 2抢占优先级 8个响应优先级
///优先级组配置2 - 4抢占优先级 4个响应优先级
///优先级组配置3 - 8抢占优先级 2个响应优先级
///优先级组配置4 - 16抢占优先级 0个响应优先级
enum
{
    kNvicPriorityGroup0     = 0x700,
    kNvicPriorityGroup1     = 0x600,
    kNvicPriorityGroup2     = 0x500,
    kNvicPriorityGroup3     = 0x400,
    kNvicPriorityGroup4     = 0x300,
};
void SysIRQInit()
{
    /// 设置中断优先级组为1, 2抢占优先级 8个响应优先级
    UInt32 group = kNvicPriorityGroup1;
    SCB->AIRCR = AIRCR_VECTKEY_MASK | group;
}

void SysEnableIRQ(UInt32 irq_number, TSysIrqPreePriority preemption_priority, TSysIrqSubPriority sub_priority)
{
    UInt8 tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = preemption_priority << tmppre;
    tmppriority |=  (UInt8)(sub_priority & tmpsub);
        
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[irq_number] = tmppriority;
    
    NVIC->ISER[(UInt32)(irq_number) >> 5] = 1 << ((UInt32)irq_number & 0x1F);
}

void SysDisableIRQ(UInt32 irq_number)
{
    NVIC->ICER[(UInt32)(irq_number) >> 5] = 1 << ((UInt32)irq_number & 0x1F);

    /// 清除PENDING标志
    NVIC->ICPR[(UInt32)(irq_number) >> 5] = 1 << ((UInt32)irq_number & 0x1F);
}

