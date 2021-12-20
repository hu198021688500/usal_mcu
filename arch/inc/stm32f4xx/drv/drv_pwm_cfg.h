/**
 * @file drv_pwm_cfg.h
 * @brief pwm配置 
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

#ifndef _DRV_PWM_CFG_H_
#define _DRV_PWM_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif
///< 定时器配置
typedef struct TDrvPwmTimerCfgTag TDrvPwmTimerCfg;
struct TDrvPwmTimerCfgTag
{
    UInt8   tim_index;         ///< 定时器序列号
    UInt32  pwm_frequence;     ///< pwm输出频率
    Bool    time_is_open;      ///< 定时器是否打开
    UInt32  pwm_period;        ///< pwm周期值(自动计算)
};
///< 定时器通道配置
typedef struct TDrvPwmCfgTag TDrvPwmCfg;
struct TDrvPwmCfgTag
{
    UInt8           pwm_tim_index_;    ///< TDrvPwmTimerCfg序列号
    UInt8           tim_chn_;          ///< 定时器通道
    TDrvPort       *pwm_port;          //< 接收引脚所在的端口
    Byte            pwm_pin;           //< 接收引脚编号
    TPortMux        gpio_mux;          //< 引脚复用功能
};

extern const TDrvPwmCfg g_drv_pwm_cfgs[3];

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_PWM_CFG_H_
