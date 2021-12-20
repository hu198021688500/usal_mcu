/**
 * @file drv_uart.h
 * @brief 串口驱动层
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
#ifndef _DRV_UART_H_
#define _DRV_UART_H_

#include <ofc_object.h>
#include <ofc_ring_buf.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 校验位的枚举
 */
typedef enum TDrvUartParityTag TDrvUartParity;
enum TDrvUartParityTag
{
    kDrvUartParityNone   = 0,      ///< 无校验
    kDrvUartParityEven   = 1,      ///< 偶校验
    kDrvUartParityOdd    = 2,      ///< 奇校验
};

/**
 * uart模式的枚举
 */
typedef enum TDrvUartModeTag TDrvUartMode;
enum TDrvUartModeTag
{
    kDrvUartMode232  = 0,      ///< 串口
    kDrvUartMode485  = 1,      ///< 485
};


typedef struct TDrvUartTag TDrvUart;

/**
 * 通知函数类型
 * @param sender: 产生通知的对象
 * @return
 */
typedef Int32 (*TDrvUartDoNotify)(TDrvUart *sender);
/**
 * 构造一个串口读写对象
 * @par 示例
 * @code
 * @endcode
*/
struct TDrvUartTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    const void *cfg_;                ///< 基础配置
    TOfRingBuf rx_buf_;          ///< 接收数据缓冲区
    Int32 index_;                    ///< 串口号

    Int32 fifo_size_;                ///< 硬件FIFO大小
    TDrvUartMode mode_;              ///< uart的模式
    Bool is_open_;

PUBLIC
    /// 存放事件接收者
    void *receiver;

    /// 当有数据收到时调用的函数.注意该函数在中断中调用
    TDrvUartDoNotify OnDataReceived;

    /// 当有空闲区域可以发送数据时调用该函数.注意该函数在中断中调用
    TDrvUartDoNotify OnDataSent;
};

/**
 * 驱动创建
 * @param self: 对象实例指针
 * @param index: 串口索引
 * @param rx_buf: 接收缓冲区
 * @param rx_buf_size: 接收缓冲区大小,需要确保大小比串口的FIFO容量大
 * @param mode: 串口模式; 注: 如果设置为485模式,且该UART的配置中没有配置rts引脚相关的信息,调用会崩溃
 * @return
 */
void TDrvUartCreate(TDrvUart *self, Int32 index, void *rx_buf, Int32 rx_buf_size, TDrvUartMode mode);

/**
 * 驱动打开
 * @param self: 对象实例指针
 * @param baudrate: 波特率,如115200, 9600
 * @param parity: 奇偶位类型
 * @return
 */
void TDrvUartOpen(TDrvUart *self, Int32 baudrate, TDrvUartParity parity);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return
 */
void TDrvUartClose(TDrvUart *self);
/**
 * 串口数据读取
 * @param self: 对象实例指针
 * @param buf: 缓存buffer
 * @param size: 缓存大小
 * @return
 */
Int32 TDrvUartRead( TDrvUart *self, void *buf, Int32 size);
/**
 * 串口数据发送
 * @param self: 对象实例指针
 * @param data: 数据区
 * @param size: 数据大小
 * @return
 */
Int32 TDrvUartWrite( TDrvUart *self, const void *data, Int32 size);
/**
 * 返回当前串口是否已经打开
 * @param
 * @return 
 */
Bool TDrvUartIsOpen(TDrvUart *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_UART_H_
