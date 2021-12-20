/**
 * @file dev_config.h
 * @brief 设备节点
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

#ifndef _SFC_DEV_CONFIG_H_
#define _SFC_DEV_CONFIG_H_

#include <ofc_types.h>
#ifdef __cplusplus
extern "C" {
#endif

/*
 * 主设备号(0~255)  
 * @note:有子设备操作子设备, 主设备号由驱动操作
 */
typedef enum TSfDevNumTag    TSfDevNum;
enum TSfDevNumTag
{
    kSfDevNumPmu                = 0x0000,            ///< pmu电源管理
    kSfDevNumWdog               = 0x0100,            ///< 看门狗设备
    kSfDevNumPower              = 0x0200,            ///< 电源输出  (应用操作子设备)
    kSfDevNumSwitch             = 0x0300,            ///< 开关量设备(应用操作子设备)
    kSfDevNumAdc                = 0x0400,            ///< ADC设备   (应用操作子设备)
    kSfDevNumFlash              = 0x0500,            ///< Flash设备 (应用操作子设备)
    kSfDevNumLog                = 0x0600,            ///< 串口日志设备
    kSfDevNumRs485              = 0x0700,            ///< Rs485设备 (应用操作子设备)
    kSfDevNumCan                = 0x0800,            ///< CAN设备   (应用操作子设备)
    kSfDevNumPwm                = 0x0900,            ///< Pwm设备   (应用操作子设备)
    
    kSfDevMax,
};
/*
 * CAN子设备  
 */
typedef enum TSfDevCanTag    TSfDevCan;
enum TSfDevCanTag
{
    kSfDevNumCan0     = kSfDevNumCan,    ///< can设备-can0
    kSfDevNumCan1,                       ///< can设备-can1
    kSfDevNumCan2,                       ///< can设备-can2
    TSfDevNumCanMax,
};
/*
 * can设备子设备命令
 */
typedef enum TSfDevNumCanIoctlCmdTag    TSfDevNumCanIoctlCmd;
enum TSfDevNumCanIoctlCmdTag
{
    /// 波特率设置会自动重启can
    kSfDevNumCanIoctlCmdSetBaud         = 0,      ///< can设备控制命令-设置波特率(byte0~3:波特率值(250000, 500000, 125000, 100000, 1000000))

    kSfDevNumCanIoctlCmdMax,
};

/*
 * 电源输出子设备  
 */
typedef enum TSfDevNumPowerTag    TSfDevNumPower;
enum TSfDevNumPowerTag
{
    kSfDevNumPowerDo0     = kSfDevNumPower,    ///< 电源输出-0
    kSfDevNumPowerDo1,                				 ///< 电源输出-1
    kSfDevNumPowerDo2,                         ///< 电源输出-2
    kSfDevNumPowerDo3,                     		 ///< 电源输出-3
    kSfDevNumPowerDo4,                      	 ///< 电源输出-4
    kSfDevNumPowerDo5,                      	 ///< 电源输出-5
	
    TSfDevNumPowerMax,
};

/*
 * 开关量设备子设备 
 */
typedef enum TSfDevNumSwitchTag    TSfDevNumSwitch;
enum TSfDevNumSwitchTag
{
    kSfDevNumSwitchDi0          = kSfDevNumSwitch,    ///< 开关量设备-0
    kSfDevNumSwitchDi1,                               ///< 开关量设备-1
    kSfDevNumSwitchDi2,                               ///< 开关量设备-2
    kSfDevNumSwitchDi3,                               ///< 开关量设备-3
    kSfDevNumSwitchDi4,                               ///< 开关量设备-4
    kSfDevNumSwitchDi5,                               ///< 开关量设备-5
    
    TSfDevNumSwitchMax,
};

/*
 * ADC设备子设备 
 */
typedef enum TSfDevNumAdcTag    TSfDevNumAdc;
enum TSfDevNumAdcTag
{
    kSfDevNumAdc0					     = kSfDevNumAdc,        ///< Adc设备-0
    kSfDevNumAdc1,                                    ///< Adc设备-1
    kSfDevNumAdc2,                                    ///< Adc设备-2
    kSfDevNumAdc3,                                    ///< Adc设备-3
    kSfDevNumAdc4,                                    ///< Adc设备-4
    kSfDevNumAdc5,                                    ///< Adc设备-5
    
    TSfDevNumAdcMax,
};

/*
 * Flash设备子设备
 */
typedef enum TSfDevNumFlashTag    TSfDevNumFlash;
enum TSfDevNumFlashTag
{
    kSfDevNumFlashInfo         = kSfDevNumFlash,                 ///< Flash设备-升级信息区
    kSfDevNumFlashAppBack,                                       ///< Flash设备-APP备份区域
    
    TSfDevNumFlashMax,
};
/*
 * Flash设备子设备命令
 */
typedef enum TSfDevNumFlashIoctlCmdTag    TSfDevNumFlashIoctlCmd;
enum TSfDevNumFlashIoctlCmdTag
{
    kSfDevNumFlashIoctlCmdErasePage         = 0,           ///< Flash控制命令-擦除片区(byte0~3:擦除地址 byte4~byte7:擦除大小)
    kSfDevNumFlashIoctlCmdWriteAddr,                       ///< Flash控制命令-设置写地址(byte0~3:设置地址)
    kSfDevNumFlashIoctlCmdReadAddr,                        ///< Flash控制命令-设置读地址(byte0~3:设置地址)

    kSfDevNumFlashIoctlCmdMax,
};

/*
 * Pwm设备子设备(占空比0~100%)
 */
typedef enum TSfDevNumPwmTag    TSfDevNumPwm;
enum TSfDevNumPwmTag
{
    kSfDevNumPwm1         = kSfDevNumPwm,                 ///< Pwm设备-pwm1

    kSfDevNumPwmMax,
};

/*
 * Rs485设备子设备 
 */
typedef enum TSfDevNumRs485Tag    TSfDevNumRs485;
enum TSfDevNumRs485Tag
{
    kSfDevNumPRs485_1         = kSfDevNumRs485,               ///< Rs485设备-第一路
    kSfDevNumPRs485_2,                                        ///< Rs485设备-第二路

    kSfDevNumRs485Max,
};
/*
 * can设备子设备命令
 */
typedef enum TSfDevNumRs485IoctlCmdTag    TSfDevNumRs485IoctlCmd;
enum TSfDevNumRs485IoctlCmdTag
{
    /// 波特率设置会自动重启can
    kSfDevNumRs485IoctlCmdSetBaud         = 0,      ///< uart设备控制命令-设置波特率(byte0~3:波特率值(9600, 115200))

    kSfDevNumRs485IoctlCmdMax,
};

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // _SFC_DEV_CONFIG_H_
