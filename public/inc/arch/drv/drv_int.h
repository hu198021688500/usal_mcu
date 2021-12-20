/**
 * @file drv_int.h
 * @brief 中断相关的支持
 * @note 
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */
#ifndef _DRV_INT_H_
#define _DRV_INT_H_

#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// 抢占优先级
typedef enum TSysIrqPreePriorityTag TSysIrqPreePriority;
enum TSysIrqPreePriorityTag
{
    kSysIrqPreePriority0    = 0,     ///< 抢占优先级0
    kSysIrqPreePriority1,           ///< 抢占优先级1
};

/// 响应优先级
typedef enum TSysIrqSubPriorityTag TSysIrqSubPriority;
enum TSysIrqSubPriorityTag
{
    kSysIrqSubPriority0     = 0,   ///< 响应优先级0
    kSysIrqSubPriority1,           ///< 响应优先级1
    kSysIrqSubPriority2,           ///< 响应优先级2
    kSysIrqSubPriority3,           ///< 响应优先级3
    kSysIrqSubPriority4,           ///< 响应优先级4
    kSysIrqSubPriority5,           ///< 响应优先级5
    kSysIrqSubPriority6,           ///< 响应优先级6
    kSysIrqSubPriority7,           ///< 响应优先级7
};
/**
 * 中断初始化
 * @param 
 * @return
 */
void SysIRQInit(void);
/**
 * 使能中断
 * @param irq_number: 中断号
 * @param preemption_priority: 抢断优先级(0~1) 数字越低优先级越高
 * @param sub_priority: 响应优先级(0~7) 数字越低优先级越高
 * @return
 */
void SysEnableIRQ(UInt32 irq_number, TSysIrqPreePriority preemption_priority, TSysIrqSubPriority sub_priority);

/**
 * 禁用中断(irq_number)
 * @param irq_number: 中断号
 * @return
 */
void SysDisableIRQ(UInt32 irq_number);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_INT_H_
