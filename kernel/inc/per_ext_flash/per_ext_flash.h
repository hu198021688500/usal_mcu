/**
 * @file per_ext_flash.h
 * @brief 外设-外部flash
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

#ifndef _KERNEL_PER_EXT_FLASH_H_
#define _KERNEL_PER_EXT_FLASH_H_

#include "ofc_types.h"
#include "ofc_object.h"

#include "drv_port.h"
#include "drv_spi.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum TPerExtFlashTypeTag TPerExtFlashType;
enum TPerExtFlashTypeTag
{
    kPerExtFlashTypeW25Q64      = 0,        ///< w25q64芯片
    kPerExtFlashTypeMx25L16,                ///< Mx25L1606e芯片
    kPerExtFlashTypeSst25,                  ///< sst25vf016b芯片
    
    kPerExtFlashTypeMax,
};

/**
 * 外部flash
 * @par 示例 
 * @code
 * @endcode
 */
typedef struct TPerExtFlashTag TPerExtFlash;

struct TPerExtFlashTag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    TDrvSpi     drv_spi_;              ///< spi驱动
    TDrvPort   *cs_port_;              ///< 片选端口
    UInt8       cs_pin_;               ///< 片选引脚
    
    TPerExtFlashType  chip_type_;            ///< 芯片类型0:w25q64, 1:mx25L1606e 2:sst25vf016b
    UInt32      total_size_;           ///< 总大小
    UInt32      sector_size_;          ///< 扇区大小
    UInt32      page_size_;            ///< 页大小
    
    UInt8       change_buffer_[4096];  ///< 扇区大小,用于自动写入时候的扇区缓存
};

extern TPerExtFlash g_per_ext_flash;

/**
 * 构造函数
 * @param self: 对象实例指针
 * @return 
 */
void TPerExtFlashCreate(TPerExtFlash *self);

/**
 * 打开设备
 * @param self: 对象实例指针
 * @return True:成功 False:失败
 */
Bool TPerExtFlashOpen(TPerExtFlash *self);

/**
 * 关闭设备
 * @param self: 对象实例指针
 * @return 
 */
void TPerExtFlashClose(TPerExtFlash *self);

/**
 * 芯片擦除(一般不用,写入函数自动判断进行擦除)
 * @param object: 对象实例
 * @param addr: 地址(扇区地址倍数)
 * @param size: 地址大小(扇区大小倍数)
 * @return True:成功 False:失败
 */
Bool TPerExtFlashErasePage(TPerExtFlash *self, UInt32 addr, UInt32 size);

/**
 * 芯片数据自动写入(支持自动检查是否需要擦除和写入,地址和大小只要不超过芯片最大限值就好)
 * @param object: 对象实例
 * @param addr: 地址
 * @param data: 数据地址
 * @param size: 数据大小
 * @return 写入成功数据大小
 */
Int32 TPerExtFlashWrite(TPerExtFlash *self, UInt32 addr, const UInt8 *data, Int32 size);
/**
 * 芯片数据自动读取(地址和大小只要不超过芯片最大限值就好)
 * @param object: 对象实例
 * @param addr: 地址
 * @param buf: 数据缓存
 * @param size: 数据大小
 * @return 读取成功数据大小
 */
Int32 TPerExtFlashRead(TPerExtFlash *self, UInt32 addr, UInt8 *buf, Int32 size);
/**
 * 芯片进入掉电模式
 * @param object: 对象实例
 * @return
 */
void TPerExtFlashPowerDown(TPerExtFlash *self);
/**
 * 芯片唤醒
 * @param object: 对象实例
 * @return
 */
void TPerExtFlashWakeUp(TPerExtFlash *self);
/**
 * 擦除整个芯片(耗时较长)
 * @param object: 对象实例
 * @return
 */
void TPerExtFlashEraseChip(TPerExtFlash *self);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KERNEL_PER_EXT_FLASH_H_
