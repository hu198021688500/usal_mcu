/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   sys_config
作者:   km
说明:   系统参数.
***************************************************************************************************/


#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

#include"base_type.h"

//系统最大输出电压、电流
#define GUN_MAX_VOLT         	10400		 //充电枪最大输出电压
#define GUN_MAX_CURR         	5000		 //充电枪最大输出电流
#define MOD_MAX_ADDR         	32      	 //系统模块最大数量，所有对应数组[1]代表1号模块
#define VOLTAGE_FLOOR           500		     //充电枪最小输出电压
#define SYS_MAX_TEMP         	1500		 //系统检测最大温度
#define SYS_MIN_TEMP         	-500		 //系统检测最小温度

//校准范围
#define kCaliMax	6144			
#define kCaliMid	4096			
#define kCaliMin	2048			
#define kZeroMax	1024			
#define kZeroMid	0			
#define kZeroMin	-1024			

//存储有效标志
#define SYS_CONFIG_VALID_FLAG 0x5AA52288


typedef struct 
{
    char sysSn[32];//充电桩编码   
}structSysConfigChar;

#pragma pack(push)
#pragma pack(1)
typedef struct 
{
    int32_t uPorjectType;              //项目类型 ENUM_PROJECTTYPE
    int32_t uSysType;                  //故障源ENUM_SYS_TYPE
    int32_t uModuleType;               //模块类型  
	int32_t uModNum;			       //模块数量-总（所有）
	int32_t unitModuleNum;             //每组单元的模块数量
	int32_t uGhMode;                   //接触器群充机型：1均充模式 2先到先充
	int32_t uModLoadValue;             //模块负载率
	int32_t uModLimit;                 //模块输出率
    int32_t uLowNoise;                 //模块降噪模式    
    
    int32_t uGunNum;                   //枪数
    int32_t uGunType;                  //充电枪类型
    int32_t bmsType;                   //BMS类型ENUM_BMSTYPE
    int32_t uBmsAxType;                //辅源类型 0:12V 1:24V
    int32_t uMeterType;                //电表类型
    int32_t uRsvIoType;                //预留IO类型
	int32_t uParallelMode;			   //并充模式
    int32_t iAGun_ScalSysPortVdcCali[2];  //输出端电压校准   
    
    int32_t iAGun_ScalSysPortVdcZero[2];   
    int32_t iAGun_ScalModVdcCali[2];      //模块电压校准   
    int32_t iAGun_ScalModVdcZero[2];   

    //保护
    int32_t uMaxChargeVolt;            	//最高允许充电电压
    int32_t uMaxChargeCurr;            	//最高允许充电电流
    int32_t uPowerLimit_10x;           	//最高允许充电功率 
    int32_t uInsdetRes;                	//绝缘阻值
    int32_t uMaxAllowChSOC;            	//最高允许SOC
    int32_t uGunTempProtect;           	//充电枪过温值
    int32_t uEnvTmepHigh;            	//
    int32_t uEnvTmepLow;           	    //
	int32_t uOutletTempUp_10x;			//内部过温
    int32_t uDcOverVolt;               	//过压值 
    
    int32_t uDcUnderVolt;              	//欠压值
    int32_t uDcoverCurr;               	//过流值
    int32_t uAcOverVolt;               	//输入过压值
    int32_t uAcUnderVolt;              	//输入欠压值
	int32_t uDoorProtectEn;            	//门禁保护
	int32_t uBreakerCheckEn;           	//断路器检测
	int32_t uAcContCheckEn;            	//输入接触器检测
	int32_t uBatProtectEn;             	//车辆电池保护
	
	int32_t uBatStatueEn;              	//车辆状态保护
	//int32_t uBMSAxGuding;              	//固定bms 辅源电压，不恢复12

	uint32_t validFlag;//标志
    uint32_t crc;
}structSysConfigInt;//固定4字节,MAX->W25_PAGE_SIZE
#pragma pack(pop)


typedef struct 
{
    int32_t WorkMode;                  //工作模式 ENUM_WORKMODE，远程为IDLE时才能切换set_workmode
    int32_t LoadType;                  //负载类型 ENUM_LOADTYPE
    int32_t v_local;                   //本地需求电压
    int32_t i_local;                   //本地需求电流

    int32_t GunUnLock[2];                 //应急解锁
    int32_t uRenZhengEn;               //认证模式使能
    
    int32_t Debug1;
    int32_t Debug2;
    int32_t getModMegg;                //获取模块信息指令
    int32_t modFanspeed;               //模块风扇速度
    int32_t sysFanspeed;               //系统风扇速度
}structSysConfigTempInt;//vClientSinexcelConfigData

typedef struct 
{
    int32_t para[32];
    char str1[64];                 
    uint8_t str2[64];                 
}structSysConfigUnstandard;//非标保持参数

typedef struct 
{
    structSysConfigInt sInt;            //数据--保存
    structSysConfigTempInt sTempInt;    //数据--不保存
    structSysConfigChar sChar;          //字符串-保存
    structSysConfigUnstandard sUnstandard;//非标-保存
}structSysConfig;
extern structSysConfig sSysConfig;

typedef enum
{
    DEBUG1_TCU = 1,             //无TCU通信
    DEBUG1_NOERROR,             //不报故障
    DEBUG1_PROCESS,             //开机模拟，无故障


    DEBUG1_FLASH_CLEAR = 1024,         //清空FLASH    
    DEBUG1_MAX,
}ENUM_DEBUG1;

typedef enum
{
    SYSTYPE_FASTSLOW = 0,             //快慢充
	SYSTYPE_CONTACTOR_GROUP,	  //接触器群充
    
    SYSTYPE_MAX,
}ENUM_PROJECTTYPE;

typedef enum
{
    BMS_GB_TYPE = 0,
    BMS_GB_2015_TYPE = 5,
    BMS_MAX_TYPE,
}ENUM_BMSTYPE;

typedef enum
{
    WORK_REMOTE = 0,
    WORK_LOCAL,
}ENUM_WORKMODE;

typedef enum
{
    LOAD_BAT = 0,
    LOAD_R,
}ENUM_LOADTYPE;

typedef enum
{
    MOD_TYPE_0 = 0,
		
	MOD_TYPE_INFY_30KW,      //英飞源30kw模块
	MOD_TYPE_WINLINE_30KW,	 //永联30kw模块
}ENUM_MODULETYPE; //模块类型

typedef enum
{
    ENUM_GUN_TYPE2 = 2,    
    ENUM_GUN_NONE  = 4,     //不锁枪    

}ENUM_GUN_TYPE;             //充电枪类型

typedef enum
{
    BOARD_MAIN = 1,         //主板

    BOARD_ADDR_MAX  = 13,   //最多12枪，所有对应数组[1]代表1枪    
}ENUM_BOARD_ADDR;           //板子地址

void sys_config_init(uint32_t sysConfigValidFlag);
void set_sys_config_save(void);
void record_manage_sys_config_init(uint32_t sysConfigValidFlag);
void voltage_cali(int32_t idata,uint8_t gunSn);

void sys_debug_set(uint16_t de);
uint16_t sys_debug_get(void);

#endif


/***************************************************************************************************
文件结束.
***************************************************************************************************/
