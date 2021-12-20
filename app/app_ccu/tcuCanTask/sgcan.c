

#include "sgcan.h"
#include <string.h>
//#include "base.h"
#include "io_megg.h"

#define     MAX_NUM             4
//static      T_DRIVER_CAN        t_drv_can[MAX_NUM];

CAN_MSG_T tcu_can_data;

static int8_t can_recv(CAN_MSG_T *pt_msg)
{
	pt_msg->id = tcu_can_data.id;
	pt_msg->len = tcu_can_data.len;
	memcpy(pt_msg->data,tcu_can_data.data,pt_msg->len);
	return 0;
}

uint32_t u32_sg_can_tx(uint32_t port,T_CAN_SG_DATA *pt_can_data)
{
#if 1
    CAN_MSG_T t_msg;
    uint16_t i = 0, pack = 0, sum = 0;

    if (pt_can_data->len >= (SG_CAN_BUF_SIZE - 5))
        return 0;

    memset(&t_msg, 0, sizeof(t_msg));

    t_msg.id =  pt_can_data->id;
//    t_msg.id = ((uint32_t)(pt_can_data->u8_pri & 0x07) << 26);
//    t_msg.id |= ((pt_can_data->u32_pgn & 0x0000FF00) << 8);
//    t_msg.id |= ((uint32_t)(pt_can_data->u8_dest_addr) <<8);
//    t_msg.id |= pt_can_data->u8_src_addr;

    t_msg.ch = 0;
    t_msg.format = 1;    //0 - STANDARD, 1 - EXTENDED IDENTIFIER
    t_msg.type = 0;      //0 - DATA FRAME, 1 - REMOTE FRAME
    t_msg.len = 8;

    if (pt_can_data->len <= 8)
    {
        for (i=0; i<t_msg.len; i++)
            t_msg.data[i] = pt_can_data->data[i];
//        return t_drv_can[port].s32_can_tx(&t_msg);
        tcu_com_can_send(&t_msg,port);
    }
    else
    {
        pack =  (pt_can_data->len + 3 + 2 + 6) / 7;     
        sum += pack;

        t_msg.data[0] = 1;
        t_msg.data[1] = pack;
        t_msg.data[2] = pt_can_data->len;
        t_msg.data[3] = pt_can_data->len >> 8;
        t_msg.data[4] = pt_can_data->data[0];
        t_msg.data[5] = pt_can_data->data[1];
        t_msg.data[6] = pt_can_data->data[2];
        t_msg.data[7] = pt_can_data->data[3];

        sum += t_msg.data[2];
        sum += t_msg.data[3];
        for (i = 0; i < pt_can_data->len; i++)
            sum += pt_can_data->data[i];

        pt_can_data->data[pt_can_data->len] = sum;
        pt_can_data->data[pt_can_data->len + 1] = sum >> 8;

//        t_drv_can[port].s32_can_tx(&t_msg);
        tcu_com_can_send(&t_msg,port);
        for (i = 2, sum = 0; i <= pack; i++, sum++) {
            t_msg.data[0] = i;
            memcpy(&t_msg.data[1], &pt_can_data->data[4 + sum * 7], 7);
//            t_drv_can[port].s32_can_tx(&t_msg);
            tcu_com_can_send(&t_msg,port);
        }
    }
#endif
    return 0;
}

uint32_t u32_simpcan_rx(uint32_t port,T_CAN_SG_DATA *can_multi, T_CAN_SING_DATA *single_can)
{
#if 1
    CAN_MSG_T can_msg;
    uint16_t     sum = 0, i,check=0,flag=0;
//    if (t_drv_can[port].s32_can_rx(&can_msg) == 0) {
    if (can_recv(&can_msg) == 0){
        flag=1;
        if (can_msg.data[0] == 1) {
//            can_multi->u32_pgn = (can_msg.id >> 8) & 0x00FF00;
//            can_multi->u8_dest_addr = (can_msg.id >> 8) & 0xFF;
//            can_multi->u8_src_addr = can_msg.id & 0xFF;
//            can_multi->u8_pri = (can_msg.id >> 26) & 0x7;
            can_multi->id = can_msg.id;
            can_multi->len = 4;
            can_multi->pack_num = can_msg.data[1];
            can_multi->first_len = ((uint16_t)can_msg.data[3] << 8) | can_msg.data[2];
            can_multi->next_pack = 2;
            can_multi->recv_complete=0;
            memcpy(&can_multi->data[0], &can_msg.data[4], 4);
        } else if ((can_msg.data[0] == can_multi->next_pack) && (can_multi->id == can_msg.id)) {
            if ((can_msg.data[0] >= 2) && (can_msg.data[0] <= SG_CAN_MAX_PACK_NUM)) {
                memcpy(&can_multi->data[4 + (can_msg.data[0] - 2) * 7], &can_msg.data[1], 7);
                if (can_multi->next_pack == can_multi->pack_num) {
                    can_multi->next_pack = can_multi->next_pack;
                }
                if (can_multi->next_pack == can_multi->pack_num) {
                    check = ((uint16_t)can_multi->data[can_multi->first_len +1] << 8) + can_multi->data[can_multi->first_len];
                    for (i = 0; i < can_multi->first_len; i++)
                        sum+= can_multi->data[i];
                    sum += can_multi->pack_num;
                    sum += (uint8_t)can_multi->first_len;
                    sum += (uint8_t)(can_multi->first_len >> 8);
                    if (sum == check) {
                        can_multi->recv_complete = 1;
                        can_multi->check_sum=sum;
                    } else {
                        can_multi->recv_complete = 0;
                        can_multi->crc_error=1;
                        can_multi->next_pack=0;
                    }
                }
                ++can_multi->next_pack;
                can_multi->len += 7;
            }

        }else {
            can_multi->next_pack=0;
            can_multi->restart=1;
        }
        single_can->len = can_msg.len;
//        single_can->u32_pgn = (can_msg.id >> 8) & 0x00FF00;
//        single_can->u8_dest_addr = (can_msg.id >> 8) & 0xFF;
//        single_can->u8_src_addr = can_msg.id & 0xFF;
//        single_can->u8_pri = (can_msg.id >> 26) & 0x7;
        single_can->id = can_msg.id;
        memcpy(single_can->data, can_msg.data, 8);

        return 0;
    }
    if(0!=flag){
        return 0;
    }
#endif
    return 1;
}

#if 0
uint32_t u32_sg_forward_tx(uint32_t port,uint8_t src_addr,uint8_t dst_addr,CAN_SG_DATA_T *pt_can_data)
{
    pt_can_data->u8_src_addr=src_addr;
    pt_can_data->u8_dest_addr=dst_addr;
    return u32_sg_can_tx(port,pt_can_data);
}

S32_T   s32_sgcan_set_driver(uint32_t port,S32_CAN_RXTX can_rx,S32_CAN_RXTX can_tx)
{
    t_drv_can[port].s32_can_rx=can_rx;
    t_drv_can[port].s32_can_tx=can_tx;

    return SYS_OK;
}
#endif

