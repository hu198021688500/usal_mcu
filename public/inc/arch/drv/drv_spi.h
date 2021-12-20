/**
 * @file drv_spi.h
 * @brief SPI主驱动
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

#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_
#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvSpiTag TDrvSpi;

struct TDrvSpiTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    Bool is_open_;
    Int32 index_;  ///< 配置索引
    Int32 spi_index_;  ///< spi索引
    
};


/**
 * 驱动创建
 * @param self: 对象实例指针
 * @param cfg: SPI模块配置信息
 * @return 创建成功返回True,否则返回False,参数错误
 */
Bool TDrvSpiCreate(TDrvSpi *self, UInt8 index);

/**
 * 驱动打开
 * @param self: 对象实例指针
 * @return
 */
void TDrvSpiOpen(TDrvSpi *self);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return
 */
void TDrvSpiClose(TDrvSpi *self);

/**
 * 驱动是否已经打开
 * @param
 * @return 已经打开返回True,否则返回False
 */
Bool TDrvSpiIsOpen(TDrvSpi *self);
/**
 * spi发送和读取数据
 * @param value:发送数据:读取一字节发送0xff
 * @return 返回读取到的数据
 */
Byte TDrvSpiWriteAndRead(TDrvSpi *self, Byte value);
    
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _DRV_SPI_H_
