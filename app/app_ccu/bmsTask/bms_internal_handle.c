/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_internal_handle
作者:   km
说明:   bms数据处理.
***************************************************************************************************/


/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "bms_internal_handle.h"//对外接口.
#include "bms_device.h"


/***************************************************************************************************
定义:   全局变量.
***************************************************************************************************/
StructBmsComFrameInfo bmsComFrame[2];
BmsComInternalData bmsComInternalData[2];

void bms_internal_handle_init(void)
{
	for(uint8_t gunSn = 0;gunSn < 2;gunSn++)
	{
    	bms_internal_handle_start_charger_init(gunSn);
	}
}

void bms_internal_handle_start_charger_init(uint8_t gunSn)
{
    bmsComInternalData[gunSn].cstIndex = 0;
    bmsComInternalData[gunSn].cstValue = 0;
    
    bmsComFrame[gunSn].chm26.unFrameFlag.all = 0;
    bmsComFrame[gunSn].crm01.unFrameFlag.all = 0;
    bmsComFrame[gunSn].cts07.unFrameFlag.all = 0;
    bmsComFrame[gunSn].cml08.unFrameFlag.all = 0;
    bmsComFrame[gunSn].cro0A.unFrameFlag.all = 0;
    bmsComFrame[gunSn].ccs12.unFrameFlag.all = 0;
    bmsComFrame[gunSn].cst1A.unFrameFlag.all = 0;
    bmsComFrame[gunSn].csd1D.unFrameFlag.all = 0;
    bmsComFrame[gunSn].cem1F.unFrameFlag.all = 0;
}

void bms_internal_1ms_decrement_task(uint8_t gunSn)
{
    if (bmsComFrame[gunSn].chm26.sendTime > 0)
    {
        bmsComFrame[gunSn].chm26.sendTime--;
    }
    
    if (bmsComFrame[gunSn].chm26.maxSendTime > 0)
    {
        bmsComFrame[gunSn].chm26.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].crm01.sendTime > 0)
    {
        bmsComFrame[gunSn].crm01.sendTime--;
    }
    
    if (bmsComFrame[gunSn].crm01.maxSendTime > 0)
    {
        bmsComFrame[gunSn].crm01.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].cts07.sendTime > 0)
    {
        bmsComFrame[gunSn].cts07.sendTime--;
    }
    
    if (bmsComFrame[gunSn].cts07.maxSendTime > 0)
    {
        bmsComFrame[gunSn].cts07.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].cml08.sendTime > 0)
    {
        bmsComFrame[gunSn].cml08.sendTime--;
    }
    
    if (bmsComFrame[gunSn].cml08.maxSendTime > 0)
    {
        bmsComFrame[gunSn].cml08.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].cro0A.sendTime > 0)
    {
        bmsComFrame[gunSn].cro0A.sendTime--;
    }
    
    if (bmsComFrame[gunSn].cro0A.maxSendTime > 0)
    {
        bmsComFrame[gunSn].cro0A.maxSendTime--;
    }
    
    
    if (bmsComFrame[gunSn].ccs12.sendTime > 0)
    {
        bmsComFrame[gunSn].ccs12.sendTime--;
    }
    
    if (bmsComFrame[gunSn].ccs12.maxSendTime > 0)
    {
        bmsComFrame[gunSn].ccs12.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].cst1A.sendTime > 0)
    {
        bmsComFrame[gunSn].cst1A.sendTime--;
    }
    
    if (bmsComFrame[gunSn].cst1A.maxSendTime > 0)
    {
        bmsComFrame[gunSn].cst1A.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].csd1D.sendTime > 0)
    {
        bmsComFrame[gunSn].csd1D.sendTime--;
    }
    
    if (bmsComFrame[gunSn].csd1D.maxSendTime > 0)
    {
        bmsComFrame[gunSn].csd1D.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].cem1F.sendTime > 0)
    {
        bmsComFrame[gunSn].cem1F.sendTime--;
    }
    
    if (bmsComFrame[gunSn].cem1F.maxSendTime > 0)
    {
        bmsComFrame[gunSn].cem1F.maxSendTime--;
    }
    
    if (bmsComFrame[gunSn].waitBhm27Time > 0)
    {
        bmsComFrame[gunSn].waitBhm27Time--;
    }
    
    if (bmsComFrame[gunSn].waitBrm02Time > 0)
    {
        bmsComFrame[gunSn].waitBrm02Time--;
    }
    
    if (bmsComFrame[gunSn].waitBcp06Time > 0)
    {
        bmsComFrame[gunSn].waitBcp06Time--;
    }
    
    if (bmsComFrame[gunSn].waitBro09Time > 0)
    {
        bmsComFrame[gunSn].waitBro09Time--;
    }
    
    if (bmsComFrame[gunSn].waitBro09OkTime > 0)
    {
        bmsComFrame[gunSn].waitBro09OkTime--;
    }
    
    if (bmsComFrame[gunSn].waitBcl10Time > 0)
    {
        bmsComFrame[gunSn].waitBcl10Time--;
    }
    
    if (bmsComFrame[gunSn].waitBcs11Time > 0)
    {
        bmsComFrame[gunSn].waitBcs11Time--;
    }
    
    if (bmsComFrame[gunSn].waitBsm13Time > 0)
    {
        bmsComFrame[gunSn].waitBsm13Time--;
    }
    
    if (bmsComFrame[gunSn].waitBst19Time > 0)
    {
        bmsComFrame[gunSn].waitBst19Time--;
    }
    
    if (bmsComFrame[gunSn].waitBsd1DTime > 0)
    {
        bmsComFrame[gunSn].waitBsd1DTime--;
    }
    
	//HARDWARE_CHECK_MODE
    if (bmsComFrame[gunSn].waitBcm41Time > 0)//等待接收
    {
        bmsComFrame[gunSn].waitBcm41Time--;
    }

    if (bmsComFrame[gunSn].ccm40.sendTime > 0)//等到发送
    {
        bmsComFrame[gunSn].ccm40.sendTime--;
    }  

    if (bmsDevice[gunSn].bmsData.croAASendCnt > 0)//等到发送
    {
        bmsDevice[gunSn].bmsData.croAASendCnt--;
    }  

    
}

void bms_internal_handle_task_1ms(uint8_t gunSn)
{
    bms_internal_1ms_decrement_task(gunSn);
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/

