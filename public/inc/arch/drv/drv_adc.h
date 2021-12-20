/**
 * @file drv_adc.h
 * @brief 模拟数字转换驱动程序
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

#ifndef _DRV_ADC_H_
#define _DRV_ADC_H_

#include <ofc_object.h>
#include <ofc_types.h>
#include "drv_dma.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ADC驱动类
 * @par 示例
 * @code
 * @endcode
*/
typedef struct TDrvAdcTag TDrvAdc;
struct TDrvAdcTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    void *cfg_;  ///< 基础配置
    Byte index_;
    Byte adc_index_;
    Byte chn_index_;
    Bool is_open_;
    
    TDrvDmaCh    dma_chn_;
};

/**
 * 驱动类的构造函数
 * @param self: 对象实例指针
 * @param index: 索引0, 1
 * @return
 */
void TDrvAdcCreate(TDrvAdc *self, Int32 index);

/**
 * 驱动类的打开
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvAdcOpen(TDrvAdc *self);

/**
 * 驱动类的关闭
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TDrvAdcClose(TDrvAdc *self);

/**
 * 驱动类的读取数据
 * @param self: 对象实例指针
 * @return 返回转换后的电压值, 单位毫伏
 */
UInt16 TDrvAdcRead(TDrvAdc *self);

#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif  ///< _DRV_ADC_H_
