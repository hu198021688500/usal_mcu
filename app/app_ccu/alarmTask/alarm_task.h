/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   alarm_stopcause
作者: km
说明:   alarm deal.
***************************************************************************************************/

#ifndef __ALARM_TASK_H_
#define __ALARM_TASK_H_

#include"base_type.h"

typedef enum 
{
    ALARM_STOP_NULL = 0,

    //故障+结束

	//系统故障
    ALARM_EPO,              //急停故障
    ALARM_BREAKER,          //输入断路器故障
    ALARM_FUSE,             //输出熔断器故障      
    ALARM_FAN,              //风扇故障
    ALARM_SPD,              //避雷器故障
    ALARM_OUTLETTEMPUP,     //充电桩过温故障
    ALARM_VGRIDUP,          //输入过压故障
    ALARM_VGRIDLOW,         //输入欠压故障

	ALARM_DOOR,             //门禁故障
	ALARM_ACCONT,           //输入接触器故障
    ALARM_AX,               //辅源故障---掉电保护
    ALARM_MIDCONT,          //中间接触器故障
    ALARM_TCUCOM,           //TCU通信故障
    ALARM_MODCOMFAULT,      //模块通信故障---所有模块失联,有别于上面
    ALARM_METER_COM,        //电表通信故障-预留
    ALARM_OUTLETTEMPUP_2,	//充电桩过温告警---告警
    
    ALARM_MOD,              //模块故障
    ALARM_MOD_AC,           //模块输入故障---告警
    ALARM_MOD_ACUP,         //模块输入过压故障---告警
    ALARM_MOD_ACLOW,        //模块输入欠压故障---告警
    ALARM_MOD_ACLOSE,       //模块输入缺相故障---告警
    ALARM_MOD_SHORT,        //模块输出短路故障---告警
    ALARM_MOD_DCOI,         //模块输出过流故障---告警
    ALARM_MOD_DCUP,         //模块输出过压故障---告警
    
    ALARM_MOD_DCLOW,        //模块输出欠压故障---告警
    ALARM_MOD_OT,           //模块过温故障---告警
    ALARM_MOD_COM,          //模块通信故障---告警
    ALARM_MOD_FAN,          //模块风扇故障---告警
    ALARM_RSV1, 			//预留1
	ALARM_RSV2,             //预留2
    ALARM_RSV3,             //预留3
	ALARM_RSV4, 			//预留4

    
	//枪1故障
    ALARM_DCCONT_GUN1,      //GUN1直流接触器故障
    ALARM_GunLock_GUN1, 	//GUN1充电枪锁故障
	ALARM_INSDETFAIL_GUN1,	//GUN1绝缘故障
	ALARM_VDCINV_GUN1,      //GUN1电池反接故障
    ALARM_CC1_GUN1,         //GUN1控制导引故障
    ALARM_GUNTEMPUP_GUN1,   //GUN1充电枪过温故障
    ALARM_GUNNOTBACK_GUN1,  //GUN1充电枪未归位---告警
    ALARM_BMS_COM_GUN1,     //GUN1-BMS通信故障
    
    ALARM_VDCUP_GUN1,       //GUN1直流过压故障-
    ALARM_VDCLOW_GUN1,      //GUN1直流欠压故障-
    ALARM_IDCUP_GUN1,       //GUN1直流过流故障-
	ALARM_INSDETABS_GUN1,	//GUN1绝缘监测告警---告警-
    ALARM_GUNTEMPUP_2_GUN1, //GUN1充电枪过温告警---告警-
	ALARM_SHORT_GUN1,		//GUN1输出短路故障
	ALARM_DISCHARGE_GUN1,   //GUN1泄放回路故障
	ALARM_RSV5, 			//预留5

	//枪2故障
	ALARM_DCCONT_GUN2,		//GUN2直流接触器故障
    ALARM_GunLock_GUN2,     //GUN2充电枪锁故障
	ALARM_INSDETFAIL_GUN2,	//GUN2绝缘故障
	ALARM_VDCINV_GUN2,		//GUN2电池反接故障
	ALARM_CC1_GUN2, 		//GUN2控制导引故障
	ALARM_GUNTEMPUP_GUN2,	//GUN2充电枪过温故障
	ALARM_GUNNOTBACK_GUN2,	//GUN2充电枪未归位---告警
	ALARM_BMS_COM_GUN2, 	//GUN2-BMS通信故障
	ALARM_VDCUP_GUN2,		//GUN2直流过压故障
    ALARM_VDCLOW_GUN2,      //GUN2直流欠压故障
	ALARM_IDCUP_GUN2,		//GUN2直流过流故障
	ALARM_INSDETABS_GUN2,	//GUN2绝缘监测告警---告警
	ALARM_GUNTEMPUP_2_GUN2, //GUN2充电枪过温告警---告警-
    ALARM_SHORT_GUN2,       //GUN2输出短路故障
	ALARM_DISCHARGE_GUN2,	//GUN2泄放回路故障
	ALARM_RSV6, 			//预留6
	


    ALARM_END_CODE,         //故障代码到此结束
    
    //如果是故障类增加在纯结束ALARM_END_CODE之前，并在unionAlarm
/**************************************************华丽的分割线**************************************/
    STOP_UNDEFINE = 299,                //未定义
    STOP_BY_CARD = 300,                 //刷卡停止
    STOP_BY_USER,                       //用户停止
    STOP_BY_ENERGY,                     //电量达到设定
    STOP_BY_TIME,                       //时间达到设定
    STOP_BY_MONEY,                      //金额达到设定
    STOP_BY_MONEY_NOTENOUGH,            //余额不足
    STOP_BY_BACKGROUP,                  //后台停止
    STOP_NET_DISCONNECT,                //后台断开连接
    STOP_VIN_ERROR,                     //VIN码不匹配
    STOP_BY_ADMIN,                      //管理员迪文界面中止
    STOP_INSDET_VBEFORE,                //绝缘检测阶段，枪口带压
    STOP_INSDET_TO1,                    //绝缘超时，模块未开机
    STOP_INSDET_TO2,                    //绝缘超时，模块开机
    STOP_BMS_VINSDET,                   //BMS绝缘电压低
    STOP_BCP_BATVOLAGE,                 //BCP电池电压异常
    STOP_SOC_PROTECTION,                //SOC达到用户设置值
    STOP_BMS_SINGLE_VUP,                //BMS单体电压过高
    STOP_BMS_TEMP_MAX,                  //BMS温度过高
    STOP_BMS_STAUE,                     //BMS状态异常-----
    STOP_BMS_BCSVOL,                    //BCS上传电压异常-----
    STOP_BSM_VSINGLE,                   //BMS 单体动力蓄电池电压
    STOP_BSM_SOC,                       //BMS 整车动力蓄电池荷电状态
    STOP_BSM_OC,                        //BMS 动力蓄电池充电过电流
    STOP_BSM_TEMPUP,                    //BMS 动力蓄电池温度过高
    STOP_BSM_INSDET,                    //BMS 动力蓄电池绝缘状态 
    STOP_BSM_CC,                        //BMS 蓄电池组输出连接器状态
    STOP_BSM_BATLOW,                    //BMS 单体动力蓄电池电压过低
    STOP_BSM_SOCLOW,                    //BMS 整车动力蓄电池SOC过低
    STOP_VOLTAGE_BMSMAX,                //充电电压超过BMS最大允许值
    STOP_VOLTAGE_BMSDEMAND,             //充电电压超过BMS需求值
    STOP_BAT_CIRCUIT,                   //车辆内部回路异常终止
    STOP_BAT_DEMAND_VOLTAGE,            //车辆需求电压大于允许
    STOP_BAT_DEMAND_CURRENT,            //车辆需求电流大于允许
    STOP_SOFTFAIL_MOD_START,            //软启失败,模块开机
    STOP_SOFTFAIL_MOD_UNSTART,          //软启失败,模块未开机
    STOP_SOFTFAIL_TIMEOUT,              //软启失败,启动超时

    STOP_TCU_UPDATE,                    //TCU升级
    STOP_TCU_CTRL_ABS,                  //TCU 控制板状态异常
    STOP_TCU_UNKNOWN,                   //TCU 未知的结束原因

    //BMS主动停止
    BMS_STOP_CAUSE_CHARGE_STARTUP_TIMEOUT_CRM =500,  //启动时间超时
    BMS_STOP_CAUSE_RECV_BRM_TIMEOUT,            //收到BRM报文超时
    BMS_STOP_CAUSE_RECV_BCP_TIMEOUT,            //收到BCP报文超时
    BMS_STOP_CAUSE_RECV_BRO_TIMEOUT,            //收到BR0报文超时
    BMS_STOP_CAUSE_RECV_BCS_TIMEOUT,            //收到BCS报文超时
    BMS_STOP_CAUSE_RECV_BCL_TIMEOUT,            //接收BCL报文超时
    BMS_STOP_CAUSE_RECV_BSM_TIMEOUT,            //接收BSM报文超时
    BMS_STOP_CAUSE_RECV_BEM_FRAME,              //收到BEM报文停止
    BMS_STOP_CAUSE_RECV_BST_TIMEOUT,            //收到BST报文超时
    BMS_STOP_CAUSE_BSM_CHARGER_ALLOW_TIMEOUT,   //BSM允许充电异常
    BMS_STOP_CAUSE_BRO_ABS,                     //BRO准备就绪异常
    BMS_STOP_CAUSE_RECV_BST_0_00,//BST的SOC目标值	
    BMS_STOP_CAUSE_RECV_BST_0_02,//BST的电压设定值	
    BMS_STOP_CAUSE_RECV_BST_0_04,//BST单体电压满	
    BMS_STOP_CAUSE_RECV_BST_0_06,//BST_00_6未定义	
    BMS_STOP_CAUSE_RECV_BST_1_00,//BST绝缘故障		
    BMS_STOP_CAUSE_RECV_BST_1_02,//BST连接器过温	
    BMS_STOP_CAUSE_RECV_BST_1_04,//BST元件过温		
    BMS_STOP_CAUSE_RECV_BST_1_06,//BST连接器故障	
    BMS_STOP_CAUSE_RECV_BST_2_00,//BST电池组过温	
    BMS_STOP_CAUSE_RECV_BST_2_02,//BST其他故障	
    BMS_STOP_CAUSE_RECV_BST_2_04,//BST_02_4未定义    
    BMS_STOP_CAUSE_RECV_BST_2_06,//BST_02_6未定义    
    BMS_STOP_CAUSE_RECV_BST_3_00,//BST电流过大		
    BMS_STOP_CAUSE_RECV_BST_3_02,//BST电压异常		
    BMS_STOP_CAUSE_RECV_BST_OHTER,//BST未知停机  
    
    STOP_MAX_CODE,
}ENUM_ALARM_STOP;

typedef enum
{
    ALARM_TYPE_NULL = 0,      //非
    ALARM_TYPE_ABS,           //告警不停机
    ALARM_TYPE_FAULT,         //系统故障 
	ALARM_TYPE_GUN1_FAULT,	  //GUN1故障 
	ALARM_TYPE_GUN2_FAULT,	  //GUN2故障 
}ENUM_ALARM_TYPE;

typedef enum
{
    GUN_SN_ONE = 0,
    GUN_SN_TWO,	
    GUN_SN_MAX, 
}ENUM_GUN_SN;



typedef struct 
{
    uint32_t originalCode;      //停止码ENUM_ALARM_STOP
    char *tip;               //显示、记录等
    ENUM_ALARM_TYPE alarmtype;  //alarm等级
    
}StructStopCodeMap;

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
    int32_t metercomcnt;
    int32_t modcomcnt;
    int32_t heartcnt;
    uint8_t ver;
    uint8_t alarmsource;		//是否故障源 0 是，1bu是

    unionAlarmA uAlarmA;//ENUM_ALARM_STOP 1-16
    unionAlarmB uAlarmB;//ENUM_ALARM_STOP 17-32
    unionAlarmC uAlarmC[2];//ENUM_ALARM_STOP 33-48:1枪  49-64:2枪 
}StructAlarmMegg;

extern StructAlarmMegg sAlarmMegg;

char* get_alarm_stopcause_tips(uint16_t index);
uint8_t get_alarm_statue(ENUM_ALARM_STOP index);
void set_alarm_statue(ENUM_ALARM_STOP index,uint8_t uset);
BOOL get_alarm_fast_response(void);

void alarm_init(void);
void alarm_main(void);

#endif

/***************************************************************************************************
文件结束.
***************************************************************************************************/
