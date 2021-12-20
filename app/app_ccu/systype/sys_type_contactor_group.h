/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   sys_type_contactor_group.h
作者:   km
说明:   接触器群充控制逻辑头文件.
***************************************************************************************************/
#ifndef SYS_TYPE_CONTACTOR_GROUP_H
#define SYS_TYPE_CONTACTOR_GROUP_H

#include "fun.h"


#define GH_TWOGUN_MAX_GROUP 6
#define GH_TWOGUN_GUN_NUM 2

#define GH_MOD_MAX_NUM 32


typedef union
{
    uint8_t all;
    struct{
    uint8_t bCont1:1;
    uint8_t bCont2:1;
    uint8_t bCont3:1;
    uint8_t bCont4:1;
    uint8_t bCont5:1;

    uint8_t bRsv :3;    
    }bit;
}unContCtrlandBack;

typedef struct 
{
    char group_sim[GH_TWOGUN_MAX_GROUP]; //每组模块应在组号（计算中）
    char group_now[GH_TWOGUN_MAX_GROUP]; //每组模块现在组号
    char startseq[GH_TWOGUN_GUN_NUM];//启动顺序 [0]最先开机枪号
    char group_num[GH_TWOGUN_GUN_NUM]; //每枪现在模块组数
    
    char init_flag;
    char mod_num_total;     //模块总数
    char mod_num_unit;      //每个单元模块数
    char unit_num;          //单元数

    char relay_fault[GH_TWOGUN_MAX_GROUP],relay_fault_bak[GH_TWOGUN_MAX_GROUP];//中间接触器故障类型 1：一个合不上 2：两个都粘连
    unContCtrlandBack unContst,unContErr;
    unContCtrlandBack unCont,unCont_Deal,unCont_bak; //un为中间计算，Deal为IO最终执行，中间有延时
    char relay_wait[GH_TWOGUN_MAX_GROUP];//un->deal待执行
    int contFaultCnt[GH_TWOGUN_MAX_GROUP];
    char gunStart_bak[GH_TWOGUN_GUN_NUM];

	unsigned char uOrderNum;   //命令类型
	unsigned char uOrderPar;   //命令参数
    
    int stepcnt;
}GhModeTwoGunData;

void vGh2_contactor_group(void);
uint8_t vGetMidContCt_Gh2(void);
void vSetMidContSt_Gh2(uint8_t st);
uint8_t vGetMidContSt_Gh2(void);
void vMidContfault_Gh2(void);
uint8_t vGetModGroup_Gh2(uint8_t addr);
uint8_t vGetMidContErr_Gh2(void);
uint8_t vGetModGroupNum_Gh2(uint8_t gun); 


#endif


