#include "dev_can.h"

TDevPowerConfig g_dev_power_cfg_[] =
{
    {&g_drv_port[kDrvPortNumF],  0, False, False, False},   ///< 电源输出-0
    {&g_drv_port[kDrvPortNumF],  1, False, False, False},   ///< 电源输出-1
    {&g_drv_port[kDrvPortNumE], 11, False, False, False},   ///< 电源输出-2
    {&g_drv_port[kDrvPortNumF],  2, False, False, False},   ///< 电源输出-3
    {&g_drv_port[kDrvPortNumF],  3, False, False, False},   ///< 电源输出-4
    {&g_drv_port[kDrvPortNumD],  2, False, False, False},   ///< 电源输出-5
    
};


