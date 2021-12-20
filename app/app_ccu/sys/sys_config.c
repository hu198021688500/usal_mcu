/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   sys_config
作者:   km
说明:   系统参数.
***************************************************************************************************/

#include <string.h>

#include "sys_config.h"
#include "charge_task.h"
#include"io_megg.h"
#include"log.h"

structSysConfig sSysConfig;

void sys_config_init(uint32_t sysConfigValidFlag)
{
    memset(&sSysConfig,0,sizeof(sSysConfig));
    strcpy(sSysConfig.sChar.sysSn,"6666666");

	XJ_LOG("校准[%d %d %d %d]",sSysConfig.sInt.iAGun_ScalSysPortVdcCali,    \
	sSysConfig.sInt.iAGun_ScalSysPortVdcZero,sSysConfig.sInt.iAGun_ScalModVdcCali,sSysConfig.sInt.iAGun_ScalModVdcZero);
	
    sSysConfig.sInt.validFlag = sysConfigValidFlag;
	sSysConfig.sInt.uPorjectType = 1;//;
    sSysConfig.sInt.uModuleType = MOD_TYPE_INFY_30KW;
	sSysConfig.sInt.unitModuleNum = 1;
    sSysConfig.sInt.uModNum = 8;
	sSysConfig.sInt.uGhMode = 2;

    sSysConfig.sInt.uModLoadValue = 10;
    sSysConfig.sInt.uGunNum = 1;
    sSysConfig.sInt.uGunType = 4;//2;测试无枪锁

    sSysConfig.sInt.bmsType = 5;

    sSysConfig.sInt.uMaxChargeVolt = 10000;


    sSysConfig.sInt.uMaxChargeCurr = 2500;            //最高允许充电电流
    sSysConfig.sInt.uDcUnderVolt = 450;              //欠压值
    sSysConfig.sInt.uDcOverVolt = 10100;               //过压值 
    sSysConfig.sInt.uDcoverCurr = 2600;               //过流值

    sSysConfig.sInt.uAcUnderVolt = 3040;              //输入欠压值
    sSysConfig.sInt.uAcOverVolt = 4560;               //输入过压值
    
    sSysConfig.sInt.uGunTempProtect = 950;           //充电枪过温值
	sSysConfig.sInt.uOutletTempUp_10x = 800;			//内部过温
	
	sSysConfig.sInt.uDoorProtectEn = 1;            //门禁保护
	sSysConfig.sInt.uBatProtectEn = 1;             //车辆电池保护
    sSysConfig.sInt.uAcContCheckEn = 1;
	sSysConfig.sInt.uBatStatueEn = 1;            
	
    sSysConfig.sInt.uMaxAllowChSOC = 101;
    sSysConfig.sInt.uModLimit = 100;
    sSysConfig.sInt.uInsdetRes = 100;

//    sys_debug_set(DEBUG1_TCU);
}

void set_sys_config_save(void)
{
    //sFlashSave.bit.para = 1;    //开始参数保存
}

void record_manage_sys_config_init(uint32_t sysConfigValidFlag)
{
	
}


void sys_debug_set(uint16_t de)
{
    sSysConfig.sTempInt.Debug1 = de;
}

uint16_t sys_debug_get(void)
{
    return sSysConfig.sTempInt.Debug1;
}

/**
  * @brief  两点校准
  * @param  iData -- y(用户输出实际电压值) iVoltPortData --x采集运算的电压值
  */
void vTwoPointAdjust(int32_t iData,int16_t iVoltPortData,int16_t iVoltModData,int16_t* iCaliPortData,int16_t* iZeroPortData,int16_t* iCaliModData,int16_t* iZeroModData)
{
    int32_t lYn,lXn,lK;
    int16_t iTemp;
    int16_t i;
    int16_t iZero;
    static int16_t iCaliFlag,iScalData[4];
    
//    lTemp1=((long)iData*4096)/iVoltPortData;

    if(iCaliFlag==0)
    {    
        iScalData[0]=iData;
        iScalData[1]=iVoltPortData;
        
        iScalData[2]=iData;
        iScalData[3]=iVoltModData;

        iCaliFlag=1;
    }
    else
    {
        lYn=iData-iScalData[0];
        //y2-y1
        lXn=iVoltPortData-iScalData[1];            //x2-x1
        if(lXn==0)
        {
            lXn=1;
        }
        lK=(lYn<<12)/lXn;                //k=(y2-y1)/(x2-x1)
        iTemp=(lK*iScalData[1])>>12;
        iZero=iScalData[0]-iTemp;

        iTemp=(lK*(*iCaliPortData))>>12;
        (*iCaliPortData)=_IQsat(iTemp, kCaliMax, kCaliMin);

        iTemp=iZero+(*iZeroPortData);
        (*iZeroPortData)=_IQsat(iTemp, kZeroMax, kZeroMin);
        
        lYn=iData-iScalData[2];                                    //y2-y1
        lXn=iVoltModData-iScalData[3];            //x2-x1
        if(lXn==0)
        {
            lXn=1;
        }                    
        lK=(lYn<<12)/lXn;                //k=(y2-y1)/(x2-x1)
        iTemp=(lK*iScalData[3])>>12;
        iZero=iScalData[2]-iTemp;

        iTemp=(lK*(*iCaliModData))>>12;
        (*iCaliModData)=_IQsat(iTemp, kCaliMax, kCaliMin);

        iTemp=iZero+(*iZeroModData);
        (*iZeroModData)=_IQsat(iTemp, kZeroMax, kZeroMin);

        iCaliFlag=0;
        for(i=0;i<4;i++)
        {
            iScalData[i]=0;
        }
    }
    
}

/**
  * @brief  电压校准
  * @param  idata -- (用户输出实际电压值)
  */
void voltage_cali(int32_t idata,uint8_t gunSn)
{
    static int16_t cali[2][4] = {0};
    static int8_t calicnt[2] = {0};
    if(0 == calicnt)
    {
        cali[gunSn][0] = sSysConfig.sInt.iAGun_ScalSysPortVdcCali[gunSn];
        cali[gunSn][1] = sSysConfig.sInt.iAGun_ScalSysPortVdcZero[gunSn];
        cali[gunSn][2] = sSysConfig.sInt.iAGun_ScalModVdcCali[gunSn];
        cali[gunSn][3] = sSysConfig.sInt.iAGun_ScalModVdcZero[gunSn];
        vTwoPointAdjust(idata,sSysMegg[gunSn].v_Gun,sSysMegg[gunSn].v_Mod,&cali[gunSn][0],&cali[gunSn][1],&cali[gunSn][2],&cali[gunSn][3]);
        calicnt[gunSn] = 1;
    }
    else
    {
        calicnt[gunSn] = 0;
        vTwoPointAdjust(idata,sSysMegg[gunSn].v_Gun,sSysMegg[gunSn].v_Mod,&cali[gunSn][0],&cali[gunSn][1],&cali[gunSn][2],&cali[gunSn][3]);
//        DLOG("gun%d:校准para[%d][%d][%d][%d]",gunSn,cali[gunSn][0],cali[gunSn][1],cali[gunSn][2],cali[gunSn][3]);      
//        sSysConfig.sInt.iAGun_ScalSysPortVdcCali[gunSn] = cali[gunSn][0];
//        sSysConfig.sInt.iAGun_ScalSysPortVdcZero[gunSn] = cali[gunSn][1];
//        sSysConfig.sInt.iAGun_ScalModVdcCali[gunSn] = cali[gunSn][2];
//        sSysConfig.sInt.iAGun_ScalModVdcZero[gunSn] = cali[gunSn][3];
        
		set_sys_config_save();

        XJ_LOG("gun%d:校准完成[%d %d %d %d]",gunSn,sSysConfig.sInt.iAGun_ScalSysPortVdcCali[gunSn],    \
        sSysConfig.sInt.iAGun_ScalSysPortVdcZero[gunSn],sSysConfig.sInt.iAGun_ScalModVdcCali[gunSn],sSysConfig.sInt.iAGun_ScalModVdcZero[gunSn]);
    
    }

}

/***************************************************************************************************
文件结束.
***************************************************************************************************/
