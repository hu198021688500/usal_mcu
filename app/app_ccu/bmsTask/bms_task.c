/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_task
作者:   km
说明:   bms数据处理.
***************************************************************************************************/


/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "bms_task.h"
#include "bms_device.h"
#include "bms_canhandle.h"
#include "sys_config.h"
#include "fun.h"
#include "can_tx_rx_list.h"
#include "alarm_task.h"


/***************************************************************************************************
定义:   全局变量.
***************************************************************************************************/
unionSysClockFlag unBmsTaskClockFlag;
structSysClockCnt sBmsTaskClockCnt;   

#if 0
void bms_can2_error_judge(void)
{
 //   static unsigned int occurTime = 0;
    
    if (bmsDevice.mainState != SHAKE_HAND)
    {
        return;
    }
/*    else if(0 == get_sys_alarm_bit_value(SYS_ALARM_CAN2_ERROR_COM_BIT_INDEX_104))
    {
        return;
    }
    
    if (unBmsTaskClockFlag.bit.b1s)
    {
        uint8_t errorCnt = CAN_GetLSBTransmitErrorCounter(CAN2);
        if (errorCnt > 100)
        {
            const int maxErrorTime = 60*5; 
            
            if(sSysRamIntf.unSysFlag.bit.bSysbChargeFlag)
            {
                occurTime = get_poweron_times();
            }
            
            if(((get_poweron_times() - occurTime) < maxErrorTime))
            {
                set_sys_alarm_bit_value(SYS_ALARM_CAN2_ERROR_COM_BIT_INDEX_104,1);
            }
            else
            {
                set_sys_alarm_bit_value(SYS_ALARM_CAN2_ERROR_COM_BIT_INDEX_104,0);
            }
        }
        else
        {
            set_sys_alarm_bit_value(SYS_ALARM_CAN2_ERROR_COM_BIT_INDEX_104,0);
            occurTime = get_poweron_times();
        }
    }
*/    
}
#endif

void BMSTaskInit(void)
{
	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
   	    bmsDevice[gunSn].bmsType = sSysConfig.sInt.bmsType;
		bms_device_init(gunSn);
	}
}

void BMSTaskMain(void)
{
    sys_clock_handle(&sBmsTaskClockCnt,&unBmsTaskClockFlag);
    canhandle_task_1ms();
    bms_device_charger_handle();
    //bms_can2_error_judge();
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/

