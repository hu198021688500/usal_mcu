/**
 * @file drv_clock.h
 * @brief 时钟驱动层
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
#ifndef _DRV_CLOCK_H_
#define _DRV_CLOCK_H_

#include <ofc_object.h>
//#include <stm32f4xx.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvClockFrequencyTag TDrvClockFrequency;
struct TDrvClockFrequencyTag
{
    UInt32 sysclk_frequency;
    UInt32 hclk_frequency;
    UInt32 pclk1_frequency;
    UInt32 pclk2_frequency;
};

    
typedef struct TDrvClockTag TDrvClock;
struct TDrvClockTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;
    
    TDrvClockFrequency  clock_frequency_;   ///< 当前时钟频率
};

extern TDrvClock g_drv_clock;

/**
 * 创建一个时钟驱动
 * @param self: 对象实例指针
 * @return
 */
void TDrvClockCreate(TDrvClock *self);

/**
 * 设置外部时钟
 * @param self: 对象实例指针
 * @return
 */
void TDrvClockInitClock(TDrvClock *self);

/**
 * 获取时钟频率
 * @param self: 对象实例指针
 * @param clock_frequency: 时钟值
 * @return True:成功 False:失败
 */
Bool TDrvClockGetFrequency(TDrvClock *self, TDrvClockFrequency *clock_frequency);
/**
 * 延时微秒(不休眠)
 * @param self: 对象实例指针
 * @param us: 微秒
 * @return 无
 */
void TDrvClockDelayUs(TDrvClock *self, UInt32 us);
/**
 * 延时毫秒(不休眠)
 * @param self: 对象实例指针
 * @param us: 毫秒
 * @return 无
 */
void TDrvClockDelayMs(TDrvClock *self, UInt32 ms);

/// 给初始化库调用
extern void SystemInit(void);
/// 系统时钟值,给rtos使用
extern UInt32 SystemCoreClock; 

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_CLOCK_H_
