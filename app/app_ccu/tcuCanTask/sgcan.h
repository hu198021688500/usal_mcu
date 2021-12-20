/******************************
    > File Name: sgcan.h
    > Author:Zhx
    > Mail: 862145496@qq.com
    > Created Time: Sat 18 Sep 2021 02:15:35 AM PDT
 *****************************/

#ifndef SGCAN_H_
#define SGCAN_H_

#include "can.h"


#define         CAN_UP          0
#define         CAN_DOWN        1
#define         CAN_RECT        2



#define SG_CAN_BUF_SIZE         (1024+20)
#define SG_CAN_MAX_PACK_NUM     64



typedef struct
{
    uint32_t   id;
    uint16_t   len;
    uint16_t   first_len;
    uint16_t   check_sum;
    uint8_t    pack_num;
    uint8_t    next_pack;
    uint8_t    recv_complete;
    uint8_t    crc_error;
    uint8_t    restart;
    uint8_t    data[SG_CAN_BUF_SIZE];
} T_CAN_SG_DATA;

typedef struct
{
    uint32_t pgn;
    uint32_t id;
    uint16_t len;
    uint8_t  src_addr;
    uint8_t  dest_addr;
    uint8_t  pri;
    uint8_t  data[8];
} T_CAN_SING_DATA;

typedef struct{
    T_CAN_SG_DATA     	t_tx_data;
    T_CAN_SG_DATA       t_rx_data;
    T_CAN_SING_DATA     t_sing_data;
}T_SG_DATA;

extern CAN_MSG_T tcu_can_data;

uint32_t   u32_sg_forward_tx(uint32_t port,uint8_t src_addr,uint8_t dst_addr,T_CAN_SG_DATA *pt_can_data);
//int32_t   s32_sgcan_set_driver(uint32_t port,S32_CAN_RXTX can_rx,S32_CAN_RXTX can_tx);
uint32_t   u32_simpcan_rx(uint32_t port,T_CAN_SG_DATA *can_multi, T_CAN_SING_DATA *single_can);
uint32_t   u32_sg_can_tx(uint32_t port,T_CAN_SG_DATA *pt_can_data);

#endif /* SGCAN_H_ */
