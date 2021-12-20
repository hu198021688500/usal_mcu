/**
 * @file Kfc_dev.h
 * @brief 操作设备接口
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

#ifndef _KFC_DEV_H_
#define _KFC_DEV_H_

#include <ofc_types.h>
#include "dev_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 打开设备
 * @param dev_num: 设备号
 * @return True:成功 False:失败
 */
Bool KfcDevOpen(UInt16 dev_num);
    
/**
 * 关闭设备
 * @param dev_num: 设备号
 * @return True: 成功 False:失败
 */
Bool KfcDevClose(UInt16 dev_num);

/**
 * 设备ioctl
 * @param dev_num: 设备号
 * @param  cmd: 命令字
 * @param argv: 命令参数
 * @return True: 成功 False:失败
 */
Bool KfcDevIoctl(UInt16 dev_num, UInt32 cmd, void *argv);
    
/**
 * 设备读取函数
 * @param dev_num: 设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 KfcDevRead(UInt16 dev_num, void *buf, UInt32 len);

/**
 * 设备写函数
 * @param dev_num: 设备号
 * @param  buf: 数据存放地址
 * @param  len: 数据长度
 * @return <= 0: 失败  >0: 成功字节数
 */
Int32 KfcDevWrite(UInt16 dev_num, void *buf, UInt32 len);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _KFC_DEV_H_
