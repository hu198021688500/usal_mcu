/**
 * @file hmi_param_tab_type.h
 * @brief 交互参数表类型
 * @note
 * @author
 * @date 2021-7-2
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-7-2    <td>1.0                   <td>                <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */
#ifndef _HMI_PARAM_TAB_TYPE_H_
#define _HMI_PARAM_TAB_TYPE_H_

#include <ofc_object.h>

#ifdef __cplusplus
extern "C" {
#endif

enum THmiParamCmdResultTag
{
    kHmiParamCmdResultOk    = 0,       ///< 应答结果成功
    kHmiParamCmdResultIdErr,           ///< 应答结果参数id错误
    kHmiParamCmdResultParamErr,        ///< 应答结果内容错误
    kHmiParamCmdResultPermissionErr,   ///< 应答结果权限错误
    kHmiParamCmdResultDoErr,           ///< 应答结果执行失败
};
typedef enum THmiParamCmdResultTag THmiParamCmdResult;

/**
 * 定义参数项具体回调接口
 * @param is_set: False:读取 True:设置
 * @param data: 读取或者设置地址
 * @param len: 读取或者设置长度
 * @return THmiParamCmdResult结果
 */
typedef THmiParamCmdResult (*TCmdParamCallback)(Bool is_set, UInt8 *data, UInt32 len);
typedef struct TParamItemTag TParamItem;
struct TParamItemTag
{
    UInt8         param_id;               ///< 参数id
    UInt8         param_len;              ///< 参数长度
    TCmdParamCallback  call_back;         ///< 参数回调
};


#ifdef __cplusplus
}
#endif
#endif
