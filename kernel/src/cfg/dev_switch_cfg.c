/**
 * @file dev_switch_cfg.c
 * @brief 用于开关量设备配置
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

TDevSwitchConfig g_dev_switch_cfg_[] =
{
    {&g_drv_port[kDrvPortNumD],     5,     kPortPullDisabled, False},    ///< 开关量设备-0
    {&g_drv_port[kDrvPortNumD],     4,     kPortPullDisabled, False},    ///< 开关量设备-1
    {&g_drv_port[kDrvPortNumD],     7,     kPortPullDisabled, False},    ///< 开关量设备-2
    {&g_drv_port[kDrvPortNumD],     6,     kPortPullDisabled, False},    ///< 开关量设备-3
    {&g_drv_port[kDrvPortNumG],    10,     kPortPullDisabled, False},    ///< 开关量设备-4
    {&g_drv_port[kDrvPortNumG],     9,     kPortPullDisabled, False},    ///< 开关量设备-5
};
