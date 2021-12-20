/**
 * @file drv_spi_cfg.h
 * @brief SPI驱动配置 
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

#ifndef _DRV_SPI_CFG_H_
#define _DRV_SPI_CFG_H_

#include "drv_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct TDrvSpiCfgTag TDrvSpiCfg;
struct TDrvSpiCfgTag
{
    UInt8           spi_index;         ///< spi序列号
    TDrvPort       *sck_port;          ///< spi_sck端口
    Byte            sck_pin;           ///< spi_sck引脚编号
    TDrvPort       *mosi_port;         ///< spi_mosi端口
    Byte            mosi_pin;          ///< spi_mosi引脚编号
    TDrvPort       *miso_port;         ///< spi_miso端口
    Byte            miso_pin;          ///< spi_miso引脚编号
    TPortMux        spi_mux;           ///< 引脚复用功能
    UInt32          baud_div;          ///< 波特率分频设置(0: /2 1:/4 2: /8 3: /16 ....7: / 256)
    Byte            cpol;              ///< 时钟极性 0:空闲SCK低电平 1:空闲SCK高电平
    Byte            cpha;              ///< 时钟相位 0:从第一个时钟边沿采样 1:从第二个时钟边沿采样
};

extern const TDrvSpiCfg g_drv_spi_cfgs[2];

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _DRV_SPI_CFG_H_
