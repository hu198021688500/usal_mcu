/**
 * @file drv_adc_cfg.c
 * @brief 用于adc配置
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
TDrvAdcCfg g_drv_adc_cfgs[] = 
{
    [0] = {           ///< Adc设备-系统温度采样(进风口)
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 0,
            .adc_channel = 10,
            .adc_sample_tim = 6,
          },
    [1] = {           ///< Adc设备-系统温度采样(出风口)
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 1,
            .adc_channel = 11,
            .adc_sample_tim = 6,
          },
    [2] = {           ///< Adc设备-枪1正极温度采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 0,
            .adc_channel = 0,
            .adc_sample_tim = 6,
          },
    [3] = {           ///< Adc设备-枪1负极温度采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 1,
            .adc_channel = 1,
            .adc_sample_tim = 6,
          },
    [4] = {           ///< Adc设备-枪2正极温度采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 2,
            .adc_channel = 12,
            .adc_sample_tim = 6,
          },
    [5] = {           ///< Adc设备-枪2负极温度采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 3,
            .adc_channel = 13,
            .adc_sample_tim = 6,
          },
    [6] = {           ///< Adc设备-枪1母线正对PE电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 4,
            .adc_channel = 4,
            .adc_sample_tim = 6,
          },
    [7] = {           ///< Adc设备-枪1母线负对PE电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 5,
            .adc_channel = 5,
            .adc_sample_tim = 6,
          },
    [8] = {           ///< Adc设备-枪2母线正对PE电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 2,
            .adc_channel = 2,
            .adc_sample_tim = 6,
          },
    [9] = {           ///< Adc设备-枪2母线负对PE电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 3,
            .adc_channel = 3,
            .adc_sample_tim = 6,
          },
    [10] = {           ///< Adc设备-枪1母线外侧电压
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 6,
            .adc_channel = 6,
            .adc_sample_tim = 6,
          },
    [11] = {           ///< Adc设备-枪2母线外侧电压
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 4,
            .adc_channel = 14,
            .adc_sample_tim = 6,
          },
    [12] = {           ///< Adc设备-枪1电流采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumA],
            .adc_pin = 7,
            .adc_channel = 7,
            .adc_sample_tim = 6,
          },
    [13] = {           ///< Adc设备-枪2电流采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumC],
            .adc_pin = 5,
            .adc_channel = 15,
            .adc_sample_tim = 6,
          },
    [14] = {           ///< Adc设备-枪1CC1电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumB],
            .adc_pin = 0,
            .adc_channel = 8,
            .adc_sample_tim = 6,
          },
    [15] = {           ///< Adc设备-枪2CC1电压采样
            .base =     ADC1,
            .adc_port = &g_drv_port[kDrvPortNumB],
            .adc_pin = 1,
            .adc_channel = 9,
            .adc_sample_tim = 6,
          },
};
