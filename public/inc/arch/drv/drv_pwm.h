/**
 * @file drv_pwm.h
 * @brief PWM输出
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
#ifndef _DRV_PWM_H_
#define _DRV_PWM_H_

#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct TDrvPwmTag TDrvPwm;


/**
 * 中断事件函数
 * @note: 由中断中调用
 * @param sender: 事件的产生者
 * @return
 */
typedef void (*TDrvPwmIrq)(void *sender);

struct TDrvPwmTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PUBLIC
    void            *recevier;      ///< 事件接收者
    TDrvPwmIrq       MethodOnIrq;   ///< 中断事件;
    
PRIVATE
    
    Bool             is_open_;      ///< 是否已经打开
    UInt8            cfg_index_;    ///< 配置序列
    Int32            pwm_timer_index_;  ///< pwm使用定时器结构体序列
};

/**
 * 构造函数
 * @param self: 对象实例指针
 * @param index: 项索引: 0..2
 * @return True:成功 False:失败
 */
Bool TDrvPwmCreate(TDrvPwm *self, UInt8 index);

/**
 * 驱动打开
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvPwmOpen(TDrvPwm *self);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvPwmClose(TDrvPwm *self);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @param value: 占空比值(高电平有效时间比)(0~100%)
 * @return True:成功 False:失败
 */
Bool TDrvPwmSetVaule(TDrvPwm *self, UInt32 value);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_PWM_H_
