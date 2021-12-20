/**
 * @file upgrade.h
 * @brief 升级相关处理
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

#ifndef _BOOTLDR_UPGRADE_H_
#define _BOOTLDR_UPGRADE_H_

#include "ofc_types.h"
#include "ofc_object.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * 升级类
 * @par 示例 
 * @code
 * @endcode
 */
typedef struct TBootldrUpgradeTag TBootldrUpgrade;

struct TBootldrUpgradeTag
{
PROTECTED
    TOfObject parent_;              ///< 继承的父类
    TOfVirtualMethod Destroy_;      ///< 析构函数

PRIVATE
    
};


extern TBootldrUpgrade g_upgrade;  ///< 内存单例模式对象

/**
 * 构造函数
 * @param self: 对象实例指针
 * @return 
 */
void TBootldrUpgradeCreate(TBootldrUpgrade *self);

/**
 * 检查是否有更新
 * @param self: 对象实例指针
 * @return True:有更新 False:无更新
 */
Bool TBootldrUpgradeCheckIsUpdate(TBootldrUpgrade *self);

/**
 * 从备份区拷贝app
 * @param self: 对象实例指针
 * @return True:更新成功 False:更新失败
 */
Bool TBootldrUpgradeCopyApp(TBootldrUpgrade *self);


#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _BOOTLDR_UPGRADE_H_
