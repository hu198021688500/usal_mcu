/**
 * @file         ofc_chain.h
 * @brief        链表类
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
#ifndef _OFC_CHAIN_H_
#define _OFC_CHAIN_H_

#include "ofc_object.h"
#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TOfChainTag TOfChain;

/**
 * 链表节点
 */
typedef struct TOfChainNodeTag TOfChainNode;
struct TOfChainNodeTag
{
    TOfObject parent_;  ///< 父亲类

PROTECTED
    TOfVirtualMethod Destroy_;  ///< 析构事件

PRIVATE
    TOfChain *owner_;  ///< 所属链表对象
    TOfChainNode *prev_;  ///< 上一个结点
    TOfChainNode *next_;  ///< 下一个结点
    void *data_;  ///< 自定义数据
    Bool freeing_;  ///< 正在释放当前结点
};

/**
 * 链表节点构造函数
 * @param self: 对象指针
 * @param owner: 链表所属者
 * @param data: 私有数据
 * @return 
 */
void TOfChainNodeCreate(TOfChainNode *self, TOfChain *owner, void *data);

/**
 * 获取节点所属者
 * @param self: 对象指针
 * @return 所属者
 */
TOfChain *TOfChainNodeOwner(TOfChainNode *self);

/**
 * 获取当前节点的上一个节点
 * @param self: 对象指针
 * @return 上一个节点
 */
TOfChainNode *TOfChainNodePrev(TOfChainNode *self);

/**
 * 获取当前节点的下一个节点
 * @param self: 对象指针
 * @return 下一个节点
 */
TOfChainNode *TOfChainNodeNext(TOfChainNode *self);

/**
 * 获取当前节点的私有数据
 * @param self: 对象指针
 * @return 私有数据
 */
void *TOfChainNodeData(TOfChainNode *self);

/**
 * 设置私有数据
 * @param self: 对象指针
 * @param data: 私有数据
 * @return 
 */
void TOfChainNodeSetData(TOfChainNode *self, void *data);


/**
 * 链表锁/解锁事件
 * @param sender: 事件发起者
 * @return
 */
typedef void (*TOfChainDoLock)(void *sender);

/**
 * 链表锁/解锁事件
 */
typedef struct TOfChainOnLockTag TOfChainOnLock;
struct TOfChainOnLockTag
{
    TOfChainDoLock Method;  ///< 方法
    void  *object;  ///< 对象指针，指针TOfChainDoLock的第一个参数
};

typedef void (*TOfChainDoDeletion)(void *sender, TOfChainNode *node);
typedef struct TOfChainOnDeletionTag TOfChainOnDeletion;
struct TOfChainOnDeletionTag
{
    TOfChainDoDeletion Method;  ///< 方法
    void *object;  ///< 对象指针，指针TOfChainDoDeletion的第一个参数
};

struct TOfChainTag
{
    TOfObject parent_;  ///< 父亲类

PUBLIC
    TOfChainOnDeletion OnDeletion; ///< 结点删除事件

PROTECTED
    TOfVirtualMethod Destroy_;  ///< 析构事件

PRIVATE
    TOfChainOnLock Lock_;  ///< 锁事件
    TOfChainOnLock Unlock_;  ///< 解锁事件
    void *data_;  ///< 自定义数据
    TOfChainNode *head_;  ///< 首结点
    TOfChainNode *tail_;  ///< 尾结点
    Int32 count_;  ///< 结点个数
};

/**
 * 链表构造函数
 * @param self: 对象指针
 * @param lock: 锁事件，可为NULL
 * @param unlock: 解锁事件,可为NULL
 * @note 当lock和unlock都不为NULL时，锁事件才有效，否则锁事件无效 
 * @return 
 */
void TOfChainCreate(TOfChain *self, TOfChainOnLock *lock, TOfChainOnLock *unlock);

/**
 * 创建一个链表对象
 * @param lock: 锁事件，可为NULL
 * @param unlock: 解锁事件,可为NULL
 * @note 当lock和unlock都不为NULL时，锁事件才有效，否则锁事件无效 
 * @return 
 */
TOfChain *OfCreateChain(TOfChainOnLock *lock, TOfChainOnLock *unlock);

/**
 * 获取属性
 * @param self: 对象指针
 * @return
 */
void *TOfChainData(TOfChain *self);

/**
 * 获取当前存储个数
 * @param self: 对象指针
 * @return
 */
Int32 TOfChainCount(TOfChain *self);

/**
 * 设置自定义数据
 * @param self: 对象指针
 * @param data: 自定义数据
 * @return
 */
void TOfChainSetData(TOfChain *self, void *data);

/**
 * 清除链表所有结点, 会激发 OnDeletion 事件
 * @param self: 对象指针
 * @return
 */
void TOfChainClear(TOfChain *self);

/**
* 添加项数据
* @param self: 对象指针
* @param data: 项数据
* @return NULL: 添加失败; 其他：返回添加的结点
*/
TOfChainNode *TOfChainAddData(TOfChain *self, void *data);

/**
 * 添加结点
 * @param self: 对象指针
 * @param node: 结点
 * @return NULL: 添加失败; 其他：返回添加的结点
 */
TOfChainNode *TOfChainAddNode(TOfChain *self, TOfChainNode *node);

/**
 * 插入指定结点之前
 * @param self: 对象指针
 * @param data: 项数据
 * @param node: 指定的结点,当为NULL时在头部插入
 * @return NULL: 添加失败; 其他：返回插入的结点
 */
TOfChainNode *TOfChainInsertData(TOfChain *self, void *data, TOfChainNode *node);

/**
 * 插入指定结点之前
 * @param self: 对象指针
 * @param insert_node: 插入的结点
 * @param node: 指定的结点,当为NULL时在头部插入
 * @return NULL: 添加失败; 其他：返回插入的结点
 */
TOfChainNode *TOfChainInsertNode(TOfChain *self, TOfChainNode *insert_node, TOfChainNode *node);

/**
 * 删除相同数据的第一项, 会激发 OnDeletion 事件
 * @param self: 对象指针
 * @param data: 项数据
 * @return True: 删除成功; False: 删除失败
 */
Bool TOfChainDeleteData(TOfChain *self, void *data);

/**
 * 删除结点项,会delete node,会激发 OnDeletion 事件
 * @param self: 对象指针
 * @param node: 结点项
 * @return 
 */
void TOfChainDeleteNode(TOfChain *self, TOfChainNode *node);

/**
 * 移除结点项，仅仅从Chain中移除,不会delete node,不会激发 OnDeletion 事件
 * @param self: 对象指针
 * @param node: 结点项
 * @return
 */
void TOfChainRemoveNode(TOfChain *self, TOfChainNode *node);

/**
 * 判断项数据是否存在，查找第一个出现相同的项数据结点
 * @param self: 对象指针
 * @param data: 项数据
 * @return True: 存在; False: 不存在
 */
Bool TOfChainExistedData(TOfChain *self, void *data);

/**
 * 查找相同数据所在的第一个结点
 * @param self: 对象指针
 * @param data: 项数据
 * @return NULL: 查找失败; 其他：返回项数据所在的结点
 */
TOfChainNode *TOfChainFindData(TOfChain *self, void *data);

/**
 * 查找结点是否存在
 * @param self: 对象指针
 * @param node: 结点
 * @return True: 存在; False: 不存在
 */
Bool TOfChainFindNode(TOfChain *self, TOfChainNode *node);

/**
 * 获取第一项结点
 * @param self: 对象指针
 * @return 结点项
 */
TOfChainNode *TOfChainFirstNode(TOfChain *self);
 
/**
 * 获取最后一项结点
 * @param self: 对象指针
 * @return 结点项
 */
TOfChainNode *TOfChainLastNode(TOfChain *self);

/**
 * 获取指定结点的前一项
 * @param self: 对象指针
 * @param node: 指定结点,若为NULL，则返回值返回Tail
 * @return 结点项
 */
TOfChainNode *TOfChainPrevNode(TOfChain *self, TOfChainNode *node);

/**
 * 获取指定结点的后一项
 * @param self: 对象指针
 * @param node: 指定结点,若为NULL,则返回值返回Head
 * @return 结点项
 */
TOfChainNode *TOfChainNextNode(TOfChain *self, TOfChainNode *node);


#ifdef __cplusplus
}  // extern "C"
#endif
#endif  // _OFC_CHAIN_H_
