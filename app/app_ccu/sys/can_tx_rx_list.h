/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技
文件:   can_tx_rx_list
作者:   km
说明:   Can的数据处理.
***************************************************************************************************/


#ifndef __CAN_TX_RX_LIST_H__                     //防重包含.
#define __CAN_TX_RX_LIST_H__

#include "base_type.h"
//#include "stm32f4xx.h"
#include "stm32f4xx_can.h"

typedef struct
{
    uint8_t sa;    //源地址
    uint8_t da;    //目标地址
    uint8_t pf;    //PDU格式
    union 
    {
        struct 
        {
            uint8_t dp:1;    //数据页
            uint8_t r:1;    //保留位
            uint8_t p:3;    //优先级
        }bit;
        uint8_t pp;
    }bype1;
}CanHeader;

typedef union
{
    unsigned int all;
    CanHeader header;
}CanFrameHeader;
    
#endif  //__CAN_TX_RX_LIST_H__

/***************************************************************************************************
文件结束.
***************************************************************************************************/



