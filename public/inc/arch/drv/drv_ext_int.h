/**
 * @file drv_ext_int.h
 * @brief 外部引脚中断驱动 
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

#ifndef _DRV_EXT_INT_H_
#define _DRV_EXT_INT_H_

#include <ofc_object.h>
#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 引脚中断触发模式
 */
typedef enum TDrvExtIntTriggerModeTag TDrvExtIntTriggerMode;
enum TDrvExtIntTriggerModeTag
{
    kDrvExtIntDisable,          ///< 禁用中断
    kDrvExtIntRising,           ///< 上升沿有效
    kDrvExtIntFalling,          ///< 下降沿有效
    kDrvExtIntBoth,             ///< 上升沿和下降沿都有效
    
    ///----------------如下stm32不支持---------------------//
    kDrvExtIntLow,              ///< 低电平有效
    kDrvExtIntHigh,             ///< 高电平有效
};



typedef struct TDrvExtIntTag TDrvExtInt;
/**
 * 中断事件函数
 * @note: 由中断中调用
 * @param recevier: 事件的产生者
 * @return
 */
typedef void (*TDrvExtIntIrqEvent)(void *recevier);
    

struct TDrvExtIntTag
{
PROTECTED
    TOfObject parent_;                  ///< 继承的父类
    TOfVirtualMethod Destroy_;          ///< 析构函数

PUBLIC
    void *recevier;                     ///< 事件接收者
    TDrvExtIntIrqEvent   MethodOnIrq;   ///< 中断事件; 注: 电平触发的事件,在中断事件返回前需清除外设的中断电平

PRIVATE
    UInt8                pin_;          ///< 引脚
    TDrvPortNum           port_;         ///< 端口号
    TDrvExtInt          *next_;         ///< 下一个对象
};

/**
 * 构造函数
 * @note: 构造后必须先设置OnIrq,再调用TDrvExtIntSetTrigerMode来使能中断
 * @param self: 对象实例指针
 * @return 
 */
void TDrvExtIntCreate(TDrvExtInt *self, TDrvPortNum port, UInt8 pin);


/**
 * 设置中断触发模式
 * @note: 如果引脚配置为电平模式,建议使能该引脚的上下拉功能,否则外设芯片不焊接的话会一直产生中断
 * @param self: 对象实例指针
 * @param mode: 触发模式
 * @return 
 */
void TDrvExtIntSetTrigerMode(TDrvExtInt *self, TDrvExtIntTriggerMode mode);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_EXT_INT_H_
