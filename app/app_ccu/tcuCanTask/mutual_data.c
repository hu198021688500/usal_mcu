/***************************************************************************************************   
Copyright @ 2020-20xx小桔科技

文件:   com_data
作者:   km
说明:   通信数据.
***************************************************************************************************/

#include <string.h>
#include "mutual_data.h"
#include "io_megg.h"

structComData sMutualData[2];

void mutual_sys_data(uint8_t gunSn)
{
    sMutualData[gunSn].sSysInfo.iOutletTemp_10x = sSysMegg[0].t_Outlet;
    sMutualData[gunSn].sSysInfo.iInletTemp_10x = sSysMegg[0].t_Inlet;
    sMutualData[gunSn].sSysInfo.iChGunTempMax_10x = sSysMegg[gunSn].t_Gun;
    sMutualData[gunSn].sSysInfo.iChGunTempP_10x = sSysMegg[gunSn].t_GunP;
    sMutualData[gunSn].sSysInfo.iChGunTempN_10x = sSysMegg[gunSn].t_GunN;
    
    sMutualData[gunSn].sSysInfo.iModVolt_10x = sSysMegg[gunSn].v_Mod;
    sMutualData[gunSn].sSysInfo.iSysVolt_10x = sSysMegg[gunSn].v_Gun;
    sMutualData[gunSn].sSysInfo.iSysCurr_10x = sSysMegg[gunSn].i_Gun;

    sMutualData[gunSn].sSysInfo.iVolt_Demand_10x = chargerMegg[gunSn].v_demand;
    sMutualData[gunSn].sSysInfo.iCurr_Demand_10x = chargerMegg[gunSn].i_demand_mod;

    sMutualData[gunSn].sSysInfo.iAcAVolt_10x = sSysMegg[gunSn].vab_10x/1.732;
    sMutualData[gunSn].sSysInfo.iAcBVolt_10x = sSysMegg[gunSn].vbc_10x/1.732;
    sMutualData[gunSn].sSysInfo.iAcCVolt_10x = sSysMegg[gunSn].vac_10x/1.732;
    sMutualData[gunSn].sSysInfo.iAcHz = sSysMegg[gunSn].hz_10x;
    //sComData[gunSn].sSysInfo.iMeterE_1000x = dlt645Megg.meter.energyPower_1000x;

    sMutualData[gunSn].sSysInfo.uInsDetRes[0] = chargerMegg[gunSn].record.insdetRes;
    sMutualData[gunSn].sSysInfo.uInsDetRes[1] = chargerMegg[gunSn].record.insdetRes2;
    sMutualData[gunSn].sSysInfo.iInsDetPosVolt[0] = chargerMegg[gunSn].record.GFDPosVdc;
    sMutualData[gunSn].sSysInfo.iInsDetPosVolt[1] = chargerMegg[gunSn].record.GFDPosVdc2;
    sMutualData[gunSn].sSysInfo.iInsDetNegVolt[0] = chargerMegg[gunSn].record.GFDNegVdc;
    sMutualData[gunSn].sSysInfo.iInsDetNegVolt[1] = chargerMegg[gunSn].record.GFDNegVdc2;

    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bAcConTSt = get_Out_IO(ENUM_OUT_ACCONT,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bDcConTSt = get_Out_IO(ENUM_OUT_DCCONT,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bMidConTSt1 = get_Out_IO(ENUM_OUT_MIDCONT1,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bMidConTSt2 = get_Out_IO(ENUM_OUT_MIDCONT2,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bMidConTSt3 = get_Out_IO(ENUM_OUT_MIDCONT3,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bGunLockSt = sSysMegg[gunSn].unOut.bit.bGunLock;
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bFanSt = get_Out_IO(ENUM_OUT_FAN,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bBmsAxSt = get_Out_IO(ENUM_OUT_BMS_V,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bSwSt = chargerMegg[gunSn].flag.bit.bStartCh;

    if(get_insdet_state(gunSn) == INSDET_STATE_IDLE)
    {
        sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bInsDetSt = 0;
    }
    else if(get_insdet_state(gunSn) == INSDET_STATE_OVER)
    {
        sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bInsDetSt = 2;
    }
    else
    {
        sMutualData[gunSn].sSysInfo.unFlagCtrlSt.bit.bInsDetSt = 1;
    }
        
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bAcConTSt = get_In_IO(ENUM_IN_ACCONT,0);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bDcConTSt = get_DcCont_State(gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bMidConTSt1 = get_In_IO(ENUM_IN_MIDCONT1,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bMidConTSt2 = get_In_IO(ENUM_IN_MIDCONT2,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bMidConTSt3 = get_In_IO(ENUM_IN_MIDCONT3,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bEpoSt = get_In_IO(ENUM_IN_EPO,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bCC1St = get_In_IO(ENUM_IN_CC1,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bDoorSt = get_In_IO(ENUM_IN_DOOR,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bSPDSt = get_In_IO(ENUM_IN_SPD,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bGunToPileSt = 1-get_In_IO(ENUM_IN_GUNBACK,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bGunLockSt = get_In_IO(ENUM_IN_GUNLOCK,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bFuseSt = get_In_IO(ENUM_IN_FUSE,gunSn);
    sMutualData[gunSn].sSysInfo.unFlagBackSt.bit.bBreakerSt = get_In_IO(ENUM_IN_BREAKER,0);


}

/*获取当前充电停止原因*/
uint32_t mutual_get_current_stopcause(uint8_t gunSn)              
{
	return(get_charger_stopcause(gunSn));
}


BOOL tcu_start_charger_interface(uint8_t gunSn)
{
	return start_charger_interface(START_BY_TCU,gunSn);

}

void tcu_set_charger_stopcause(uint32_t stopCause,uint8_t gunSn)
{
    set_charger_stopcause(stopCause,gunSn);
}

void mutual_charge_data(uint8_t gunSn)
{

}

void mutual_mod_data(uint8_t gunSn)
{

}

void mutual_other_data(uint8_t gunSn)
{

}

void mutual_data_init(void)
{
	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    sMutualData[gunSn].sofewareVer = 1;
	    memset(&sMutualData[gunSn],0,sizeof(sMutualData[gunSn]));
	    //sMutualData[gunSn].sChargeInfo = &chargerMegg[gunSn].record;
	    sMutualData[gunSn].sBmsInfo = &bmsDevice[gunSn].frameData;
		sMutualData[gunSn].uAlarmA.all = sAlarmMegg.uAlarmA.all;
		sMutualData[gunSn].uAlarmB.all = sAlarmMegg.uAlarmB.all;
		sMutualData[gunSn].uAlarmC.all = sAlarmMegg.uAlarmC[gunSn].all;
	}
}

void mutual_data_deal(void)
{
	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
        mutual_sys_data(gunSn);
        mutual_mod_data(gunSn);
        mutual_other_data(gunSn);
	}
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/
