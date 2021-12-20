/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_device
作者:   km
说明:   bms数据处理.
***************************************************************************************************/


/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "bms_device.h"
#include "bms_canhandle.h"
#include "bms_internal_handle.h"
#include "bms_com.h"
#include "tick_handle.h"
#include <string.h>
#include "sys_config.h"
#include "io_megg.h"
#include "alarm_task.h"


/***************************************************************************************************
定义:   全局变量.
***************************************************************************************************/
BmsDevice bmsDevice[2];
BmsExternalData bmsExternData[2];

void bms_charger_start(uint8_t gunSn)
{
    if (bmsDevice[gunSn].unBmsStateB.bit.bBmsComState)
    {
//        ERROR("bms正在通信中，启动失败");
        return;
    }
    
    bmsExternData[gunSn].unStateA.bit.bStartChargerCmdFlag = 1;
}

void bms_updata_handle(void)
{
	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    bmsExternData[gunSn].cc1bak = bmsExternData[gunSn].cc1State;
	    if(bmsExternData[gunSn].cc1State != sSysMegg[gunSn].CC1)
	    {
	        bmsExternData[gunSn].cc1State = sSysMegg[gunSn].CC1;
	        bmsExternData[gunSn].unStateA.bit.bChargerIsFlag = 0;
	        bmsExternData[gunSn].unStateA.bit.bGb2015ProtocolFlag = 0;
	    }
	}
}

void bms_device_charger_handle(void)
{
    bms_updata_handle();

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    if (bmsExternData[gunSn].unStateA.bit.bStartChargerCmdFlag)
	    {
	        bmsExternData[gunSn].unStateA.bit.bStartChargerCmdFlag = 0;
	        bms_start_charger_init(gunSn);
	    }
		
		bms_internal_handle_task_1ms(gunSn);
	}
    
    bms_charger_scheduler();
}

void bms_device_init(uint8_t gunSn)
{
    bmsDevice[gunSn].mainState = IDLE;
}

/**
  * @brief  外部的模块（如国网）则用此函数来获取BMS模块相关信息和数据
  */
const BmsFrameData* getBmsFrameDataHandle(uint8_t gunSn)
{
    return &bmsDevice[gunSn].frameData;
}

/**
  * @brief  外部的模块（如国网）则用此函数来获取BMS模块相关信息和数据
  */
const BmsDevice* getBmsDeviceHandle(uint8_t gunSn)
{
    return &bmsDevice[gunSn];
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/

