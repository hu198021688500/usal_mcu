/**
 * @file drv_port.h
 * @brief 端口操作驱动层
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
#ifndef _DRV_PORT_H_
#define _DRV_PORT_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 端口引脚功能的枚举
 */
typedef enum TPortMuxTag TPortMux;
enum TPortMuxTag
{
    kPortMuxDisabled        = 0U,   ///< 引脚默认功能
    kPortMuxAlt1            = 1U,   ///< 引脚功能1
    kPortMuxAlt2            = 2U,   ///< 引脚功能2
    kPortMuxAlt3            = 3U,   ///< 引脚功能3
    kPortMuxAlt4            = 4U,   ///< 引脚功能4
    kPortMuxAlt5            = 5U,   ///< 引脚功能5
    kPortMuxAlt6            = 6U,   ///< 引脚功能6
    kPortMuxAlt7            = 7U,   ///< 引脚功能7
    kPortMuxAlt8            = 8U,   ///< 引脚功能8
    kPortMuxAlt9            = 9U,   ///< 引脚功能9
    kPortMuxAlt10           = 10U,  ///< 引脚功能10
    kPortMuxAlt11           = 11U,  ///< 引脚功能11
    
    kPortMuxAltAdc          = 16U,   ///< 引脚功能11,ADC输入
};
/**
 * 端口引脚拉功能的枚举
 */
typedef enum TPortPullTag TPortPull;
enum TPortPullTag
{
    kPortPullDisabled       = 0U,   ///< 引脚拉功能禁止
    kPortPullUp             = 1U,   ///< 引脚上拉使能
    kPortPullDown           = 2U,   ///< 引脚下拉使能
};
/**
 * 端口驱动能力的枚举
 */
typedef enum TPortStrengthTag TPortStrength;
enum TPortStrengthTag
{
    kPortStrengthLow        = 0U,   ///< 弱驱动
    kPortStrengthHigh       = 1U,   ///< 强驱动
};

typedef struct TDrvPortPinConfigTag TDrvPortPinConfig;
struct TDrvPortPinConfigTag
{
    TPortMux            mux;            ///< 复用功能
    TPortPull           pull;           ///< 上下拉使能
    TPortStrength       strength;       ///< 驱动能力
};


/**
 * 
 * @par 示例
 * @code
 * @endcode
*/
    
typedef struct TDrvPortTag TDrvPort;
struct TDrvPortTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;
    
    void            *gpio_type_;        ///< gpio寄存器地址
    Bool            is_create_;         ///< 是否创建
};

/**
 * 创建一个端口设备
 * @param self: 对象实例指针
 * @return
 */
void TDrvPortCreate(TDrvPort *self);
/**
 * 打开引脚
 * @param self: 对象实例指针
 * @param  pin: 引脚号
 * @param is_out: 是否输出
 * @param config: 配置项,NULL为默认输入输出配置
 * @return True:成功 False:失败
 */
Bool TDrvPortPinOpen(TDrvPort *self, UInt8 pin, Bool is_out, TDrvPortPinConfig *config);

/**
 * 关闭引脚
 * @param self: 对象实例指针
 * @param pin: 引脚号
 * @return True:成功 False:失败
 */
Bool TDrvPortPinClose(TDrvPort *self, UInt8 pin);

/**
 * 设置引脚状态
 * @param self: 对象实例指针
 * @param  pin: 引脚
 * @param  status: 设置状态
 * @return True:成功 False:失败
 */
Bool TDrvPortPinSet(TDrvPort *self, UInt8 pin, Bool status);

/**
 * 获取引脚状态
 * @param self: 对象实例指针
 * @param  pin: 引脚
 * @param  status: 获取状态
 * @return True:成功 False:失败
 */
Bool TDrvPortPinGet(TDrvPort *self, UInt8 pin, Bool *status);

/**
 * 对外端口号
 */
 
typedef enum TDrvPortNumTag TDrvPortNum;
enum TDrvPortNumTag
{
    kDrvPortNumA       = 0,        ///< port_A
    kDrvPortNumB,
    kDrvPortNumC,
    kDrvPortNumD,
    kDrvPortNumE,
    kDrvPortNumF,
    kDrvPortNumG,
    kDrvPortNumH,
    kDrvPortNumI,
    kDrvPortNumJ,
    kDrvPortNumK,
    
    kDrvPortNumMax,
};
extern TDrvPort g_drv_port[kDrvPortNumMax];

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_PORT_H_
