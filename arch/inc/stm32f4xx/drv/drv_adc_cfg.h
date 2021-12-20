/**
 * @file drv_adc_cfg.h
 * @brief ADC驱动配置 
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

#ifndef _DRV_ADC_CFG_H_
#define _DRV_ADC_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvAdcCfgTag TDrvAdcCfg;
struct TDrvAdcCfgTag
{
    void            *base;  ///< 寄存器组基地址
    TDrvPort        *adc_port;           //< 接收引脚所在的端口
    Byte             adc_pin;             //< 接收引脚编号
    Byte             adc_channel;       ///< 通道号
    Byte             adc_sample_tim;     ///< 采样时间

    Byte             use_channel;       ///< 通道号
    // Int32 ref_val;  ///< 参考电压
    // Int8 mode;  ///< 转换模式
    // Int8 bits;  ///< 转换位数
    // Int8 smplt;  ///< 采样时间
    // Int8 pcc_idx;  ///< PCC索引
};

#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif ///< _DRV_ADC_CFG_H_
