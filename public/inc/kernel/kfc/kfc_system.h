/**
 * @file kfc_system.h
 * @brief 内核系统接口
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

#ifndef _KFC_SYSTEM_H_
#define _KFC_SYSTEM_H_

#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 线程休眠函数
 * @param ms: 休眠时长,单位: ms
 * @return
 */
void KfcSleep(UInt32 ms);

/**
* 得到当前的tick数,单位: 毫秒
* @return tick数
*/
UInt32 KfcGetTickCount(void);
/**
* 微秒软件延迟(不休眠)
* @return us:微秒
*/
void KfcDelayUs(UInt32 us);
/**
* 毫秒软件延迟(不休眠)
* @return ms:毫秒
*/
void KfcDelayMs(UInt32 ms);
#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_SYSTEM_H_
