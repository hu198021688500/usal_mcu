/**
 * @file per_can_mcp2515.h
 * @brief 外设-mcp2515 can模块
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

#ifndef _KERNEL_PER_CAN_MCP2515_H_
#define _KERNEL_PER_CAN_MCP2515_H_

#include "ofc_types.h"
#include "ofc_object.h"

#include "drv_port.h"
#include "drv_spi.h"
#include "drv_ext_int.h"
#include "kfc_timer.h"
#include <ofc_ring_buf.h>

#include "can_def.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Can驱动波特率的枚举
 */
typedef enum TPerCanMcp2515BaudRateTag TPerCanMcp2515BaudRate;
enum TPerCanMcp2515BaudRateTag
{
    kPerCanMcp2515BaudRate250k    = 0,  ///< 波特率为250K
    kPerCanMcp2515BaudRate500k,         ///< 波特率为500K
    kPerCanMcp2515BaudRate1000k,        ///< 波特率为1M
    kPerCanMcp2515BaudRate100k,         ///< 波特率为100K
    kPerCanMcp2515BaudRate125k,         ///< 波特率为125K
    
    kPerCanMcp2515BaudRateMax,
};
/**
 * spi can模块
 * @par 示例 
 * @code
 * @endcode
 */
typedef struct TPerCanMcp2515Tag TPerCanMcp2515;

struct TPerCanMcp2515Tag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    TDrvSpi     drv_spi_;              ///< spi驱动
    TDrvPort   *cs_port_;              ///< 片选端口
    UInt8       cs_pin_;               ///< 片选引脚
    
    TDrvExtInt  drv_ext_int_;          ///< 外部中断
    TOfRingBuf  rx_buf_;               ///< 接收数据缓冲区
    
    TKfcTimer  *check_timer;           ///< 检查接收定时器
    
};

extern TPerCanMcp2515 g_per_can_mcp2515;

/**
 * 构造函数
 * @param self: 对象实例指针
 * @param cs_port: 片选端口
 * @param cs_pin: 片选引脚
 * @return 
 */
void TPerCanMcp2515Create(TPerCanMcp2515 *self, void *rx_buf, Int32 rx_buf_size, TDrvPort *cs_port, UInt8 cs_pin, TDrvPortNum int_port, Byte int_pin);

/**
 * 打开设备
 * @param self: 对象实例指针
 * @return True: 成功 False:失败
 */
Bool TPerCanMcp2515Open(TPerCanMcp2515 *self, TPerCanMcp2515BaudRate baud_item);

/**
 * 关闭设备
 * @param self: 对象实例指针
 * @return 
 */
void TPerCanMcp2515Close(TPerCanMcp2515 *self);

/**
 * 写CAN数据到发送队列.写完后会自动开始发送
 * @note: 
 * @param self: 对象实例指针
 * @param data: 需要发送的CAN数据
 * @return True:成功 False:失败
 */
Bool TPerCanMcp2515Write(TPerCanMcp2515 *self, TCanData *data);
/**
 * 读取收到的CAN数据
 * @note: 
 * @param self: 对象实例指针
 * @param buf: 存放CAN数据的缓冲区
 * @param count: Buf能容纳多少个TCANVData类型的数据
 * @return 返回实际读取到的TCANVData类型的数据个数
 */
Int32 TPerCanMcp2515Read(TPerCanMcp2515 *self, TCanData *data, Int32 count);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_PER_CAN_MCP2515_H_
