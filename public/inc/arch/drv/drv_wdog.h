/**
 * @file drv_wdog.h
 * @brief 看门狗驱动层
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
#ifndef _DRV_WDOG_H_
#define _DRV_WDOG_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 使用看门狗操作
 * @par 示例
 * @code
   TDrvWdogCreate(&g_drv_wdog);
   TDrvWdogOpen(&g_drv_wdog);
   ....
   TDrvWdogFeedDog(&g_drv_wdog);
 * @endcode
*/
 
typedef struct TDrvWdogTag TDrvWdog;
struct TDrvWdogTag
{
PROTECTED
    TOfObject parent_;
    TOfVirtualMethod Destroy_;
};

extern TDrvWdog g_drv_wdog;

/**
 * 创建一个看门狗设备驱动
 * @param self: 对象实例指针
 * @return
 */
void TDrvWdogCreate(TDrvWdog *self);

/**
 * 打开看门狗
 * @param self: 对象实例指针
 * @return
 */
void TDrvWdogOpen(TDrvWdog *self);

/**
 * 关闭看门狗(暂时为空,无法关闭)
 * @param self: 对象实例指针
 * @return
 */
void TDrvWdogClose(TDrvWdog *self);

/**
 * 喂狗(默认2s内需要喂狗)
 * @param self: 对象实例指针
 * @return
 */
void TDrvWdogFeedDog(TDrvWdog *self);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif //_DRV_WDOG_H_
