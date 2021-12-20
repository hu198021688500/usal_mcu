/**
 * @file drv_dma.h
 * @brief DMA驱动程序
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

#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_

#include <ofc_object.h>
#include <ofc_types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvDmaTag TDrvDma;


struct TDrvDmaTag
{
PROTECTED
    TOfObject parent_;  ///< 继承的父类
    TOfVirtualMethod Destroy_; ///< 析构函数

    void  *base_;
    void  *chs_base_;
PRIVATE
    Int32 index_;
    Bool is_open_;  ///< 是否已经打开

};

/**
 * Dma控制器构造函数
 * @param self: 对象实例指针
 * @return
 */
void TDrvDmaCreate(TDrvDma *self, Int32 index);

/**
 * 打开DMA驱动
 * @param self: 对象实例指针
 * @return 
 */
void TDrvDmaOpen(TDrvDma *self);

/**
 * 关闭DMA驱动
 * @note: 关闭时会自动关闭所有打开的通道
 * @param self: 对象实例指针
 * @return
 */
void TDrvDmaClose(TDrvDma *self);

extern TDrvDma g_drv_dma[2];  ///< DMA驱动对象



typedef struct TDrvDmaChTag TDrvDmaCh;
// /**
//  * DMA通知事件的函数类型
//  * @param sender: 事件的产生者
//  * @return
//  */
// typedef void (*TDrvDmaNotifyEvent)(TDrvDmaCh *sender);

// /**
//  * DMA错误事件的函数类型
//  * @param sender: 事件的产生者
//  * @param errors: 错误信息,每种错误是一个bit,可以通过TDrvDmaErrorMask的值的掩码来得到具体的错误.如if (errors & kDmaErrorDstBusMask) ...
//  * @return
//  */
// typedef void (*TDrvDmaOnError)(TDrvDmaCh *sender, UInt32 errors);


typedef struct TDrvDmaChCfgTag TDrvDmaChCfg;
struct TDrvDmaChCfgTag
{
  UInt8 *buffer;
  UInt32 buffer_size;
  UInt32 data_len;
  UInt32 peripheral_addr;
  Bool   dir;
};


struct TDrvDmaChTag
{
PROTECTED
    TOfObject parent_;  ///< 继承的父类
    TOfVirtualMethod Destroy_;  ///< 析构函数

PUBLIC
    //void *receiver;  ///< 事件的接收者
    // TDrvDmaNotifyEvent OnComplete;  ///< DMA完成事件; 注: 该事件在中断中调用
    // TDrvDmaOnError OnError;  ///< DMA错误事件; 注: 该事件在中断中调用.调用前已经禁用DMA请求

PRIVATE
//    TDrvDma *dma_;  ///< DMA管理器
    UInt32 index_;  ///< DMA通道号
    UInt32 source_;  ///< 请求源
    Bool is_open_;  ///< 是否已经打开
    TDrvDmaChCfg chn_cfg_;    ///< 通道配置
};

/**
 * Dma通道构造函数
 * @param self: 对象实例指针
 * @param dma: DMA控制器
 * @param channel: DMA通道号
 * @param source: 该通道对应的输入源
 * @return
 */
void TDrvDmaChCreate(TDrvDmaCh *self, UInt32 index, TDrvDmaChCfg *chn_cfg);

/**
 * 打开DMA通道
 * @param self: 对象实例指针
 * @return 
 */
void TDrvDmaChOpen(TDrvDmaCh *self);

/**
 * 关闭DMA通道
 * @note: 关闭时会自动关闭所有打开的通道
 * @param self: 对象实例指针
 * @return
 */
void TDrvDmaChClose(TDrvDmaCh *self);


/**
 * 启动DMA传输
 * @note: 
 * @param self: 对象实例指针
 * @return
 */
void TDrvDmaChStart(TDrvDmaCh *self, Bool is_wait_finish);


#ifdef __cplusplus
}  ///< extern "C"
#endif

#endif  ///< _DRV_DMA_H_
