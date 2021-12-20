/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_canhandle
作者:   km
说明:   bms数据处理.
***************************************************************************************************/

#ifndef __CAN_HANDLE_H__                     //防重包含.
#define __CAN_HANDLE_H__
/**************************************************************************************************/

/***************************************************************************************************
包含:   头文件.
***************************************************************************************************/
#include "base_type.h"
#include "bms_device.h"
#include "can_tx_rx_list.h"           
#include "kfc_dev.h"
#include "srt_config.h"
#include "can_def.h"

//连接管理的PF定义
#define CAN_TP_CM_RTS  0xec //连接模式下的请求发送
#define CAN_TP_DT_RTS  0xeb //传输协议：数据（TP.DT）



typedef struct
{
    //用于传输协议
    uint16_t numberOfBytes; //全部报文字节数
    uint8_t numberOfPackets; //全部数据包数
    uint8_t pgn; //参数组编号
    uint8_t sequenceRx;
    uint16_t recvTickMsCnt;//接收时间计数
    
    uint8_t data[MAX_CAN_FREAME_DATA_LEN];
}CanLongFrame;



typedef struct
{
    char *data;
    uint8_t len;
    unionBmsFrameFlag flag;
}PCanFrame;


typedef struct
{
     CanFrameHeader sendCanHeader;
     CanFrameHeader recvCanHeader;
     CanLongFrame   canLongFrame;
     //CanTxRxList *p_canHandle;//存放报文对象指针
     CanRxMsg m_rxMessage;//接收到的数据
     CanTxMsg m_sendFrame;//发送报文
}CanHandlerData;

typedef union
{
    uint32_t all;
    struct
    {
        uint32_t  bCheckPriorityEnable:1;//是否检测报文优先级
        uint32_t  bCheckFrameLongEnable:1;//是否检查报文的长短包
        uint32_t  bCheckSourceAddrEnable:1;//是否检查报文的源地址 
        uint32_t  bCheckFrameOrderEnable:1;//多包报文是否检查报文顺序
        
     }bit;
}unionCanhandleMode;
/***************************************************************************************************
申明:   公有变量.
***************************************************************************************************/
extern CanHandlerData canHandlerData[2];

/***************************************************************************************************
申明:   公有方法.
***************************************************************************************************/
extern int send_bms_can_frame(CanTxMsg *canFrame,uint8_t gunSn);
extern void canhandle_recv_init(uint8_t gunSn);
extern const PCanFrame *get_canheadler_bms_can_frame(BMS_PF_TYPE canIndex,uint8_t clearFlag,uint8_t gunSn);
extern int get_bms_can_frame_exit_flag(BMS_PF_TYPE canIndex,uint8_t gunSn);
extern void canhandle_task_1ms(void);
extern int set_canhandle_work_mode(unionCanhandleMode mode,uint8_t gunSn);

#endif  //__CAN_HANDLE_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/

