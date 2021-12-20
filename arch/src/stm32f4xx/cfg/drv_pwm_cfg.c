/**
 * @file drv_pwm_cfg.c
 * @brief 用于pwm配置
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
 
 TDrvPwmTimerCfg g_drv_pwm_timer_cfgs[] = 
 {
     [0] = {
        .tim_index = 0,         ///< 定时器1
        .pwm_frequence = 20000,  ///< pwm频率
        .time_is_open = False,  ///< 未打开
    },
 };
 
const TDrvPwmCfg g_drv_pwm_cfgs[] = 
{
    [0] = {
        .pwm_tim_index_ = 0,    ///< 使用g_drv_pwm_timer_cfgs[0]
        .tim_chn_   = 0,        ///< 定时器通道1
        .pwm_port = &g_drv_port[kDrvPortNumE],
        .pwm_pin = 9,
        .gpio_mux = kPortMuxAlt1,
    },
};
