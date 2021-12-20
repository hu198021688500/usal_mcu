/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   charge_task
作者:   km
说明:   充电任务.
***************************************************************************************************/

#ifndef __CHARGETASK_H
#define __CHARGETASK_H

#include "fun.h"

typedef enum
{
    CHARGER_STATE_IDLE = 0,         //空闲
    CHARGER_STATE_GUNLOCK,          //枪锁闭合
    CHARGER_STATE_MOD_SWITCH,       //模块调配/接触器动作
    CHARGER_STATE_AXLOCK,           //辅源闭合
    CHARGER_STATE_GB_CONFIG,        //国标判断
    CHARGER_STATE_INSDET,           //绝缘检测+泄放
    CHARGER_STATE_INS_DISCHARGE,    //绝缘泄放完成
    CHARGER_STATE_WAIT_BROAA,       //BRO
    CHARGER_STATE_PRE_CHARGE,       //预充电
    CHARGER_STATE_CHARGING,         //充电中
    CHARGER_STATE_STARTFAULT,       //启动失败
    CHARGER_STATE_STOP,             //停止充电
    CHARGER_STATE_CHG_DISCHARGE,    //停止泄放
    CHARGER_STATE_AXOPEN,           //辅源断开
    CHARGER_STATE_GUNOPEN,          //枪锁断开
    CHARGER_STATE_END,              //结束
    CHARGER_STATE_END_SAVE,         //记录保存

    CHARGER_STATE_MAX, //17，改动该enum需注意ChargerSaveEnergyData-chargStTime只能放16个
}ENUM_SYS_CHARGER_STATE;

typedef struct 
{
    ENUM_SYS_CHARGER_STATE state;   //
    char *tip;                      //显示、记录等
}structStateTip;

/**
  * @brief  开机条件判断
  */
typedef enum
{
    CHARGE_START_LIMIT_NULL = 0,
    CHARGE_START_LIMIT_BUSY,        //不是待机状态
    CHARGE_START_LIMIT_DISCONNECT,  //枪未连接
    CHARGE_START_LIMIT_FAULT,       //充电机故障不能充电
}ENUM_CHARGER_START_RESULT;

typedef enum
{
    START_BY_ADMIN = 1,             //从板子屏幕开机
    START_BY_TCU, 
}ENUM_CHARGER_START_TYPE;          //开机方式

typedef struct
{
//0*32
    uint16_t chargerIndex;          //当前充电记录索引
    uint16_t chargerStopReason;     //充电结束原因
    uint32_t chargerTime;           //总充电时间单位秒
    uint32_t chargerStartTime;      //
    uint32_t chargerEndTime;        //
//4
    uint8_t startSoc;               //开始SOC
    uint8_t endSoc;                 //结束SOC
    uint8_t startChargeMode;        //充电启动方式
    uint8_t gbVersion;              //充电国标类型
    int32_t chargerPower;           //总充电电量 0.001KW
    int32_t chargerStartPower;      //充电启始电量
    int32_t chargerEndPower;        //充电结束电量
//8
    int16_t insdetVdc;              //绝缘需求电压
    uint16_t insdetRes;
    uint16_t insdetRes2;
    int16_t GFDPosVdc;
    int16_t GFDNegVdc;
    int16_t GFDPosVdc2;
    int16_t GFDNegVdc2;
    int16_t GFDVmod;                //绝缘检测，模块电压启动异常时存储
//12    
    int16_t pre_v_gun;
    int16_t pre_v_mod;
    int16_t bcp_vabs;               //BCP电压异常 整车动力蓄电池当前电池电压
    int16_t bcp_vabs2;              //BCP电压异常 产生BCP电压异常的前一时刻枪头电压值
//14    
    int8_t vin[17];                 //车辆VIN
    int8_t workmode;                //0 远程 1 本地
    int8_t rsv[2];                  //预留，对齐。
//19
    int16_t stopmegg1;
    int16_t stopmegg2;
    int16_t stopmegg3;
    int16_t stopmegg4;
    int16_t stopmegg5;
    int16_t stopmegg6;
    int16_t stopmegg7;
    int16_t stopmegg8;
//23

}ChargerSaveData;//修改向后补，防止前面记录冲突

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


typedef union 
{
    uint16_t all;
    struct
    {
        uint16_t bStartCh       :1; // 开机状态 1开机 0待机
        uint16_t bStartMod      :1; // 模块开机总控制 1开机 0待机
        uint16_t bRecordSave    :1;	// 记录保存状态 1保存中
        uint16_t bAcConnect     :1; // 需闭合交流接触器
        uint16_t bFan           :1; // 需闭合风扇
        uint16_t bInsdet        :1; // 绝缘板动作

        uint16_t bRsv           :10;
    }bit;
}unionChargerDeviceFlag;

typedef struct
{
    int32_t modswitchcnt;
    int32_t axdelaycnt;
    int32_t gunopencnt;
    int32_t insdetDiscnt;
    int32_t waitBatVolCnt;
    int32_t waitPortCnt;
    int32_t bsmPausecnt;
    int32_t dcContCutcnt;
    int32_t chargeDiscnt;
    int32_t updaterecordcnt;    
    int32_t recordcnt;
    
    int32_t maxvoltagecnt;
    int32_t bclvoltagecnt;
    int32_t batSinglecnt;
    int32_t batSOCcnt;
    int32_t batTempcnt;
    int32_t prechgcnt; //软启动超时

}structChargerDeviceCnt;


typedef enum
{
    INSDET_STATE_IDLE = 0,          //空闲
    INSDET_STATE_START,             //启动绝缘
    INSDET_STATE_MULTI,             //并充副枪
    INSDET_STATE_GB15,              //模块电压先开启至绝缘电压，再闭合接触器  
    INSDET_STATE_INS1,              //继电器切换1
    INSDET_STATE_INS2,              //继电器切换2
    INSDET_STATE_CALCULATE,           
    INSDET_STATE_DISCHARGE,         //泄放
    INSDET_STATE_DISCHARGE_DELAY,   //等待断开
    INSDET_STATE_OVER,              //检测完成
}ENUM_SYS_INSDET_STATE;//绝缘状态机

typedef struct
{
    int32_t v_InsDet;               			//绝缘检测电压
    ENUM_SYS_INSDET_STATE state,state_bak;    	//绝缘流程状态机

    int16_t PortVoltAbsCnt;         			//绝缘前，端口电压大于某值检测
    int16_t PortVoltInvCnt;         			//绝缘前，端口电压反接检测
    int32_t ModVoltDelayCnt;       				//GB15绝缘时先开模块再闭合接触器，中间延时
    int32_t ModVoltAbsCnt;         				//模块电压未达到绝缘电压异常
    int32_t ins1cnt;
    int32_t ins2cnt;
    int32_t dischargecnt;          				//泄放

}structInsdetDeal; 

typedef struct
{
    int8_t cc1StateStableCount;
    int8_t fanCount;
    int32_t accontCount;

}structOtherCnt; 

typedef struct
{
    ENUM_SYS_CHARGER_STATE chargerState,chargerStatebak;    //充电状态
    structChargerDeviceCnt cnt;             				//充电状态机内cnt，IDLE清空
    structInsdetDeal sInsdet;               				//绝缘数据，IDLE清空

    uint8_t cc1State;
}sChargerDevice;    //静态，仅供对应.c使用

typedef struct
{
    ENUM_SYS_CHARGER_STATE state_last;  //上一阶段，用作记录
    int16_t v_insdet;                   //绝缘检测需求电压
    int16_t v_mod[2];                   //模块电压整秒数据，[0]上一个 [1]上上个
    int16_t v_gun[2];                   //枪口电压整秒数据，[0]上一个 [1]上上个
    int16_t i_gun[2];                   //枪口电压整秒数据，[0]上一个 [1]上上个
    int16_t v_demand[2];                //需求电压整秒数据，[0]上一个 [1]上上个
    int16_t i_demand[2];                //需求电流整秒数据，[0]上一个 [1]上上个
    int16_t v_bcs[2];                   //BCS电压整秒数据，[0]上一个 [1]上上个
    int16_t i_bcs[2];                   //BCS电流整秒数据，[0]上一个 [1]上上个
    int16_t i_max;                      //充电中最大输出电流，来于电表
    int16_t t_max;                      //充电中电池最大温度，来于BSM
    int16_t v_max_bat;                  //充电中电池最大单体电压 *10，BYTE0来于BCS单体，BYTE1来于BCP最高允许单体
}structRecordData;  //用作记录数据

typedef struct
{
    uint16_t v_demand; 					//BMS需求或本地需求电压
    uint16_t i_demand; 					//BMS需求或本地需求电流
    uint16_t v_demand_mod; 				//发给本枪模块的总电压，限制时会有区别，此枪下发模块的需求电压
    uint16_t i_demand_mod; 				//发给本枪模块的总电流，限制或并充时会有区别，此枪下发模块的需求电流
    uint32_t  chargerStopCause;			//充电结束原因，开机或插枪时清除

    unionChargerDeviceFlag flag;
    ChargerSaveData record;         	//充电临时记录
    UNION_SYS_CHARGE_STEP unStep;   	//充电状态大分段
    structRecordData record_data;       //记录数据
}sChargerMegg;

extern sChargerMegg chargerMegg[2];
















extern structSysClockCnt sChargerTaskClockCnt;
extern unionSysClockFlag unChargerTaskClockFlag;



void chargeTaskInit(void);
void charger_task_main(void);
ENUM_SYS_INSDET_STATE get_insdet_state(uint8_t gunSn);
ENUM_SYS_CHARGER_STATE get_charge_state(uint8_t gunSn);
BOOL start_charger_interface(ENUM_CHARGER_START_TYPE type,uint8_t gunSn);//开机接口
void set_charger_stopcause(uint32_t stopCause,uint8_t gunSn);//关机接口
uint32_t get_charger_stopcause(uint8_t gunSn);


#endif

