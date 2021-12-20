
/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技
文件:   alarm_stopcause
作者:   km
说明: alarm deal.
***************************************************************************************************/

/******************************************************************************************
@note:
故障判断分为：
1、根据单板自身采集到的信息
2、根据终端主枪发送故障信息
3、根据1枪发送故障信息
4、分体机主机发送信息(暂无)

相关设置：sSysConfig.sInt.uSysType
******************************************************************************************/

#include<string.h>
#include "alarm_task.h"
#include "charge_task.h"
#include "sys_config.h"
#include "io_megg.h"
#include "log.h"
#include "time_cur.h"
#include "tick_handle.h"         
#include "dcharger_version.h"

StructAlarmMegg sAlarmMegg;
static uint8_t  alarmDataBitValue[ALARM_END_CODE]={0};

const StructStopCodeMap  alarmStopCauseCodeMap[]=
{
    {ALARM_STOP_NULL,       NULL,                       ALARM_TYPE_NULL},      

    //故障+结束    
    {ALARM_EPO,                "急停故障",                         ALARM_TYPE_FAULT},
    {ALARM_BREAKER,            "输入断路器故障",                      ALARM_TYPE_FAULT},
    {ALARM_FUSE,               "输出熔断器故障",                      ALARM_TYPE_FAULT},
    {ALARM_FAN,                "风扇故障",                         ALARM_TYPE_FAULT},
    {ALARM_SPD,                "避雷器故障",                        ALARM_TYPE_FAULT},
    {ALARM_OUTLETTEMPUP,       "充电桩过温故障",                      ALARM_TYPE_FAULT},
    {ALARM_VGRIDUP,            "输入过压故障",                       ALARM_TYPE_FAULT},
    {ALARM_VGRIDLOW,           "输入欠压故障",                       ALARM_TYPE_FAULT},

	{ALARM_DOOR,               "门禁故障",                         ALARM_TYPE_FAULT},
    {ALARM_ACCONT,             "输入接触器故障",                      ALARM_TYPE_FAULT},
    {ALARM_AX,                 "辅源掉电",                         ALARM_TYPE_FAULT},
    {ALARM_MIDCONT,            "中间接触器故障",                      ALARM_TYPE_FAULT},
	{ALARM_TCUCOM,			   "TCU通信失败",					   ALARM_TYPE_FAULT},
	{ALARM_MODCOMFAULT, 	   "模块通信故障",					   ALARM_TYPE_FAULT},
	{ALARM_METER_COM,		   "电表通信故障",					   ALARM_TYPE_FAULT},
	{ALARM_OUTLETTEMPUP_2,	   "充电桩过温告警", 					   ALARM_TYPE_ABS},
	
    {ALARM_MOD,                "模块故障",                         ALARM_TYPE_FAULT},
    {ALARM_MOD_AC,             "模块输入告警",                         ALARM_TYPE_ABS},
    {ALARM_MOD_ACUP,           "模块输入过压告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_ACLOW,          "模块输入欠压告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_ACLOSE,         "模块输入缺相告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_SHORT,          "模块输出短路告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_DCOI,           "模块输出过流告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_DCUP,           "模块输出过压告警",                       ALARM_TYPE_ABS},
    
    {ALARM_MOD_DCLOW,          "模块输出欠压告警",                       ALARM_TYPE_ABS},
    {ALARM_MOD_OT,             "模块过温告警",                         ALARM_TYPE_ABS},
    {ALARM_MOD_COM,            "模块通信告警",                         ALARM_TYPE_ABS},
    {ALARM_MOD_FAN,            "模块风扇告警",                         ALARM_TYPE_ABS},
    {ALARM_RSV1,               "预留1",                          ALARM_TYPE_FAULT},
    {ALARM_RSV2,               "预留2",                          ALARM_TYPE_FAULT},
    {ALARM_RSV3,               "预留3",                          ALARM_TYPE_FAULT},
	{ALARM_RSV4,			   "预留4",						   ALARM_TYPE_FAULT},

	//gun1
    {ALARM_DCCONT_GUN1,        "枪1直流接触器故障",               ALARM_TYPE_GUN1_FAULT},
    {ALARM_GunLock_GUN1,       "枪1充电枪锁故障",                ALARM_TYPE_GUN1_FAULT},

    {ALARM_INSDETFAIL_GUN1,    "枪1绝缘故障",                  ALARM_TYPE_GUN1_FAULT},
    {ALARM_VDCINV_GUN1,        "枪1电池反接故障",                ALARM_TYPE_GUN1_FAULT},
    {ALARM_CC1_GUN1,           "枪1控制导引故障",                ALARM_TYPE_GUN1_FAULT},
	{ALARM_GUNTEMPUP_GUN1,	   "枪1充电枪过温故障", 			  ALARM_TYPE_GUN1_FAULT},
	{ALARM_GUNNOTBACK_GUN1,    "枪1充电枪未归位",					     ALARM_TYPE_ABS},
	{ALARM_BMS_COM_GUN1,	   "枪1BMS通信故障",				  ALARM_TYPE_GUN1_FAULT},
	{ALARM_VDCUP_GUN1,		   "枪1直流过压故障",				  ALARM_TYPE_GUN1_FAULT},
	{ALARM_VDCLOW_GUN1, 	   "枪1直流欠压故障",				  ALARM_TYPE_GUN1_FAULT},
	{ALARM_IDCUP_GUN1,		   "枪1直流过流故障",				  ALARM_TYPE_GUN1_FAULT},
	{ALARM_INSDETABS_GUN1,	   "枪1绝缘监测告警",					     ALARM_TYPE_ABS},
	{ALARM_GUNTEMPUP_2_GUN1,   "枪1充电枪过温告警", 					 ALARM_TYPE_ABS},
	{ALARM_SHORT_GUN1,		   "枪1输出短路故障",				  ALARM_TYPE_GUN1_FAULT},
	{ALARM_DISCHARGE_GUN1,	   "枪1泄放回路故障",			      ALARM_TYPE_GUN1_FAULT},
	{ALARM_RSV5,			   "预留5",						   ALARM_TYPE_FAULT},

	//gun2
	{ALARM_DCCONT_GUN2,        "枪2直流接触器故障",               ALARM_TYPE_GUN2_FAULT},
    {ALARM_GunLock_GUN2,       "枪2充电枪锁故障",                ALARM_TYPE_GUN2_FAULT},
    {ALARM_INSDETFAIL_GUN2,    "枪2绝缘故障",                  ALARM_TYPE_GUN2_FAULT},
    {ALARM_VDCINV_GUN2,        "枪2电池反接故障",                ALARM_TYPE_GUN2_FAULT},
    {ALARM_CC1_GUN2,           "枪2控制导引故障",                ALARM_TYPE_GUN2_FAULT},
    {ALARM_GUNTEMPUP_GUN2,     "枪2充电枪过温故障",               ALARM_TYPE_GUN2_FAULT},
    {ALARM_GUNNOTBACK_GUN2,    "枪2充电枪未归位",                       ALARM_TYPE_ABS},
    {ALARM_BMS_COM_GUN2,       "枪2BMS通信故障",               ALARM_TYPE_GUN2_FAULT},
    {ALARM_VDCUP_GUN2,         "枪2直流过压故障",                ALARM_TYPE_GUN2_FAULT},
    {ALARM_VDCLOW_GUN2,        "枪2直流欠压故障",                ALARM_TYPE_GUN2_FAULT},
    {ALARM_IDCUP_GUN2,         "枪2直流过流故障",                       ALARM_TYPE_ABS},
    {ALARM_INSDETABS_GUN2,     "枪2绝缘监测告警",                       ALARM_TYPE_ABS},
    {ALARM_GUNTEMPUP_2_GUN2,   "枪2充电枪过温告警",                      ALARM_TYPE_ABS},
	{ALARM_SHORT_GUN2,		   "枪2输出短路故障",				  ALARM_TYPE_GUN2_FAULT},
    {ALARM_DISCHARGE_GUN2,	   "枪2泄放回路故障",				  ALARM_TYPE_GUN2_FAULT},
	{ALARM_RSV6,			   "预留6",						   ALARM_TYPE_FAULT},
    

    //纯结束
    {STOP_UNDEFINE,                      "未知停止",                            ALARM_TYPE_NULL},
    {STOP_BY_CARD,                       "刷卡停止",                            ALARM_TYPE_NULL},
    {STOP_BY_USER,                       "用户界面停止",                          ALARM_TYPE_NULL},
    {STOP_BY_ENERGY,                     "电量达到设定",                          ALARM_TYPE_NULL},
    {STOP_BY_TIME,                       "时间达到设定",                          ALARM_TYPE_NULL},
    {STOP_BY_MONEY,                      "金额达到设定",                          ALARM_TYPE_NULL},    
    {STOP_BY_MONEY_NOTENOUGH,            "余额不足",                            ALARM_TYPE_NULL},
    {STOP_BY_BACKGROUP,                  "后台停止",                            ALARM_TYPE_NULL},
    {STOP_NET_DISCONNECT,                "后台断开连接",                          ALARM_TYPE_NULL},
    {STOP_VIN_ERROR,                     "VIN码不匹配",                         ALARM_TYPE_NULL},
    {STOP_BY_ADMIN,                      "管理员界面中止",                         ALARM_TYPE_NULL},
    {STOP_INSDET_VBEFORE,                "绝缘检测阶段，枪口带压",                     ALARM_TYPE_NULL},
    {STOP_INSDET_TO1,                    "绝缘超时，模块未开机",                      ALARM_TYPE_NULL},
    {STOP_INSDET_TO2,                    "绝缘超时，模块开机",                       ALARM_TYPE_NULL},
    {STOP_BMS_VINSDET,                   "BMS绝缘电压低",                        ALARM_TYPE_NULL},
    {STOP_BCP_BATVOLAGE,                 "BCP电池电压异常",                       ALARM_TYPE_NULL},
    {STOP_SOC_PROTECTION,                "SOC达到用户设置值",                      ALARM_TYPE_NULL},
    {STOP_BMS_SINGLE_VUP,                "BMS单体电压过高",                       ALARM_TYPE_NULL},
    {STOP_BMS_TEMP_MAX,                  "BMS温度过高",                         ALARM_TYPE_NULL},
    {STOP_BMS_STAUE,                     "BMS状态异常",                         ALARM_TYPE_NULL},
    {STOP_BMS_BCSVOL,                    "BCS上传电压异常",                       ALARM_TYPE_NULL},
    {STOP_BSM_VSINGLE,                   "BMS 单体动力蓄电池电压",                   ALARM_TYPE_NULL},
    {STOP_BSM_SOC,                       "BMS 整车动力蓄电池荷电状态",                 ALARM_TYPE_NULL},
    {STOP_BSM_OC,                        "BMS 动力蓄电池充电过电流",                  ALARM_TYPE_NULL},
    {STOP_BSM_TEMPUP,                    "BMS 动力蓄电池温度过高",                   ALARM_TYPE_NULL},
    {STOP_BSM_INSDET,                    "BMS 动力蓄电池绝缘状态",                   ALARM_TYPE_NULL},
    {STOP_BSM_CC,                        "BMS 蓄电池组输出连接器状态",                 ALARM_TYPE_NULL},
    {STOP_BSM_BATLOW,                    "BMS 单体动力蓄电池电压过低",                 ALARM_TYPE_NULL},
    {STOP_BSM_SOCLOW,                    "BMS 整车动力蓄电池SOC过低",                ALARM_TYPE_NULL},
    {STOP_VOLTAGE_BMSMAX,                "充电电压超过BMS最大允许值",                  ALARM_TYPE_NULL},
    {STOP_VOLTAGE_BMSDEMAND,             "充电电压超过BMS需求值",                    ALARM_TYPE_NULL},
    {STOP_BAT_CIRCUIT,                   "车辆内部回路异常终止",                      ALARM_TYPE_NULL},
    {STOP_BAT_DEMAND_VOLTAGE,            "车辆需求电压大于允许",                      ALARM_TYPE_NULL},
    {STOP_BAT_DEMAND_CURRENT,            "车辆需求电流大于允许",                      ALARM_TYPE_NULL},
    {STOP_SOFTFAIL_MOD_START,            "软启失败,模块开机",                       ALARM_TYPE_NULL},
    {STOP_SOFTFAIL_MOD_UNSTART,          "软启失败,模块未开机",                      ALARM_TYPE_NULL},
    {STOP_SOFTFAIL_TIMEOUT,              "软启失败,超时",                         ALARM_TYPE_NULL},
    
    {STOP_TCU_UPDATE,                    "TCU升级",                           ALARM_TYPE_NULL},
    {STOP_TCU_CTRL_ABS,                  "TCU 控制板状态异常",                     ALARM_TYPE_NULL},
    {STOP_TCU_UNKNOWN,                   "TCU 未知的结束原因",                     ALARM_TYPE_NULL},

    //BMS主动停止
    {BMS_STOP_CAUSE_CHARGE_STARTUP_TIMEOUT_CRM,     "启动时间超时",               ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BRM_TIMEOUT,               "收到BRM报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BCP_TIMEOUT,               "收到BCP报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BRO_TIMEOUT,               "收到BR0报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BCS_TIMEOUT,               "收到BCS报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BCL_TIMEOUT,               "接收BCL报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BSM_TIMEOUT,               "接收BSM报文超时",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BEM_FRAME,                 "收到BEM报文停止",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_TIMEOUT,               "收到BST报文超时",            ALARM_TYPE_NULL},
    
    {BMS_STOP_CAUSE_BSM_CHARGER_ALLOW_TIMEOUT,      "BSM允许充电异常",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_BRO_ABS,                        "BRO准备就绪异常",            ALARM_TYPE_NULL},
    
    {BMS_STOP_CAUSE_RECV_BST_0_00,                  "BST的SOC目标值",           ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_0_02,                  "BST的电压设定值",            ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_0_04,                  "BST单体电压满",             ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_0_06,                  "BST_00_6未定义",          ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_1_00,                  "BST绝缘故障",              ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_1_02,                  "BST连接器过温",             ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_1_04,                  "BST元件过温",              ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_1_06,                  "BST连接器故障",             ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_2_00,                  "BST电池组过温",             ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_2_02,                  "BST其他故障",              ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_2_04,                  "BST_02_4未定义",          ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_2_06,                  "BST_02_6未定义",          ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_3_00,                  "BST电流过大",              ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_3_02,                  "BST电压异常",              ALARM_TYPE_NULL},
    {BMS_STOP_CAUSE_RECV_BST_OHTER,                 "BST未知停机",              ALARM_TYPE_NULL},

};

/**
  * @brief  停止原因对应Tip
  */
char* get_alarm_stopcause_tips(uint16_t index)
{    
    int32_t mid = 0;
    int32_t low = 0;
    int32_t high = (sizeof(alarmStopCauseCodeMap)/sizeof(StructStopCodeMap))-1;
    
    while(low <= high)
    {
        mid=(high+low)/2;

        if(alarmStopCauseCodeMap[mid].originalCode > index)
            high = mid - 1;
        else  if(alarmStopCauseCodeMap[mid].originalCode < index)
            low = mid + 1;
        else 
            return (char*)alarmStopCauseCodeMap[mid].tip;
    }
    DLOG("未找到alarmtip[%d]",index);
    return  NULL;
}

/**
  * @brief  获取告警状态
  * @param  ENUM_ALARM_STOP前半部分告警代码
  * @retval 0 正常 1 告警
  */
uint8_t get_alarm_statue(ENUM_ALARM_STOP index)
{
    if((index>=ALARM_END_CODE)||(index==ALARM_STOP_NULL))
    {
        return 0;
    }

    return alarmDataBitValue[index];
}

/**
  * @brief  设置告警状态
  * @param  ENUM_ALARM_STOP前半部分告警代码, uset 0 正常 1 告警
  */
void set_alarm_statue(ENUM_ALARM_STOP index,uint8_t uset)
{
    if((index>=ALARM_END_CODE)||(index==ALARM_STOP_NULL))
    {
        return;
    }

    int32_t para1 = 0;
    int32_t para2 = 0;
    int32_t para3 = 0;
    
	switch (index)
	{
		case ALARM_OUTLETTEMPUP:     //充电桩过温故障
        case ALARM_OUTLETTEMPUP_2:   //充电桩过温告警---告警
        {
            para1 = sSysMegg[0].t_Outlet;
        }
        break;

		case ALARM_GUNTEMPUP_GUN1:        //充电枪过温故障
        case ALARM_GUNTEMPUP_2_GUN1:      //充电枪过温告警---告警
		{
            para1 = sSysMegg[0].t_Gun;
		}
		break;

		case ALARM_GUNTEMPUP_GUN2:        //充电枪过温故障
        case ALARM_GUNTEMPUP_2_GUN2:      //充电枪过温告警---告警
		{
            para1 = sSysMegg[1].t_Gun;
		}
		break;
		
		case ALARM_VGRIDUP:          //输入过压故障
		case ALARM_VGRIDLOW:         //输入欠压故障
        {
            para1 = sSysMegg[0].vab_10x;
            para2 = sSysMegg[0].vbc_10x;
            para3 = sSysMegg[0].vac_10x;
        }
        break;
        
		case ALARM_VDCINV_GUN1:           //电池反接故障
		case ALARM_VDCUP_GUN1:            //直流过压故障
		case ALARM_VDCLOW_GUN1:           //直流欠压故障
		{
            para1 = sSysMegg[0].v_Gun;
		}
		break;

	    case ALARM_VDCINV_GUN2:           //电池反接故障
		case ALARM_VDCUP_GUN2:            //直流过压故障
		case ALARM_VDCLOW_GUN2:           //直流欠压故障
		{
            para1 = sSysMegg[1].v_Gun;
		}
		break;
				
		case ALARM_IDCUP_GUN1:            //直流过流故障
		{
            para1 = sSysMegg[0].i_Gun;
		}
		break;

		case ALARM_IDCUP_GUN2:            //直流过流故障
		{
            para1 = sSysMegg[1].i_Gun;
		}
		break;

		default:
			break;
	}
	uint8_t alarm = alarmDataBitValue[index];
    alarmDataBitValue[index] = uset;
    if(alarm != uset)
    {
        DLOG("%s[%d->%d],para[%d,%d,%d]",get_alarm_stopcause_tips(index),alarm,uset,para1,para2,para3);
		
        //DLOG("[%d->%d],para[%d,%d,%d]",alarm,uset,para1,para2,para3);
        if(uset)
        {
            //charg_alarm_add(index);   //保存故障记录
        }
        else
        {
            //charg_alarm_del(index);
        }
    }
}

/**
  * @brief  需要快速响应故障.
  * @retval BOOL_TRUE需要，BOOL_FALSE不需要
  */
BOOL get_alarm_fast_response(void)
{
    if((get_alarm_statue(ALARM_DOOR) == 1)||(get_alarm_statue(ALARM_EPO) == 1))
    {
        return BOOL_TRUE;
    }
    return BOOL_FALSE;
}

static void alarm_sys_epo(void)
{
    uint8_t flag = get_In_IO(ENUM_IN_EPO,GUN_SN_ONE);
    static int32_t epocnt = 0;
    int8_t st = cnt_change_until_threshold(flag,&epocnt,10000,1000,1);
    if(st != 2)
    {
        set_alarm_statue(ALARM_EPO,st);
    }    
}

static void alarm_sys_breaker(void)
{
    static int32_t breakercnt = 0;
    int8_t st = 0;

    uint8_t flag = get_In_IO(ENUM_IN_BREAKER,GUN_SN_ONE);
    st = cnt_change_until_threshold(flag,&breakercnt,10000,10,1);
    if(st != 2)
    {
        set_alarm_statue(ALARM_BREAKER,st);
    } 
}

static void alarm_dccont_gun(void)
{
    uint8_t flag = 0;
    static int32_t dccontcnt[GUN_SN_MAX] = {0};
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;
    
	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_DCCONT_GUN1:ALARM_DCCONT_GUN2;
	    flag = get_In_IO(ENUM_IN_DCCONT,gunSn);
	    flag = sSysMegg[gunSn].unOut.bit.bDcCont_Out ^ flag;

	    dccontcnt[gunSn] = (flag!=0)?(dccontcnt[gunSn]+10):(dccontcnt[gunSn]-1);

	    if(dccontcnt[gunSn] >= 10000)
	    {
	        dccontcnt[gunSn] = 10000;
	        set_alarm_statue(index,1);
	    }
	    else if(dccontcnt[gunSn] <= 0)
	    {
	        dccontcnt[gunSn] = 0;
	        set_alarm_statue(index,0);
	    }
	}
}

static void alarm_fuse(void)
{
    uint8_t flag = get_In_IO(ENUM_IN_FUSE,GUN_SN_ONE);
    static int32_t fusecnt = 0;
    int8_t st = cnt_change_until_threshold(flag, &fusecnt, 10000,10,1);

    if(st != 2)
    {
        set_alarm_statue(ALARM_FUSE,st);
    }
}

#ifdef GUNLOCK_CHECK_ALWAYS
/**
  * @brief  充电中检测枪锁故障,默认屏蔽,特殊要求如国网才开启
  */
static void alarm_gunlock(uint8_t gunSn)
{
    static int32_t gunlockcnt_a[GUN_SN_MAX] = {0};
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
    {
	    index = (gunSn == 0)?ALARM_GunLock_GUN1:ALARM_GunLock_GUN2;
	    uint8_t flag = get_In_IO(ENUM_IN_GUNLOCK,gunSn);
	    flag = sSysMegg[gunSn].unOut.bit.bGunLock ^ flag;
	    int8_t st = cnt_change_until_threshold(flag, &gunlockcnt_a[gunSn], 10000,5,1);

	    if(st != 2)
	    {
	        set_alarm_statue(index,st);
	    }
	}
}

/**
  * @brief  只在闭锁解锁时检测枪锁故障,此处只做清除故障处理，故障判断在CTRL_GUNLOCK()
  * @20.04.21 触发清除都放着CTRL_GUNLOCK
  */
static void alarm_gunlock(void)
{
    static int32_t gunlockcnt[GUN_SN_MAX] = {0};
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_GunLock_GUN1:ALARM_GunLock_GUN2;
	    if(get_alarm_statue(index))
	    {
	        uint8_t flag = get_In_IO(ENUM_IN_GUNLOCK,gunSn);
	        flag = sSysMegg[gunSn].unOut.bit.bGunLock^flag;
	        if(flag == 0)
	        {
	            gunlockcnt[gunSn]++;
	            if(gunlockcnt[gunSn] > 10000)
	            {
	                gunlockcnt[gunSn] = 0;
	                set_alarm_statue(index,0);
	            }
	        }
	    }
	    else
	    {
	        gunlockcnt = 0;
	    }
	}
}
#endif

static void alarm_spd(void)
{
    uint8_t flag = get_In_IO(ENUM_IN_SPD,GUN_SN_ONE);
    static int32_t spdcnt = 0;

    int8_t st = cnt_change_until_threshold(flag,&spdcnt,10000,10,1);
    if(st != 2)
    {
        set_alarm_statue(ALARM_SPD,st);
    } 

}

static void alarm_vdc_inv(void)
{
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_VDCINV_GUN1:ALARM_VDCINV_GUN2;

        if(get_alarm_statue(index))
        {
            if(sSysMegg[GUN_SN_ONE].CC1 ==  0)
            {
                set_alarm_statue(index,0);
            }
        }
	}
}

static void alarm_vdc_cc1(void)
{
    static int32_t cc1cnt[GUN_SN_MAX] = {0};
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_CC1_GUN1:ALARM_CC1_GUN2;

		if(chargerMegg[gunSn].unStep.bit.bworking)
        {
            set_alarm_statue(index,(sSysMegg[gunSn].CC1 == 0));
            cc1cnt[gunSn] = 0;
        }

        if(get_charge_state(gunSn) == CHARGER_STATE_IDLE)
        {
            cc1cnt[gunSn]++;
            if(cc1cnt[gunSn] >= 10000)
            {
                cc1cnt[gunSn] = 10000;
                set_alarm_statue(index,0);
            }
        }
    }
}

static void alarm_ot_out(void)
{
    static int32_t ot_outcnt = 0;

    uint8_t flag = (0 != get_alarm_statue(ALARM_OUTLETTEMPUP))?(sSysMegg[GUN_SN_ONE].t_Outlet >= 650):(sSysMegg[GUN_SN_ONE].t_Outlet >= sSysConfig.sInt.uOutletTempUp_10x);
    
    int8_t st = cnt_change_until_threshold(flag,&ot_outcnt,30000,10,1);
    if(st != 2)
    {
        set_alarm_statue(ALARM_OUTLETTEMPUP,st);
    } 
}

static void alarm_ot_gun(void)
{
    static int32_t ot_guncnt[GUN_SN_MAX] = {0};

	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_GUNTEMPUP_GUN1:ALARM_GUNTEMPUP_GUN2;
		
	    if(sSysMegg[gunSn].t_Gun >= sSysConfig.sInt.uGunTempProtect)
	    {
	        ot_guncnt[gunSn] += 10;
	        if(ot_guncnt[gunSn] >= 30000)
	        {
	            ot_guncnt[gunSn] = 30000;
	            set_alarm_statue(index,1);
	        }
	    }
	    else if(sSysMegg[gunSn].t_Gun <= 650)
	    {
	        if(ot_guncnt[gunSn]-- <= 0)
	        {
	            ot_guncnt[gunSn] = 0;
	            set_alarm_statue(index,0);
	        }
	    }
	}
}

static void alarm_vac_up(void)
{
    static int32_t vac_upcnt = 0;
    
    if(get_accont_statue() == BOOL_FALSE)
    {
        vac_upcnt = 0;
        set_alarm_statue(ALARM_VGRIDUP,0);
        return;
    }

    uint16_t vmax = BIGGER(sSysMegg[GUN_SN_ONE].vab_10x,sSysMegg[GUN_SN_ONE].vbc_10x);
    vmax = BIGGER(vmax,sSysMegg[GUN_SN_ONE].vac_10x);

    if(vmax >= sSysConfig.sInt.uAcOverVolt)
    {
        if(vac_upcnt++ >= 10000)
        {
            vac_upcnt = 10000;
            set_alarm_statue(ALARM_VGRIDUP,1);
        }
    }
    else if(vmax <= (sSysConfig.sInt.uAcOverVolt-100))
    {
        if(vac_upcnt-- <= 0)
        {
            vac_upcnt = 0;
            set_alarm_statue(ALARM_VGRIDUP,0);
        }
    }

}

static void alarm_vac_low(void)
{
    static int32_t vac_lowcnt = 0;
    uint8_t flag2 = 0;

	#if 0       
    for(uint8_t u8ModCnt = 1; u8ModCnt <MOD_MAX_ADDR;u8ModCnt ++)    //待模块协议确定
    {
        if(sModMegg_SH.sModRxMegg.unFlagModSt[u8ModCnt].bit.bOnLine==1)
        {
            flag2 = 1;
            break;
        }
    }
	#else     //当前默认模块在线，后期模块协议做好再做修改
	flag2 = 1;
	#endif 

    if((get_accont_statue() == BOOL_FALSE)||(flag2 == 0))
    {
        vac_lowcnt = 0;
        set_alarm_statue(ALARM_VGRIDLOW,0);
        return;
    }
    uint16_t vmin = SMALLER(sSysMegg[GUN_SN_ONE].vab_10x,sSysMegg[GUN_SN_ONE].vbc_10x);
    vmin = SMALLER(vmin,sSysMegg[GUN_SN_ONE].vac_10x);

    if(vmin <= sSysConfig.sInt.uAcUnderVolt)
    {
        if(vac_lowcnt++ >= 10000)
        {
            vac_lowcnt = 10000;
            set_alarm_statue(ALARM_VGRIDLOW,1);
        }
    }
    else if(vmin >= (sSysConfig.sInt.uAcUnderVolt+100))
    {
        if(vac_lowcnt-- <= 0)
        {
            vac_lowcnt = 0;
            set_alarm_statue(ALARM_VGRIDLOW,0);
        }
    }    

}

static void alarm_vdc_up(void)
{    
    static int32_t vdc_upcnt[GUN_SN_MAX] = {0};
	uint8_t flag = 0;
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
		index = (gunSn == 0)?ALARM_VDCUP_GUN1:ALARM_VDCUP_GUN2;
	    if(chargerMegg[gunSn].unStep.bit.bworking)
	    {
	        flag = (sSysMegg[gunSn].v_Gun >= sSysConfig.sInt.uDcOverVolt);

	        if(1 == cnt_change_until_threshold(flag, &vdc_upcnt[gunSn], 10000, 1, 1))
	        {
	            set_alarm_statue(index,1);
	        }
	    }

	    if(get_alarm_statue(index))
	    {
	        if(sSysMegg[gunSn].CC1 ==  0)
	        {
	            vdc_upcnt[gunSn] = 0;
	            set_alarm_statue(index,0);
	        }
	    }    
	}
}

static void alarm_vdc_low(void)
{
    static int32_t vdc_lowcnt[GUN_SN_MAX] = {0};
	uint8_t flag = 0;
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_VDCLOW_GUN1:ALARM_VDCLOW_GUN2;
	    if((chargerMegg[gunSn].unStep.bit.charging)&&(sSysConfig.sTempInt.LoadType == LOAD_BAT))
	    {
	        flag = (sSysMegg[gunSn].v_Gun <= sSysConfig.sInt.uDcUnderVolt);

	        if(1 == cnt_change_until_threshold(flag, &vdc_lowcnt[gunSn], 60000, 1, 1))
	        {
	            set_alarm_statue(index,1);
	        }
	    }

	    if(get_alarm_statue(index))
	    {
	        if(sSysMegg[gunSn].CC1 ==  0)
	        {
	            vdc_lowcnt[gunSn]= 0;
	            set_alarm_statue(index,0);
	        }
	    } 
	}
}

static void alarm_idc_up(void)
{
    static int32_t idc_upcnt[GUN_SN_MAX] = {0};
	uint8_t flag = 0;
	ENUM_ALARM_STOP index = ALARM_STOP_NULL;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    index = (gunSn == 0)?ALARM_IDCUP_GUN1:ALARM_IDCUP_GUN2;
		
	    if(chargerMegg[gunSn].unStep.bit.charging)
	    {
	        flag = (sSysMegg[gunSn].i_Gun >= sSysConfig.sInt.uDcoverCurr);

	        if(1 == cnt_change_until_threshold(flag, &idc_upcnt[gunSn], 10000, 1, 1))
	        {
	            set_alarm_statue(index,1);
	        }
	    }

	    if(get_alarm_statue(index))
	    {
	        if(sSysMegg[gunSn].CC1 ==  0)
	        {
	            idc_upcnt[gunSn] = 0;
	            set_alarm_statue(index,0);
	        }
	    }  
	}

}

static void alarm_door(void)
{
    static int32_t doorcnt = 0;
    if(sSysConfig.sInt.uDoorProtectEn)
    {
        uint8_t flag = get_In_IO(ENUM_IN_DOOR,GUN_SN_ONE);
        int8_t st = cnt_change_until_threshold(flag,&doorcnt,10000,1000,1);
        if(st != 2)
        {
            set_alarm_statue(ALARM_DOOR,st);
        }  
    }
    else
    {
        set_alarm_statue(ALARM_DOOR,0);
        doorcnt = 0;
    }
}

static void alarm_accont(void)
{
    static int32_t accontcnt = 0;
	uint8_t flag;
    
    if(sSysConfig.sInt.uAcContCheckEn)
    {
        flag = get_In_IO(ENUM_IN_ACCONT,GUN_SN_ONE);
        flag = sSysMegg[0].unOut.bit.bACCont_Out ^ flag;
        int8_t st = cnt_change_until_threshold(flag,&accontcnt,10000, 2, 1);
        if(st != 2)
        {
            set_alarm_statue(ALARM_ACCONT,st);
        } 

    }
    else
    {
        set_alarm_statue(ALARM_ACCONT,0);
        accontcnt = 0;
    }    
}

#if 0

static void alarm_ax(void)//掉电记录保存需在机器上测试，桌面上不合适
{
    uint8_t flag = get_In_IO(ENUM_IN_UVP,GUN_SN_ONE);
    static int32_t axcnt = 0;
    int8_t st = cnt_change_until_threshold(flag, &axcnt, 10000, 2000, 1);

    if(st != 2)
    {
        set_alarm_statue(ALARM_AX,st);
    }
}

static void alarm_midcont(void)
{
    if(sSysConfig.sInt.uPorjectType != PROJECT_FASTSLOW)
    {
        set_alarm_statue(ALARM_MIDCONT,0);
        return;
    }
    uint8_t flag = get_In_IO(ENUM_IN_MIDCONT,GUN_SN_ONE);
    flag = sSysMegg[GUN_SN_ONE].unOut.bit.bMidCont_Out ^ flag;
    static int32_t midcont1cnt = 0;
    int8_t st = cnt_change_until_threshold(flag, &midcont1cnt, 10000, 10, 1);

    if(st != 2)
    {
        set_alarm_statue(ALARM_MIDCONT,st);
    }
}
#endif

static void alarm_mod_com(void)
{
    if(!unChargerTaskClockFlag.bit.b1s)
    {
        return;
    }

    if((chargerMegg[GUN_SN_ONE].flag.bit.bStartCh == 0)&&(chargerMegg[GUN_SN_TWO].flag.bit.bStartCh == 0))
    {
        sAlarmMegg.modcomcnt = 0;
    }

    if(sAlarmMegg.modcomcnt >= 30)
    {
        sAlarmMegg.modcomcnt = 30;
        set_alarm_statue(ALARM_MODCOMFAULT,1);
    }
    else if(sAlarmMegg.modcomcnt <= 0)
    {
        sAlarmMegg.modcomcnt = 0;
        set_alarm_statue(ALARM_MODCOMFAULT,0);
    }
    
    sAlarmMegg.modcomcnt++;
}

#if 0
static void alarm_mod_fault(void)
{
    static int32_t modfaultcnt = 0;

    uint8_t online = 0;
    uint8_t fault = 0;    
    uint8_t flag = 0;    
    
    for (uint8_t u8ModCnt = 1; u8ModCnt <MOD_MAX_ADDR;u8ModCnt ++)
    {
        if(sSysMegg.BoardAddr == sModMegg_SH.sModRxMegg.uModGroupSave[u8ModCnt])
        {
            if(sModMegg_SH.sModRxMegg.unFlagModSt[u8ModCnt].bit.bOnLine==1)
            {
                online++;
                
                if(sModMegg_SH.sModRxMegg.unFlagModSt[u8ModCnt].bit.bAlarmSt)
                {
                    fault++;
                }
            }
        }
    }

    if((online != 0)&&(fault == online))
    {
        flag = 1;
    }

    if((chargerMegg[GUN_SN_ONE].flag.bit.bStartCh == 0)&&(chargerMegg[GUN_SN_TWO].flag.bit.bStartCh == 0))
    {
        flag = 0;
        modfaultcnt = 0;
    }

    if(flag)
    {
        modfaultcnt += 2;    
        if(modfaultcnt >= 20000)
        {
            modfaultcnt = 20000;
            set_alarm_statue(ALARM_MOD,1);
        }
    }
    else
    {
        if(modfaultcnt-- <= 0)
        {
            modfaultcnt = 0;
            set_alarm_statue(ALARM_MOD,0);
        }
    }

}
#endif

/**
  * @brief  故障触发停止充电
  */
void alarm_deal(void)
{
    uint8_t curLevel = 0;
    for(uint16_t i=1;i<ALARM_END_CODE;i++)
    {
        if(get_alarm_statue((ENUM_ALARM_STOP)i) != 0)
        {
            if(curLevel < alarmStopCauseCodeMap[i].alarmtype)
            {
                curLevel = alarmStopCauseCodeMap[i].alarmtype;
                if(curLevel>=ALARM_TYPE_FAULT)
                {

					if(ALARM_TYPE_FAULT == curLevel)
					{
                    	set_charger_stopcause(alarmStopCauseCodeMap[i].originalCode,0);  //1枪停机
                    	set_charger_stopcause(alarmStopCauseCodeMap[i].originalCode,1);  //2枪停机
						chargerMegg[0].unStep.bit.fault = 1;
						chargerMegg[1].unStep.bit.fault = 1;
					}
					else if(ALARM_TYPE_GUN1_FAULT == curLevel)
					{
                    	set_charger_stopcause(alarmStopCauseCodeMap[i].originalCode,0);  //1枪停机
						chargerMegg[0].unStep.bit.fault = 1;
					}
					else if(ALARM_TYPE_GUN2_FAULT == curLevel)
					{
                    	set_charger_stopcause(alarmStopCauseCodeMap[i].originalCode,1);   //2枪停机
						chargerMegg[1].unStep.bit.fault = 1;
					}
                }
            }
        }
    }
    
    uint8_t i = 0;
    uint16_t udata = 0;
    for(i=1;i<=16;i++)
    {
        if(get_alarm_statue((ENUM_ALARM_STOP) i))
        {
            udata |= (1<<(i-1));
        }
    }
    sAlarmMegg.uAlarmA.all = udata;
    
    udata = 0;
    for(i=1;i<=16;i++)
    {
        if(get_alarm_statue((ENUM_ALARM_STOP)(16 + i)))
        {
            udata |= (1<<(i-1));
        }
    }
    sAlarmMegg.uAlarmB.all = udata;

    udata = 0;
    for(i=1;i<=16;i++)
    {
        if(get_alarm_statue((ENUM_ALARM_STOP)(32 + i)))
        {
            udata |= (1<<(i-1));
        }
    }
    sAlarmMegg.uAlarmC[0].all = udata;    
    
    udata = 0;
    for(i=1;i<=16;i++)
    {
        if(get_alarm_statue((ENUM_ALARM_STOP)(48 + i)))
        {
            udata |= (1<<(i-1));
        }
    }
    sAlarmMegg.uAlarmC[1].all = udata;    

    static uint16_t a_bak,b_bak,c_bak,d_bak;
    if(a_bak != sAlarmMegg.uAlarmA.all)
    {
        DLOG("A[%x->%x]",a_bak,sAlarmMegg.uAlarmA.all);
        a_bak = sAlarmMegg.uAlarmA.all;
    }

    if(b_bak != sAlarmMegg.uAlarmB.all)
    {
        DLOG("B[%x->%x]",b_bak,sAlarmMegg.uAlarmB.all);
        b_bak = sAlarmMegg.uAlarmB.all;
    }    

    if(c_bak != sAlarmMegg.uAlarmC[0].all)
    {
        DLOG("C[%x->%x]",c_bak,sAlarmMegg.uAlarmC[0].all);
        c_bak = sAlarmMegg.uAlarmC[0].all;
    }

    if(d_bak != sAlarmMegg.uAlarmC[1].all)
    {
        DLOG("D[%x->%x]",d_bak,sAlarmMegg.uAlarmC[1].all);
        d_bak = sAlarmMegg.uAlarmC[1].all;
    }    
}

void alarm_init(void)
{
    memset(&sAlarmMegg,0,sizeof(sAlarmMegg));
    memset(alarmDataBitValue,0,sizeof(alarmDataBitValue));    
}

void alarm_main(void)
{
    if((sys_debug_get() == DEBUG1_NOERROR)
      ||(sys_debug_get() == DEBUG1_PROCESS))
    {
        chargerMegg[0].unStep.bit.fault = 0;
        chargerMegg[1].unStep.bit.fault = 0;
        alarm_init();
        return;
    }

    alarm_sys_epo();
    alarm_sys_breaker();
	alarm_dccont_gun();
    alarm_fuse();
//    alarm_gunlock();
    alarm_spd();
    alarm_vdc_inv();
    alarm_vdc_cc1();
    alarm_ot_out();
	alarm_ot_gun();
    alarm_vac_up();
    alarm_vac_low();
	alarm_vdc_up();
	alarm_vdc_low();
    alarm_idc_up();
    alarm_door();
    alarm_accont();
    //alarm_ax();
    //alarm_midcont();
    alarm_mod_com();
    //alarm_mod_fault();    //后期开发完模块协议增加
    
    alarm_deal();
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/

