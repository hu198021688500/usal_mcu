/**
 * @file drv_flash.h
 * @brief 内部Flash存取驱动
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
#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_
#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Flash驱动配置信息
 */
typedef struct TDrvFlashConfigTag TDrvFlashConfig;
struct TDrvFlashConfigTag
{
    Int32 eeprom_base;      ///< eeprom的基地址
    Int32 eeprom_size;      ///< eeprom的大小
    Int32 eeprom_unit_size; ///< eeprom单次写入的大小

    Int32 flash_base;       ///< program flash的基地址
    Int32 flash_size;       ///< program flash的大小
    Int32 flash_unit_size;  ///< flash单次写入的大小
    Int32 page_size;        ///< program flash的页大小
};

typedef struct TDrvFlashTag TDrvFlash;

/**
 * flash命令执行时的回调函数类型
 * @param sender: 产生通知的对象
 * @return
 */
typedef void (*TDrvFlashDoCmdPending)(TDrvFlash *sender);

/**
 * 构造一个Flash和EEPROM的读写对象
 * @par 示例
 * @code
   TDrvFlash flash;

   TDrvFlashCreate(&flash);
   TDrvFlashOpen(&flash);

   // TDrvFlashWrite...
   // ...
   TDrvFlashClose(&flash);
   OfDestroyObject((TOfObject *)&flash);
 * @endcode
*/
struct TDrvFlashTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;

PRIVATE
    const TDrvFlashConfig *cfg_;

PUBLIC
    /// 存放事件接收者
    void *receiver;

    /// 执行Flash操作命令时,由于有些指令执行时间比较长,有可能导致开门狗重启,故需要在等待命令执行完成时做一些操作
    TDrvFlashDoCmdPending OnCmdPending;
};

extern TDrvFlash g_drv_flash;  ///< FLASH驱动对象

/**
 * 驱动创建
 * @param self: 对象实例指针
 * @return
 */
void TDrvFlashCreate(TDrvFlash *self);

/**
 * 驱动打开
 * @param self: 对象实例指针
 * @return 打开成功返回true,否则为false
 */
Bool TDrvFlashOpen(TDrvFlash *self);

/**
 * 驱动关闭
 * @param self: 对象实例指针
 * @return
 */
void TDrvFlashClose(TDrvFlash *self);

/**
 * 读取Program Flash数据
 * @param self: 对象实例指针
 * @param offset: 读取的数据在Flash中的偏移位置
 * @param buf: 存放数据的缓冲区
 * @param size: 缓冲区长度
 * @return >0:读成功个数
 */
Int32 TDrvFlashRead(TDrvFlash *self, Int32 offset, void *buf, Int32 size);

/**
 * 写数据到Program Flash
 * @param self: 对象实例指针
 * @param offset: 写入页的起始位置,起始位置必须是flash_unit_size的整数倍
 * @param data: 数据指针
 * @param size: 数据长度; 长度必须是flash_unit_size的整数倍
 * @return >0:写成功个数
 */
Int32 TDrvFlashWrite(TDrvFlash *self, Int32 offset, const void *data, Int32 size);

/**
 * 擦除Program Flash页
 * @param self: 对象实例指针
 * @param offset: 擦除页的起始位置
 * @param size: 擦除长度; 长度必须是page_size的整数倍
 * @return 擦除是否成功,true: 成功, false: 失败
 */
Bool TDrvFlashErasePage(TDrvFlash *self, Int32 offset, Int32 size);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_FLASH_H_
