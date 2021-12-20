/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   io_megg
作者:   km
说明:   输入IO、输出IO、ADC采样数据.
***************************************************************************************************/

#ifndef IO_MEGG_H
#define IO_MEGG_H

#include "sys_config.h"
#include "os_thread.h"
#include "modyfy.h"
#include "can.h"


#define kCC1VoltScalCst  66 
#define kSysVdcScalCst2 7517//7573//7517         //充电电压采样比例系数
#define kSysPortNegOffset 931//924//931        //充电电压负端AD采样数字量

#define kInsPosVdcScalCst 6892       //绝缘正电压采样比例系数
#define kInsPosVdcScalOffset 513     //绝缘正电压采样偏移系数

#define kInsNegVdcScalCst 6892       //绝缘负电压采样比例系数
#define kInsNegVdcScalOffset 513     //绝缘负电压采样偏移系数

#define kSysCurrentScalCst 170        //充电电流采样比例系数
#define kSysCurrentOffset 2047       //充电电流负端AD采样数字量



typedef union 
{
    uint32_t all;
    struct
    {
        uint32_t bBMS_CAN       :1;
        uint32_t bGFD_Pos       :1;
        uint32_t bGFD_Neg       :1;
        uint32_t bGFD_PE        :1; 
        
        uint32_t bACCont_Out    :1;
        uint32_t bFan           :1;
        uint32_t bMidCont_Out0  :1;
        uint32_t bMidCont_Out1  :1;  
        
        uint32_t bMidCont_Out2  :1;
        uint32_t bRsvOut0       :1;
        uint32_t bRsvOut1       :1;
        uint32_t bRsvOut2       :1;
        
        uint32_t bRsvOut3       :1;
        uint32_t bLed           :1;
        uint32_t bBMS_V         :1;
        uint32_t bRsvOut4       :1;
        
        uint32_t bBMS_12V24     :1;
        uint32_t bDcCont_Out    :1;
        uint32_t bGunLock       :1;//外部控制用作锁枪总标志
        uint32_t bGunLock_PorN  :1;

        uint32_t bReserved      :12;
    }bit;
}unionOutputControl;

typedef enum
{
    ENUM_OUT_BMS_CAN= 0,    //BMS CAN 匹配电阻切换
    ENUM_OUT_GFD_POS,       //绝缘检测正半边
    ENUM_OUT_GFD_NEG,       //绝缘检测负半边
    ENUM_OUT_GFD_PE,        //绝缘检测PE
    
    ENUM_OUT_ACCONT,        //输入接触器
    ENUM_OUT_FAN,           //风扇
    ENUM_OUT_MIDCONT1,      //中间接触器01
    ENUM_OUT_MIDCONT2,      //中间接触器02

    ENUM_OUT_MIDCONT3,      //中间接触器03
    ENUM_OUT_RSV_OUT0,      //预留输出00
    ENUM_OUT_RSV_OUT1,      //预留输出01
    ENUM_OUT_RSV_OUT2,      //预留输出02
    
    ENUM_OUT_RSV_OUT3,      //预留输出03
    ENUM_OUT_LED,           //板上指示灯
    ENUM_OUT_BMS_V,         //BMS A+A- 输出
    ENUM_OUT_RSV_OUT4,      //预留输出04
    
    ENUM_OUT_BMS_12V24,     //BMS A+A- 12V24V切换
    ENUM_OUT_DCCONT,        //输出接触器
    ENUM_OUT_GUNLOCK,       //枪锁12V 
    ENUM_OUT_GUNLOCK_PORN,  //枪锁方向

    ENUM_OUT_MAX,
}ENUM_OUPUT_CONTROL;        //输出IO

typedef union 
{
    uint32_t all;
    struct
    {
        uint32_t bDoor          :1;    
        uint32_t bFuse          :1;
        uint32_t bCC1           :1;
        uint32_t bSPD           :1;

        uint32_t bMidCont1_In   :1;
        uint32_t bMidCont2_In   :1;
        uint32_t bMidCont3_In   :1;
        uint32_t bRsvIn1        :1;

        uint32_t bRsvIn2        :1;
        uint32_t bRsvIn3        :1;
        uint32_t bRsvIn4        :1;
        uint32_t fan_In         :1; 

        uint32_t bGunLock       :1; //正常为高电平
        uint32_t bGunBack       :1; //正常为高电平
        uint32_t bEPO           :1; //正常为高电平
        uint32_t bBreaker       :1;        
        
        uint32_t bACCont_In     :1;
        uint32_t bDcCont_In     :1;

        uint32_t bReserved      :14;
    }bit;
}unionInputBack;

/*typedef struct 
{
    int32_t Door          ;
    int32_t Fuse          ;
    int32_t CC1           ;
    int32_t SPD           ;
    
    int32_t RsvIn2        ;
    int32_t RsvIn3        ;
    int32_t RsvIn4        ;
    int32_t RsvIn5        ;
    
    int32_t RsvIn6        ;
    int32_t RsvIn7        ;
    int32_t RsvIn8        ;
    int32_t UVP           ;
    
    int32_t GunLock       ;
    int32_t GunBack       ;
    int32_t EPO           ;
    int32_t Breaker       ;
    
    int32_t ACCont_In     ;
    int32_t MidCont_In    ;
    int32_t DcCont_In    ;

}structInputCnt;*/

typedef enum
{    
    ENUM_IN_DOOR,       //门禁
    ENUM_IN_FUSE,       //熔断器
    ENUM_IN_CC1,        //CC1
    ENUM_IN_SPD,        //防雷器

    ENUM_IN_MIDCONT1,   //中间接触器1
	ENUM_IN_MIDCONT2,	//中间接触器2
	ENUM_IN_MIDCONT3,	//中间接触器3
    ENUM_IN_RSV1,       //预留1

    ENUM_IN_RSV2,       //预留2
    ENUM_IN_RSV3,       //预留3
    ENUM_IN_RSV4,       //预留4
    ENUM_IN_FAN,        //控制风扇继电器反馈

    ENUM_IN_GUNLOCK,    //枪锁
    ENUM_IN_GUNBACK,    //枪归位，1已归位
    ENUM_IN_EPO,        //急停
    ENUM_IN_BREAKER,    //输入断路器

    ENUM_IN_ACCONT,     //输入接触器
    ENUM_IN_DCCONT,    //输出接触器

    ENUM_IN_MAX,
}ENUM_INPUT_BACK;       //输入IO

typedef struct 
{
	UNLONG unModPortVdc;
	UNLONG unSysPortVdc;
	UNLONG unGFDPosVdc;
	UNLONG unGFDNegVdc;
	UNLONG unCc1Volt;	
	UNLONG unSysPortIdc;

	UNLONG unInletTemp;
	UNLONG unOutletTemp;
	UNLONG unGunPosTemp;
	UNLONG unGunNegTemp;

	int32_t VModCali;
	int32_t VModCaliZero;
	int32_t VGunCali;
	int32_t VGunCaliZero;
	int32_t IGunCali;
	int32_t IGunCaliZero;
}structCalRms;

typedef struct 
{
    unionInputBack unInput,unInput_b;
    unionOutputControl unOutput,unOutput_b;
    //structInputCnt sInputCnt;
    structCalRms sCalRms;
}structBasicMegg;

typedef struct 
{
    uint8_t gunlock;
    int16_t iGunLockNeg12VCnt;
    int16_t iGunLockNeg12VTime;
    int16_t iGunLockPos12VCnt;
    int16_t iGunLockPos12VTime;
    uint8_t lockst;
    uint8_t openst;    
    int32_t UnLockCnt;

}structGunLock;

typedef struct 
{
    uint8_t BoardAddr; // 1 ~ n
	uint16_t version;//本枪版本
	uint16_t version_gun0;//1枪版本
    uint8_t CC1,CC1_bak;

	int16_t v_Mod;      //10x
	int16_t v_Gun;      //10x -241~1041V
	int16_t v_GFD_P;    //10x
	
	int16_t v_GFD_N;     //10x
    int16_t v_CC1_10x;   // 
	int16_t i_Gun;//本枪电流
	int16_t i_Gun_All[BOARD_ADDR_MAX];//[1]代表1号板
	int16_t t_Inlet;
	
	int16_t t_Outlet;
	int16_t t_Gun;
	int16_t t_GunP;
	int16_t t_GunN;

    uint16_t hz_10x;
    uint16_t vab_10x;
    uint16_t vac_10x;
    uint16_t vbc_10x;

    unionOutputControl unOut;
}structSysBasicMegg;//采集数据10x
extern structSysBasicMegg sSysMegg[2];//采样电压、电流、温度、输出io控制等数据

uint8_t get_Out_IO(ENUM_OUPUT_CONTROL eOut,uint8_t gunSn);//输出IO状态
uint8_t get_In_IO(ENUM_INPUT_BACK eIn,uint8_t gunSn);//输入IO状态
void vSys_Basic_Init(void);//GPIO初始化
void vCcu_IO_Ctr(void);
BOOL get_DcCont_State(uint8_t gunSn);
BOOL get_accont_statue(void);
void devOpenAllInit(TAppccuTask *self);
int  send_mod_can_frame(T_CAN_MSG *t_msg);
int8_t tcu_com_can_send(CAN_MSG_T *pt_msg,uint8_t gunSn);




#endif

/***************************************************************************************************
文件结束.
***************************************************************************************************/
