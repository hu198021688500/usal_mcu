/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_com
作者:   km
说明:   bms数据处理.
***************************************************************************************************/

#ifndef __BMS_COM_H__                     //防重包含.
#define __BMS_COM_H__
/**************************************************************************************************/

/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "base_type.h"
#include "bms_device.h"

#define GB_BMS_PF_CRM                             0x01    //充电机辨识
#define GB_BMS_PF_BRM                             0x02    //BMS和车辆辨识 (多包)

#define GB_BMS_PF_BCP                             0x06    //电池充电参数 (多包)
#define GB_BMS_PF_CTS                             0x07    //充电机时间同步
#define GB_BMS_PF_CML                             0x08    //充电机最大输出能力
#define GB_BMS_PF_BRO                             0x09    //BMS完成充电准备
#define GB_BMS_PF_CRO                             0x0A    //充电机完成充电准备

#define GB_BMS_PF_BCL                             0x10    //电池充电需求
#define GB_BMS_PF_BCS                             0x11    //电池充电总状态 (多包)
#define GB_BMS_PF_CCS                             0x12    //充电机充电状态
#define GB_BMS_PF_BSM                             0x13    //电池状态信息

#define GB_BMS_PF_BMV                             0x15    //单体动力蓄电池电压 (多包)
#define GB_BMS_PF_BMT                             0x16    //动力蓄电池温度 (多包)
#define GB_BMS_PF_BSP                             0x17    //动力蓄电池预留报文 (多包)
#define GB_BMS_PF_BST                             0x19    //BMS停止充电
#define GB_BMS_PF_CST                             0x1A    //充电机停止充电

#define GB_BMS_PF_BSD                             0x1C    //BMS统计
#define GB_BMS_PF_CSD                             0x1D    //充电机统计

#define GB_BMS_PF_BEM                             0x1E    //BMS错误
#define GB_BMS_PF_CEM                             0x1F    //充电机错误

#define GB_CH_PF_CHM                              0x26    //充电机握手报文
#define GB_BMS_PF_BHM                             0x27    //充电机握手报文

//HARDWARE_CHECK_MODE
#define GB_BMS_PF_BCM                             0x41    //BMS发送的工装信息报文
#define GB_CH_PF_CCM                              0x40    //充电桩回复的工装需求报文


#define PT_BMS_PF_CRM                             0x01    //充电机辨识
#define PT_BMS_PF_BRM                             0x02    //BMS和车辆辨识 (多包)

#define PT_BMS_PF_BCP                             0x06    //电池充电参数 (多包)
#define PT_BMS_PF_CTS                             0x07    //充电机时间同步
#define PT_BMS_PF_CML                             0x08    //充电机最大输出能力
#define PT_BMS_PF_BRO                             0x09    //BMS完成充电准备
#define PT_BMS_PF_CRO                             0x0A    //充电机完成充电准备

#define PT_BMS_PF_BCL                             0x10    //电池充电需求
#define PT_BMS_PF_BCS                             0x11    //电池充电总状态 (多包)
#define PT_BMS_PF_CCS                             0x14    //充电机充电状态
#define PT_BMS_PF_BSM                             0x12    //电池状态信息

#define PT_BMS_PF_BMV                             0x15    //单体动力蓄电池电压 (多包)
#define PT_BMS_PF_BMT                             0x16    //动力蓄电池温度 (多包)
#define PT_BMS_PF_BSP                             0x17    //动力蓄电池预留报文 (多包)
#define PT_BMS_PF_BST                             0x19    //BMS停止充电
#define PT_BMS_PF_CST                             0x1A    //充电机停止充电

#define PT_BMS_PF_BSD                             0x1C    //BMS统计
#define PT_BMS_PF_CSD                             0x1D    //充电机统计

#define PT_BMS_PF_BEM                             0x1E    //BMS错误
#define PT_BMS_PF_CEM                             0x1F    //充电机错误

/***************************************************************************************************
申明:   公有变量.
***************************************************************************************************/


/***************************************************************************************************
申明:   公有方法.
***************************************************************************************************/
void bms_start_charger_init(uint8_t gunSn);
void bms_charger_scheduler(void);

#endif  //__BMS_COM_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/


