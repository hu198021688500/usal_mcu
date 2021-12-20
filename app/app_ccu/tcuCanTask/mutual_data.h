/***************************************************************************************************   
Copyright @ 2020-20xx小桔科技

文件:   com_data
作者:   km
说明:   通信数据.
***************************************************************************************************/


#ifndef __MUTUAL_DATA_H__                     //防重包含.
#define __MUTUAL_DATA_H__

#include "sys_config.h"
#include "bms_device.h"
#include "alarm_task.h"
#include "charge_task.h"


typedef union 
{
    uint16_t all;
    struct 
    {
        uint16_t    bAcConTSt       :1;       //AC接触器控制状态
        uint16_t    bDcConTSt       :1;       //DC接触器控制状态
        uint16_t    bMidConTSt1     :1;       //中间接触器第一组控制状态
        uint16_t    bMidConTSt2     :1;       //中间接触器第二组控制状态  
		
        uint16_t    bMidConTSt3     :1;       //中间接触器第三组控制状态  
        uint16_t    bGunLockSt      :1;       //电子锁控制状态  
        uint16_t    bFanSt          :1;       //风扇控制状态
        uint16_t    bBmsAxSt        :1;       //bms辅助电源控制状态
		
        uint16_t    bSwSt           :1;       //开关机状态：0关机 1开机
        uint16_t    bInsDetSt       :2;       //绝缘状态

        uint16_t    brsv            :5;    
    }bit;
}unionFlagCtrlSt;//控制状态

typedef union 
{
    uint16_t all;
    struct 
    {
        uint16_t    bAcConTSt       :1;         //AC接触器反馈
        uint16_t    bDcConTSt       :1;         //DC接触器反馈
        uint16_t    bMidConTSt1     :1;         //中间接触1反馈
        uint16_t    bMidConTSt2     :1;         //中间接触2反馈

        uint16_t    bMidConTSt3     :1;         //中间接触3反馈
        uint16_t    bEpoSt          :1;         //急停反馈       
        uint16_t    bCC1St          :1;         //cc1
        uint16_t    bDoorSt         :1;         //门禁反馈
        uint16_t    bSPDSt          :1;         //防雷反馈

        uint16_t    bGunToPileSt    :1;         //枪归位反馈：0已归位
        uint16_t    bGunLockSt      :1;         //枪锁反馈
        uint16_t    bFuseSt         :1;         //熔断器反馈
        uint16_t    bBreakerSt      :1;         //断路器反馈
        
        uint16_t    rsv             :4;                       
    }bit;
}unionFlagBackSt;//反馈状态

typedef struct 
{
    unionFlagCtrlSt unFlagCtrlSt;  
    unionFlagBackSt unFlagBackSt;

    int16_t iAcAVolt_10x;           //A相电压
    int16_t iAcBVolt_10x;           //B相电压
    int16_t iAcCVolt_10x;           //C相电压
    int16_t iAcHz;                  //电网频率
    int32_t iMeterE_1000x;          //当前电表读数（tcu下发到ccu）

	int16_t uInsDetRes[2];          //绝缘正负电阻
	int16_t iInsDetPosVolt[2];      //绝缘正负投切正电压
	int16_t iInsDetNegVolt[2];      //绝缘正负投切负电压
	
	int16_t iCc1Vol_10x;			//导引电压
    int16_t iModVolt_10x;           //模块电压
    int16_t iSysVolt_10x;           //枪电压
    int16_t iSysCurr_10x;           //本枪电流  

    int16_t iVolt_Demand_10x;       //需求电压
    int16_t iCurr_Demand_10x;       //需求电流

    int16_t iOutletTemp_10x;        //出风口温度
    int16_t iInletTemp_10x;         //进风口温度
	int16_t iChGunTempMax_10x;      //当前正负最大枪温	
	int16_t iChGunTempP_10x;        //当前正枪温	
	int16_t iChGunTempN_10x;        //当前负枪温	

}structSysInfo;//系统信息

#if 0
typedef union
{
    uint16_t all;
    struct
    {
        uint16_t bEPO:1;               //急停故障
        uint16_t bBREAKER:1;           //输入断路器故障
        uint16_t bFUSE:1;              //输出熔断器故障      
        uint16_t bFAN:1;               //风扇故障
        uint16_t bSPD:1;               //避雷器故障
        uint16_t bOUTLETTEMPUP:1;      //充电桩过温故障
        uint16_t bVGRIDUP:1;           //输入过压故障
        uint16_t bVGRIDLOW:1;          //输入欠压故障

        uint16_t bDOOR:1;             //门禁故障
        uint16_t bACCONT:1;           //输入接触器故障
        uint16_t bAX:1;               //辅源故障---掉电保护
        uint16_t bMIDCONT:1;          //中间接触器故障   
		uint16_t bTCUCOM:1;           //TCU通信失败
        uint16_t bMODCOMFAULT:1;      //模块通信故障---所有模块失联,有别于上面
        uint16_t bMETER_COM:1;        //电表通信故障--预留
        uint16_t bOUTLETTEMPUP_2:1;   //充电桩过温告警---告警
    }bit;
}unionAlarmA;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t bMOD:1;              //模块故障
        uint16_t bMOD_AC:1;           //模块输入故障---告警
        uint16_t bMOD_ACUP:1;         //模块输入过压故障---告警
        uint16_t bMOD_ACLOW:1;        //模块输入欠压故障---告警
        uint16_t bMOD_ACLOSE:1;       //模块输入缺相故障---告警
        uint16_t bMOD_SHORT:1;        //模块输出短路故障---告警
        uint16_t bMOD_DCOI:1;         //模块输出过流故障---告警
        uint16_t bMOD_DCUP:1;         //模块输出过压故障---告警
        
        uint16_t bMOD_DCLOW:1;        //模块输出欠压故障---告警
        uint16_t bMOD_OT:1;           //模块过温故障---告警
        uint16_t bMOD_COM:1;          //模块通信故障---告警
        uint16_t bMOD_FAN:1;          //模块风扇故障---告警
        uint16_t bRSV0:1;             //预留
        uint16_t bRSV1:1;             //预留
        uint16_t bRSV2:1;             //预留
        uint16_t bRSV3:1;             //预留
    }bit;
}unionAlarmB;

typedef union
{
    uint16_t all;
    struct
    {
        uint16_t bDCCONT:1;            //直流接触器故障
        uint16_t bGunLock:1;           //充电枪锁故障，充电过程中检测枪锁状态用此,标准机型无
        uint16_t bINSDETFAIL:1;        //绝缘故障
        uint16_t bVDCINV:1;            //电池反接故障
        uint16_t bCC1:1;               //控制导引故障
        uint16_t bGUNTEMPUP:1;         //充电枪过温故障
        uint16_t bGUNNOTBACK:1;        //充电枪未归位---告警
        uint16_t bBMS_COM:1;           //BMS通信故障,标准机型无
        
        uint16_t bVDCUP:1;            //直流过压故障
        uint16_t bVDCLOW:1;           //直流欠压故障
        uint16_t bIDCUP:1;            //直流过流故障
        uint16_t bINSDETABS:1;        //绝缘监测告警---告警
        uint16_t bGUNTEMPUP_2:1;      //充电枪过温告警---告警
        uint16_t bShort:1;             //输出短路故障
        uint16_t bDISCHARGE:1;        //泄放回路故障,标准机型无
		uint16_t bRSV0:1;             //预留
    }bit;
}unionAlarmC;

typedef struct 
{
    uint32_t originalCode;      //停止码ENUM_ALARM_STOP
    char *tip;               //显示、记录等
    ENUM_ALARM_TYPE alarmtype;  //alarm等级
    
}StructStopCodeMap;

typedef enum
{ 
    BOOL_FALSE = 0,
    BOOL_TRUE = 1,
}BOOL;

typedef union 
{
    uint16_t all;
    struct
    {
        uint16_t idle           :1;  //空闲
        uint16_t starting       :1;  //启动中    
        uint16_t startok        :1;  //启动成功
        uint16_t startfail      :1;  //启动失败
        uint16_t charging       :1;  //充电中  
        uint16_t over           :1;  //充电结束,充电状态机结束5s后清零
        uint16_t fault          :1;  //故障
        uint16_t bworking       :1;  //启动或充电中

        uint16_t insdet         :1;  //1 绝缘检测中
    }bit;
}UNION_SYS_CHARGE_STEP;

#endif

typedef struct 
{    
    structSysInfo sSysInfo;
    //ChargerSaveData *sChargeInfo;
    BmsFrameData *sBmsInfo;
	
    unionAlarmA uAlarmA;     //ENUM_ALARM_STOP 1-16
    unionAlarmB uAlarmB;     //ENUM_ALARM_STOP 17-32
    unionAlarmC uAlarmC;      //ENUM_ALARM_STOP 33-48:1枪  49-64:2枪 
	UNION_SYS_CHARGE_STEP charge_stage;
    unionChargerDeviceFlag flag;
    uint16_t sofewareVer;   
    
}structComData;


extern structComData sMutualData[2];//仅用作输出显示内容

uint32_t mutual_get_current_stopcause(uint8_t gunSn); 
BOOL tcu_start_charger_interface(uint8_t gunSn);
void tcu_set_charger_stopcause(uint32_t stopCause,uint8_t gunSn);




void mutual_data_init(void);
void mutual_data_deal(void);

#endif  //__DWIN_DATA_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/
