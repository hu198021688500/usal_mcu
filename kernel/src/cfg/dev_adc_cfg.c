/**
 * @file dev_adc_cfg.c
 * @brief 用于adc设备配置
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
 
TDevAdcConfig g_dev_adc_cfg_[] =
{
    [0] = {.drv_adc_chn = 0,},           ///< Adc设备-0
    [1] = {.drv_adc_chn = 1,},           ///< Adc设备-1
    [2] = {.drv_adc_chn = 2,},           ///< Adc设备-2
    [3] = {.drv_adc_chn = 3,},           ///< Adc设备-3
    [4] = {.drv_adc_chn = 4,},           ///< Adc设备-4
    [5] = {.drv_adc_chn = 5,},           ///< Adc设备-5
};

