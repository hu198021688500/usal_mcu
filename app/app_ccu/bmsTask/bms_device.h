/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_device
作者:   km
说明:   bms数据处理.
***************************************************************************************************/

#ifndef __BMS_DEVICE_H__                     //防重包含.
#define __BMS_DEVICE_H__
/**************************************************************************************************/

/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "base_type.h"
#include "fun.h"

#define MAX_CHARGER_CURRENT_OFFSETT_GB   4000//充电电流偏移量
#define MAX_CHARGER_CURRENT_OFFSETT_PT   8000//充电电流偏移量

//BMS通信协议中的CAN地址定义
#define GB_CHARGER_CAN_ADDR 0x56 //国标充电机CAN地址
#define GB_BMS_CAN_ADDR     0xF4 //国标BMS的CAN地址
#define PT_BMS_CAN_ADDR     0xF4 //深圳地标BMS的CAN地址
#define PT_CHARGER_CAN_ADDR 0xE5 //深圳地标充电机CAN地址

#define CK_BMS_CAN_ADDR     0xF5 //测试工装BMS目标地址
#define CK_CHARGER_CAN_ADDR 0x55 //测试工装充电桩目标地址

typedef enum    //bms发送的各种PF报文索引定义.
{
    BMS_BRM_TYPE = 0,
    BMS_BCP_TYPE,
    BMS_BRO_TYPE,
    BMS_BCL_TYPE,
    BMS_BCS_TYPE,
    BMS_BSM_TYPE,
    BMS_BMV_TYPE,
    BMS_BMT_TYPE,
    BMS_BSP_TYPE,
    BMS_BST_TYPE,
    BMS_BSD_TYPE,
    BMS_BEM_TYPE,
    BMS_BHM_TYPE,

    BMS_BCM_TYPE = 23,
}BMS_PF_TYPE;

//充电机收到BMS报文的各种帧长度
#define BMS_BRM_DATA_LEN                             64    //BMS和车辆辨识 (多包)
#define BMS_BCP_DATA_LEN                             16    //电池充电参数 (多包)
#define BMS_BRO_DATA_LEN                             8     //BMS完成充电准备
#define BMS_BCL_DATA_LEN                             8     //电池充电需求
#define BMS_BCS_DATA_LEN                             16    //电池充电总状态 (多包)
#define BMS_BSM_DATA_LEN                             8     //电池状态信息
#define BMS_BMV_DATA_LEN                             8     //单体动力蓄电池电压(多包，长度不定)
#define BMS_BMT_DATA_LEN                             8     //动力蓄电池温度(多包，长度不定)
#define BMS_BSP_DATA_LEN                             8     //动力蓄电池预留报文(多包，长度不定)
#define BMS_BST_DATA_LEN                             8     //BMS 中止充电
#define BMS_BSD_DATA_LEN                             8     //BMS 统计数据
#define BMS_BEM_DATA_LEN                             8     //BMS 错误报文
#define BMS_BHM_DATA_LEN                             8     //BHM 握手报文
#define MAX_CAN_FREAME_DATA_LEN                      64    //长包最大数据字节长度


//BMS的各种帧数据存放在数组中的索引
#define K_CAN_FRAME_BRM_02_START_INDEX 0
#define K_CAN_FRAME_BCP_06_START_INDEX (K_CAN_FRAME_BRM_02_START_INDEX+BMS_BRM_DATA_LEN)
#define K_CAN_FRAME_BRO_09_START_INDEX (K_CAN_FRAME_BCP_06_START_INDEX+BMS_BCP_DATA_LEN)
#define K_CAN_FRAME_BCL_10_START_INDEX (K_CAN_FRAME_BRO_09_START_INDEX+BMS_BRO_DATA_LEN)
#define K_CAN_FRAME_BCS_11_START_INDEX (K_CAN_FRAME_BCL_10_START_INDEX+BMS_BCL_DATA_LEN)
#define K_CAN_FRAME_BSM_13_START_INDEX (K_CAN_FRAME_BCS_11_START_INDEX+BMS_BCS_DATA_LEN)
#define K_CAN_FRAME_BST_19_START_INDEX (K_CAN_FRAME_BSM_13_START_INDEX+BMS_BSM_DATA_LEN)
#define K_CAN_FRAME_BSD_1C_START_INDEX (K_CAN_FRAME_BST_19_START_INDEX+BMS_BST_DATA_LEN)
#define K_CAN_FRAME_BEM_1E_START_INDEX (K_CAN_FRAME_BSD_1C_START_INDEX+BMS_BSD_DATA_LEN)
#define K_CAN_FRAME_BHM_27_START_INDEX (K_CAN_FRAME_BEM_1E_START_INDEX+BMS_BHM_DATA_LEN)

#define K_CAN_FRAME_BCM_41_START_INDEX (K_CAN_FRAME_BHM_27_START_INDEX+BMS_BHM_DATA_LEN)

#define K_CAN_FRAME_LEN_END (K_CAN_FRAME_BCM_41_START_INDEX+BMS_BEM_DATA_LEN)


typedef union
{
    uint8_t all;
    struct
    {
        uint8_t  bexit:1;//整个充电过程收到过此报文
        uint8_t  brecv:1;//当前阶段收到此报文，报文处理完成后会被清零
        uint8_t  bit2:1;
        uint8_t  bit3:1;
        uint8_t  bit4:1;
        uint8_t  bit5:1;
        uint8_t  bit6:1;
        uint8_t  bit7:1;
     }bit;
}unionBmsFrameFlag;

typedef union 
{
    uint16_t all;
    struct
    {
        uint16_t bMaxSingleVol:12;//最大单体电压
        uint16_t  bGroup:4;//最大单体电压组号
     }bit;
}unionBatteryInfo1;

typedef union 
{
    uint8_t all;
    struct
    {
        uint8_t  bInsDetFail:2;//车绝缘状态
        uint8_t  bcontactorStatus:2;//连接器状态
        uint8_t  bChargeAllow:2;//充电允许位
        uint8_t  bit6:1;
        uint8_t  bit7:1;
     }bit;
}unionBsmByte7Info;

typedef struct 
{
    const uint8_t pfTypeID;
    const uint8_t gbPf;
    const uint8_t ptPf;
    const uint16_t dataStartIndex;
    const uint8_t maxLen;
    unionBmsFrameFlag flag[2];  //双枪
}BmsFrameStruct;



typedef struct 
{
    char Data[4];
}BmsCemFrameStruct;

typedef struct 
{
    char brmBmsVersion[3];//BMS通信协议版本号
    uint8_t brmBatteryType;//电池类型
    int16_t brmBatteryCapacity;//整车动力蓄电池系统额定容量/A. h ,O.l A  1/位，
    int16_t brmBatteryTotalVol;//整车动力蓄电池系统额定总电压/V， O.l V/  位
    uint8_t brmBatteryFactory[4];//电池厂商名称
    uint8_t brmBatterySn[4];//电池组序号
    uint8_t brmBatteryDateYear;//电池组生产日期年，-1985
    uint8_t brmBatteryDateMonth;//电池组生产日期月
    uint8_t brmBatteryDateDay;//电池组生产日期日    
    uint8_t brmBatteryChargeCycles[3];//电池组充电次数
    uint8_t brmBatteryProperty;//电池组产权 0租赁 1车自有
    char brmVin[17];//车辆识别码(V1N)
    uint8_t brmSoftVersion[8];//BMS软件版本
}BrmFrameData;

typedef struct 
{
    int16_t bcpSingleMaxVol;//单体动力蓄电池最高允许充电电压  放大100倍 0.01V/bit
    int16_t bcpMaxChargerCurrent; //最高允许充电电流 正数已处理过 放大10倍 0.1A/bit
    int16_t bcpMaxChargerVoltage;//最大允许充电电压 放大10倍 0.1V/bit
    int16_t bcpBatteryTotalEnergy;//动力蓄电池标称总能量  0.1kWh/位
    uint8_t  bcpMaxChargerTemperature;//最高允许温度 数据分辨率:1 度/位,偏移量-50
    uint16_t bcpStartSoc;//充电启动SOC 0.1%
    int16_t bcpCurBatteryVoltage;//当前电池电压 放大10倍 0.1V/bit
}BcpFrameData;

typedef struct 
{
    uint8_t broReadyState;//0-未就绪 1-就绪
}BroFrameData;

typedef struct 
{
    int16_t bclVoltageDemand;//BMS需求电压 放大10倍 0.1V/bit         
    int16_t bclCurrentDemand;//BMS需求电流 放大10倍 0.1A/bit         
    uint8_t  bclChargerMode;//充电模式(Ox01: 恒压充电 ;Ox02: 恒流充电)
}BclFrameData;


typedef struct 
{
    int16_t bcsChargerMeasureVoltage;//充电电压测量值 放大10倍 0.1V/bit
    int16_t bcsChargerMeasureCurrent;//充电电流测量值 放大10倍 0.1A/bit
    unionBatteryInfo1 bcsSingleInfo;//单体电压信息
    uint8_t  bcsCurSoc;//当前SOC 1%
    uint16_t bcsChargerLeftTime;//剩余充电时间 min
}BcsFrameData;

typedef struct 
{
    uint8_t bsmMaxSingleVolGroup;//最高单体动力蓄电池电压所在编号
    uint8_t bsmMaxTemperature;//最高动力蓄电池温度  数据分辨率:1 度/位,偏移量-50
    uint8_t bsmMaxTemperatureGroup;//最高温度检测点编号
    uint8_t bsmMinTemperature;//最低动力蓄电池温度
    uint8_t bsmMinTemperatureGroup;//最低动力蓄电池温度检测点编号
    uint8_t bsmBatteyState;//电池状态信息
    unionBsmByte7Info bsmByte7;//连接状态和充电允许
}BsmFrameData;

//HARDWARE_CHECK_MODE
typedef union check_states
{
    uint8_t all;
    struct 
    { 
        uint8_t substate               :1;    // BIT0-3 子状态
        uint8_t insul110kres               :1;    //
        uint8_t res2               :1;    //
        uint8_t bmsFansEn               :1;    //

        
        uint8_t bmsLoardInContactEn                   :1;    // 内部负载
        uint8_t bmsLoardOutContactEn                  :1;    // 外部负载
        uint8_t bmsGunContactEn                       :1;    // K5K6
        uint8_t bmsModeOn                             :1;    // 启动模块
    }bit;
}check_states;

typedef struct 
{
    check_states ms_state;//主副状态
    uint8_t bsmvol;         //辅源电压
    uint16_t gunvol;         //最高温度检测点编号
    uint32_t meterenerge;    //电表电量
}BcmFrameData;


typedef struct 
{
    uint8_t bsdSoc;//终止SOC 1%
    uint16_t bsdMinSingleVol;
    uint16_t bsdMaxSingleVol;
    uint8_t bsdMinTemp;
    uint8_t bsdMaxTemp;
}BsdFrameData;

typedef struct 
{
    uint16_t bhmMaxChargerVol;//BMS最大允许充电电压 放大10倍 0.1V/bit
}BhmFrameData;



typedef struct 
{
    BrmFrameData brmData;
    BcpFrameData bcpData;
    BroFrameData broData;
    BclFrameData bclData;
    BcsFrameData bcsData;
    BsmFrameData bsmData;
    BsdFrameData bsdData;
    BhmFrameData bhmData;

    BcmFrameData bcmData;
    
    uint8_t brmSa;
    uint8_t bcpSa;
    
    BmsCemFrameStruct cemFrameSendData;
    char canFrameString[K_CAN_FRAME_LEN_END];
}BmsFrameData;


typedef struct 
{
    uint32_t chargerTimeSecondCnt;//充电秒数计时
    uint32_t bmsStopCause;//内部停止原因 0-正常 其他BMS通信模块停止充电
    uint32_t bmsStopCause2015First;//2015国标会有重试，这个记录重试前的停止原因
    uint16_t bcpStartSoc;//充电启始SOC
    uint16_t insulationVol;//绝缘检测电压
    uint16_t bsmStopTimeCnt;//BSM状态不允许充电计时
    uint8_t  chargerCnt;//充电启动次数
    uint8_t  cemType;//CEM报文超时类型
    uint8_t  cem22ErrorCnt;
    uint8_t csd1DSendCnt;
    uint16_t croAASendCnt;//5s后才发送0xAA
}BmsData;

typedef struct 
{
    union
    {
        uint32_t all;
        struct
        {
            uint32_t  bStartChargerCmdFlag:1;
            uint32_t  bit1:1;//接触器状态
            uint32_t  bInsulationResultFault:1;//绝缘检测结果 0-正常 1-异常，如果异常就不发送CRM报文了并复位BMS状态为空闲
            uint32_t  bChargerIsFlag:1;//CC1没断开下充电启动过
            uint32_t  bGb2015ProtocolFlag:1;          //GB2015协议标志
            uint32_t  bit6:1;
            uint32_t  bit7:1;
            uint32_t  bit8_31:25;
         }bit;
     }unStateA;
     
     uint8_t cc1State,cc1bak;
     
     int16_t chargerMaxOutputVol;
     int16_t chargerMinOutputVol;
     int16_t chargerMaxOutputCur;
     
     int16_t curChargerOutputVol;//当前输出电压
     int16_t curChargerOutputCur;//当前输出电流
     
     int16_t iChVdcRef;//当前下发模块电压
     int16_t iChIdcRef;//当前下发模块电流
     
     uint32_t meterElectricEnergy;//当前电表总电能
     
     uint32_t externStopCause;//外部停止原因 0-正常 其他为人工停止或充电机故障
}BmsExternalData;//外部模块提供给BMS使用的数据

enum SysMianState//系统主状态
{
    IDLE=0, //待机
    WAIT_START,//等待充电指令
    SHAKE_HAND,//握手阶段
    SETTING,//设置阶段
    CHARGING,//充电阶段
    CHARG_OVER,//充电结束阶段
    ERROR_STAGE,//错误阶段
    BMS_MAIN_STATE_NUM_NULL
};

enum SysSubState//系统子状态
{
    SEND_CHM = 0,
    SEND_CRM01_00,
    SEND_CRM01_01,
    RECV_BCP06,
    SEND_CTS07,
    SEND_CCM40,//HARDWARE_CHECK_MODE
    SEND_CML08,
    RECV_BRO09,
    SEND_CRO0A,
    
    RECV_BCL10,
    RECV_BCS11,
    SEND_CCS12,
    RECV_BSM13,
    RECV_BMV15,
    RECV_BMT16,
    RECV_BSP17,
    RECV_BST19,
    SEND_CST1A,
    
    RECV_BSD1C,
    SEND_CSD1D,
    RECV_BEM1E,
    SEND_CEM1F,
    SEND_CEM1F_END,
};


typedef union
{
    uint32_t all;
    struct
    {
        uint32_t  bPowerState:1;//开关机状态 0-BMS待机  1-充电  //bsm充电允许
        uint32_t  bComChargingStageFlag:1;//表明BMS进入充电阶段
        uint32_t  bProtocolStable:1; //协议类型确定标志
        uint32_t  bBmsFault:1;//BMS故障
        uint32_t  bBmsChargerStopNormal:1;//充电正常停止
        uint32_t  bBmsComFault:1;//BMS通信异常,国网
        uint32_t  bGb2015ProtocolFlag:1;          //1 GB2015协议标志
        uint32_t  bInsulationCmdFlag:1;           //绝缘启动命令有效标志
        
        uint32_t  bReChargerFlag:1;//重充电标志
        uint32_t  bBSMPause:1;
        uint32_t  bitA:1;
        uint32_t  bitB:1;
        uint32_t  bitC:1;
        uint32_t  bitD:1;
        uint32_t  bitE:1;
        uint32_t  bitF:1;
        
        uint32_t  bit10_1F:16;
     }bit;
}unionBmsStateA;
     
typedef union
{
    uint32_t all;
    struct
    {
        uint32_t  bBmsComState:1;//BMS通信状态 0-空闲 1-正在通信中
        uint32_t  bit1_F:15;
        uint32_t  bit10_1F:16;
     }bit;
}unionBmsStateB;

typedef struct 
{
    uint8_t mainState;//主状态
    uint8_t subState;//子状态
    
    uint8_t bmsType;
    
    unionBmsStateA  unBmsStateA;
    unionBmsStateB  unBmsStateB;
    
    BmsFrameData frameData;
    BmsData bmsData;
}BmsDevice;




/***************************************************************************************************
申明:   公有变量.
***************************************************************************************************/
extern BmsDevice bmsDevice[2];
extern BmsExternalData bmsExternData[2];
extern unionSysClockFlag unBmsTaskClockFlag;
extern structSysClockCnt sBmsTaskClockCnt;

/***************************************************************************************************
申明:   公有方法.
***************************************************************************************************/
void bms_device_init(uint8_t gunSn);
void bms_device_charger_handle(void);
void bms_charger_start(uint8_t gunSn);

/**
  * @brief  外部的模块（如国网）则用此函数来获取BMS模块相关信息和数据
  */
extern const BmsFrameData* getBmsFrameDataHandle(uint8_t gunSn);
extern const BmsDevice* getBmsDeviceHandle(uint8_t gunSn);

#endif  //__BMS_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/

