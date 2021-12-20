/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   modManage
作者:   km
说明:   模块管理.
***************************************************************************************************/


#ifndef __MODSYSMANAGE_H_
#define __MODSYSMANAGE_H_


#include "sys_config.h"
#include "io_megg.h"

//SER75015K3B
#define kMod2U_P30K     3000000  
#define kMod_VMAX_1KV   10000  
#define kModRateIdc     200




#define kCHNum 2
#define kModNumMAXCNT	100				// 10s

typedef union
{
	int16_t all;
	struct
	{
		uint16_t    bModSt				:4; //BIT0-3:1:模块状态
		uint16_t    bModOn				:1;	//BIT4:1:模块开机状态，控制模块要不要开机
		uint16_t    bModSleep			:1; //BIT5:1:模块休眠
		uint16_t    bModReady			:1;	//BIT6:1:清模块是否准备就绪，模块从CAN传过来
		uint16_t	bModOldOn			:1;				
		uint16_t	bModType			:1;				
		uint16_t	bReserved9			:1;				 
		uint16_t	bReservedA			:1;
		uint16_t	bReservedB			:1;
		uint16_t	bReservedC			:1;
		uint16_t	bReservedD			:1;
		uint16_t	bReservedE			:1;
		uint16_t	bReservedF			:1;
	}bit;
}unionMachine;

typedef union
{
    uint32_t all;
    struct
    {
        uint32_t    bMod1En             :1; //BIT0:1:1号CAN开关机命令使能
        uint32_t    bMod2En             :1; 
        uint32_t    bMod3En             :1; 
        uint32_t    bMod4En             :1; 
        uint32_t    bMod5En             :1; 
        uint32_t    bMod6En             :1; 
        uint32_t    bMod7En             :1;          
        uint32_t    bMod8En             :1;             
        uint32_t    bMod9En             :1;             
        uint32_t    bMod10En            :1;              
        uint32_t    bMod11En            :1;
        uint32_t    bMod12En            :1;
        uint32_t    bMod13En            :1;
        uint32_t    bMod14En            :1;
        uint32_t    bMod15En            :1;
        uint32_t    bMod16En            :1;

        uint32_t    bMod17En            :1; 
        uint32_t    bMod18En            :1; 
        uint32_t    bMod19En            :1; 
        uint32_t    bMod20En            :1; 
        uint32_t    bMod21En            :1; 
        uint32_t    bMod22En            :1; 
        uint32_t    bMod23En            :1;            
        uint32_t    bMod24En            :1;          
        uint32_t    bMod25En            :1;           
        uint32_t    bMod26En            :1;           
        uint32_t    bMod27En            :1;
        uint32_t    bMod28En            :1;
        uint32_t    bMod29En            :1;
        uint32_t    bMod30En            :1;
        uint32_t    bMod31En            :1;
        uint32_t    bMod32En            :1;
    }bit;
}unionU32ModCanFlag;

struct structParallel
{
	int16_t iModuleOnCmdMax; //为了满足需求电流要用的模块数量
	int16_t iModOnCmdNum;    //当前可以用的模块数量最大值

	unionMachine unMachine[MOD_MAX_ADDR];//模块信息，最多支持32个从机
 	unionU32ModCanFlag unU32ModCanOnOff;//每一个模块的开关机命令

    uint8_t modstall_bak;   //档位备份
    uint8_t modstall_flag;  //档位切换标志
    uint16_t modstall_cnt;   //档位切换5S内不开模块

};
extern struct structParallel sParallel[BOARD_ADDR_MAX];


typedef enum
{
    MODSTALL_HIGH = 0,
    MODSTALL_LOW,
}ENUM_MODSTALL;

typedef struct
{
    uint8_t numdemand;      //需求的模块数量
    uint16_t numdemand_cnt;
    uint16_t i_max;         //模块在当前输出电压下最大输出电流
    double modnum_last;     //上次计算需求模块数量 带小数点
}structModMegg;

extern structModMegg ModMegg[2];

void mod_manage_init(void);
void mod_manage_main(void);


#endif

/***************************************************************************************************
文件结束.
***************************************************************************************************/

