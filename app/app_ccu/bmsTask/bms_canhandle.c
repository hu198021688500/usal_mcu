/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   bms_canhandle
作者:   km
说明:   bms数据处理.
***************************************************************************************************/


#include <string.h>

#include "bms_canhandle.h"           
#include "bms_com.h"
#include "sys_config.h"
#include "log.h"
#include "alarm_task.h"

#include "sgcan.h"
#include "cantcu.h"

BmsFrameStruct bmsFrameGb[] = 
{
    {BMS_BRM_TYPE,GB_BMS_PF_BRM,PT_BMS_PF_BRM,K_CAN_FRAME_BRM_02_START_INDEX,BMS_BRM_DATA_LEN,0},
    {BMS_BCP_TYPE,GB_BMS_PF_BCP,PT_BMS_PF_BCP,K_CAN_FRAME_BCP_06_START_INDEX,BMS_BCP_DATA_LEN,0},
    {BMS_BRO_TYPE,GB_BMS_PF_BRO,PT_BMS_PF_BRO,K_CAN_FRAME_BRO_09_START_INDEX,BMS_BRO_DATA_LEN,0},
    {BMS_BCL_TYPE,GB_BMS_PF_BCL,PT_BMS_PF_BCL,K_CAN_FRAME_BCL_10_START_INDEX,BMS_BCL_DATA_LEN,0},
    {BMS_BCS_TYPE,GB_BMS_PF_BCS,PT_BMS_PF_BCS,K_CAN_FRAME_BCS_11_START_INDEX,BMS_BCS_DATA_LEN,0},
    {BMS_BSM_TYPE,GB_BMS_PF_BSM,PT_BMS_PF_BSM,K_CAN_FRAME_BSM_13_START_INDEX,BMS_BSM_DATA_LEN,0},
    {BMS_BST_TYPE,GB_BMS_PF_BST,PT_BMS_PF_BST,K_CAN_FRAME_BST_19_START_INDEX,BMS_BST_DATA_LEN,0},
    {BMS_BSD_TYPE,GB_BMS_PF_BSD,PT_BMS_PF_BSD,K_CAN_FRAME_BSD_1C_START_INDEX,BMS_BSD_DATA_LEN,0},
    {BMS_BEM_TYPE,GB_BMS_PF_BEM,PT_BMS_PF_BEM,K_CAN_FRAME_BEM_1E_START_INDEX,BMS_BEM_DATA_LEN,0},
    {BMS_BHM_TYPE,GB_BMS_PF_BHM,0xff,K_CAN_FRAME_BHM_27_START_INDEX,BMS_BHM_DATA_LEN,0},
    {BMS_BCM_TYPE,GB_BMS_PF_BCM,GB_BMS_PF_BCM,K_CAN_FRAME_BCM_41_START_INDEX,BMS_BHM_DATA_LEN,0},//HARDWARE_CHECK_MODE
};

static PCanFrame p_mCanFrame[2];
CanHandlerData canHandlerData[2];
static unionCanhandleMode unCanhandleMode[2];


int  send_bms_can_frame(CanTxMsg *canFrame,uint8_t gunSn)
{
    TCanData can_data;
	uint16_t canDevIndex = 0;

    canFrame->IDE = 1;//使用扩展标识符
	canFrame->RTR = 0; /* 设置为数据帧 */

	can_data.length = canFrame->DLC;
	can_data.id_type = canFrame->IDE;
	can_data.frame_type = canFrame->RTR;
	can_data.id = canFrame->ExtId;

	for(uint8_t i=0;i<8;i++)
	{
	    can_data.data[i] = canFrame->Data[i];
	}
	
    canDevIndex = (gunSn == 0)?kSfDevNumCan0:kSfDevNumCan1;
	//DLOG("bms_send[%x]",can_data.id);
    return KfcDevWrite(canDevIndex, &can_data, sizeof(can_data));
}

void canhandle_recv_init(uint8_t gunSn)
{
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        bmsFrameGb[i].flag[gunSn].all = 0;
    }
}

static int frame_gb_pf_to_index(uint8_t pf)
{
    int is_exit_index = -1;
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        if (bmsFrameGb[i].gbPf == pf)
        {
            is_exit_index = i;
            break;
        }
    }
    
    return is_exit_index;
}

static int frame_pt_pf_to_index(uint8_t pf)
{
    int is_exit_index = -1;
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        if (bmsFrameGb[i].ptPf == pf)
        {
            is_exit_index = i;
            break;
        }
    }
    
    return is_exit_index;
}

int frame_id_to_index_gb(uint8_t index)
{
    int is_exit_index = -1;
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        if (bmsFrameGb[i].pfTypeID == index)
        {
            is_exit_index = i;
            break;
        }
    }
    
    return is_exit_index;
}

int frame_id_to_index_pt(uint8_t index)
{
    int is_exit_index = -1;
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        if (bmsFrameGb[i].pfTypeID == index)
        {
            is_exit_index = i;
            break;
        }
    }
    
    return is_exit_index;
}

static int frame_id_to_index(uint8_t index)
{
    int is_exit_index = -1;
    int i;

    for (i = 0; i < sizeof(bmsFrameGb)/sizeof(BmsFrameStruct);i++)
    {
        if (bmsFrameGb[i].pfTypeID == index)
        {
            is_exit_index = i;
            break;
        }
    }
    
    return is_exit_index;
}

static void canhandle_exframe_handle(uint8_t gunSn)
{
    int index = frame_gb_pf_to_index(canHandlerData[gunSn].canLongFrame.pgn);
    if (index < 0)
    {
        return ;
    }
    
    if (bmsDevice[gunSn].unBmsStateA.bit.bProtocolStable == 0)
    {
        if (canHandlerData[gunSn].canLongFrame.pgn == GB_BMS_PF_BRM)
        {
            bmsDevice[gunSn].frameData.brmSa = canHandlerData[gunSn].recvCanHeader.header.da;
        }
        else if(canHandlerData[gunSn].canLongFrame.pgn == GB_BMS_PF_BCP)
        {
            bmsDevice[gunSn].frameData.bcpSa = canHandlerData[gunSn].recvCanHeader.header.da;
        }
    }
    
    if (bmsFrameGb[index].maxLen >= canHandlerData[gunSn].canLongFrame.numberOfBytes)
    {
        memcpy(&bmsDevice[gunSn].frameData.canFrameString[bmsFrameGb[index].dataStartIndex],canHandlerData[gunSn].canLongFrame.data,canHandlerData[gunSn].canLongFrame.numberOfBytes);
        bmsFrameGb[index].flag[gunSn].bit.bexit = 1;
        bmsFrameGb[index].flag[gunSn].bit.brecv = 1;
    }
}

BOOL canhandler_gb2015_long_frame(uint8_t pf)
{
    BOOL ret = BOOL_FALSE;
    
    if((pf==GB_BMS_PF_BRM)||(pf==GB_BMS_PF_BCP)||(pf==GB_BMS_PF_BCS)
      )
    {
        ret = BOOL_TRUE;
    }
    
    return ret;
}

BOOL canhandler_gb_unimportant_long_frame(uint8_t pf)
{
    BOOL ret = BOOL_FALSE;
    
    if((pf==GB_BMS_PF_BMV)||(pf==GB_BMS_PF_BMT)||(pf==GB_BMS_PF_BSP)
      )
    {
        ret = BOOL_TRUE;
    }
    
    return ret;
}


BOOL canhandler_szpt_long_frame(uint8_t pf)
{
    BOOL ret = BOOL_FALSE;
    
    if((pf==PT_BMS_PF_BRM)||(pf==PT_BMS_PF_BCP))
    {
        ret = BOOL_TRUE;
    }
    
    return ret;
}

BOOL canhandler_gb2015_headframe_check(uint8_t gunSn)
{
    BOOL ret = BOOL_TRUE;

    if (!unCanhandleMode[gunSn].bit.bCheckPriorityEnable)//是否检测报文优先级 
    {
        return BOOL_TRUE;//不启动使能的话，直接回复真
    }
    
    switch(canHandlerData[gunSn].recvCanHeader.header.pf)
    {
        //case GB_BMS_PF_BRM:
        //case GB_BMS_PF_BCP:
        case GB_BMS_PF_BCL:
        case GB_BMS_PF_BCS:
        case GB_BMS_PF_BSM:
        case GB_BMS_PF_BSD:
        case GB_BMS_PF_BHM:
        {
            if (canHandlerData[gunSn].recvCanHeader.header.bype1.pp != 0x18)
            {
                ret = BOOL_FALSE;
            }
        }
            break;
       case GB_BMS_PF_BRO:
       case GB_BMS_PF_BST:
        {
            if (canHandlerData[gunSn].recvCanHeader.header.bype1.pp != 0x10)
            {
                ret = BOOL_FALSE;
            }
        }
       break;
       case GB_BMS_PF_BEM:
        {
            if (canHandlerData[gunSn].recvCanHeader.header.bype1.pp != 0x08)
            {
                ret = BOOL_FALSE;
            }
        }
        break;
    }
    
    return ret;
}

static void canhandle_exframe_accpet(const CanRxMsg* frame,uint8_t gunSn)
{
    if (frame->Data[0] == 0x10)
    {
        if(frame->Data[5]||frame->Data[7]) //错误的PNG不响应，也不回复
        {
            return;
        }
        
        //停止上次多包发送
        if ((canHandlerData[gunSn].canLongFrame.pgn != 0) && (canHandlerData[gunSn].canLongFrame.pgn != frame->Data[6]))
        {
            //报文结束应答（TP.CM_EndofMsgAck）
            memset(&canHandlerData[gunSn].m_sendFrame, 0, sizeof(CanTxMsg));
            
            canHandlerData[gunSn].sendCanHeader.header.bype1.pp = 0x1C;
            canHandlerData[gunSn].sendCanHeader.header.pf = CAN_TP_CM_RTS;
            canHandlerData[gunSn].sendCanHeader.header.da = GB_BMS_CAN_ADDR;
            canHandlerData[gunSn].sendCanHeader.header.sa = frame->ExtId;
            
            canHandlerData[gunSn].m_sendFrame.ExtId = canHandlerData[gunSn].sendCanHeader.all;
            

            //取消发送
            canHandlerData[gunSn].m_sendFrame.Data[0] = 0xff;
            canHandlerData[gunSn].m_sendFrame.Data[1] = 0xff;
            canHandlerData[gunSn].m_sendFrame.Data[2] = 0xff;
            canHandlerData[gunSn].m_sendFrame.Data[3] = 0xff;
            canHandlerData[gunSn].m_sendFrame.Data[4] = 0xff;                    
            canHandlerData[gunSn].m_sendFrame.Data[5] = 0x00;
            canHandlerData[gunSn].m_sendFrame.Data[6] = canHandlerData[gunSn].canLongFrame.pgn;
            canHandlerData[gunSn].m_sendFrame.Data[7] = 0x00;
            canHandlerData[gunSn].m_sendFrame.DLC = 8;
            send_bms_can_frame(&canHandlerData[gunSn].m_sendFrame,gunSn);
        }
    }
    else
    {
        return;
    }
    
    canHandlerData[gunSn].canLongFrame.numberOfBytes = frame->Data[1] | (frame->Data[2] << 8);
    canHandlerData[gunSn].canLongFrame.numberOfPackets = frame->Data[3];
    canHandlerData[gunSn].canLongFrame.pgn = frame->Data[6];
    
    //准许发送（TP.CM_CTS）
    memset(&canHandlerData[gunSn].m_sendFrame, 0, sizeof(CanTxMsg));
    canHandlerData[gunSn].sendCanHeader.header.bype1.pp = 0x1C;
    canHandlerData[gunSn].sendCanHeader.header.pf = CAN_TP_CM_RTS;
    canHandlerData[gunSn].sendCanHeader.header.da = canHandlerData[gunSn].recvCanHeader.header.sa;
    canHandlerData[gunSn].sendCanHeader.header.sa = canHandlerData[gunSn].recvCanHeader.header.da;
    
    canHandlerData[gunSn].m_sendFrame.ExtId = canHandlerData[gunSn].sendCanHeader.all;
    canHandlerData[gunSn].m_sendFrame.Data[0] = 0x11;
    canHandlerData[gunSn].m_sendFrame.Data[1] = canHandlerData[gunSn].canLongFrame.numberOfPackets;
    canHandlerData[gunSn].m_sendFrame.Data[2] = 0x01;
    canHandlerData[gunSn].m_sendFrame.Data[3] = 0xFF;
    canHandlerData[gunSn].m_sendFrame.Data[4] = 0xFF;
    canHandlerData[gunSn].m_sendFrame.Data[5] = 0x00;
    canHandlerData[gunSn].m_sendFrame.Data[6] = canHandlerData[gunSn].canLongFrame.pgn;
    canHandlerData[gunSn].m_sendFrame.Data[7] = 0x00;
    canHandlerData[gunSn].m_sendFrame.DLC = 8;
    send_bms_can_frame(&canHandlerData[gunSn].m_sendFrame,gunSn);
    
    canHandlerData[gunSn].canLongFrame.sequenceRx = 1;
    canHandlerData[gunSn].canLongFrame.recvTickMsCnt = 0;
}

static void canhandle_exframe_recv(const CanRxMsg* frame,uint8_t gunSn)
{
    if (canHandlerData[gunSn].canLongFrame.pgn == 0)
    {
        return;
    }
    
    uint8_t sequenceNumber = frame->Data[0];

    if((1 == unCanhandleMode[gunSn].bit.bCheckFrameOrderEnable)
        || (canHandlerData[gunSn].canLongFrame.sequenceRx == sequenceNumber))
    {
        uint8_t offset = (sequenceNumber - 1) * 7;
        canHandlerData[gunSn].canLongFrame.sequenceRx++;
        
        for (uint8_t i = 0; i < 7; i++)
        {
            if (((i + offset) >= canHandlerData[gunSn].canLongFrame.numberOfBytes) || ((i + offset) >= (MAX_CAN_FREAME_DATA_LEN))) 
            {
                break;
            }
           
           canHandlerData[gunSn].canLongFrame.data[i + offset] = frame->Data[i + 1];
           canHandlerData[gunSn].canLongFrame.recvTickMsCnt = 0;
        }
    }
    else
    {
        //准许发送（TP.CM_CTS）
        memset(&canHandlerData[gunSn].m_sendFrame, 0, sizeof(CanTxMsg));
        canHandlerData[gunSn].sendCanHeader.header.bype1.pp = 0x1C;
        canHandlerData[gunSn].sendCanHeader.header.pf = CAN_TP_CM_RTS;
        canHandlerData[gunSn].sendCanHeader.header.da = canHandlerData[gunSn].recvCanHeader.header.sa;
        canHandlerData[gunSn].sendCanHeader.header.sa = canHandlerData[gunSn].recvCanHeader.header.da;
        
        canHandlerData[gunSn].m_sendFrame.ExtId = canHandlerData[gunSn].sendCanHeader.all;
        canHandlerData[gunSn].m_sendFrame.Data[0] = 0x11;
        canHandlerData[gunSn].m_sendFrame.Data[1] = (canHandlerData[gunSn].canLongFrame.numberOfPackets-canHandlerData[gunSn].canLongFrame.sequenceRx)+1;
        canHandlerData[gunSn].m_sendFrame.Data[2] = canHandlerData[gunSn].canLongFrame.sequenceRx;
        canHandlerData[gunSn].m_sendFrame.Data[3] = 0xFF;
        canHandlerData[gunSn].m_sendFrame.Data[4] = 0xFF;
        canHandlerData[gunSn].m_sendFrame.Data[5] = 0x00;
        canHandlerData[gunSn].m_sendFrame.Data[6] = canHandlerData[gunSn].canLongFrame.pgn;
        canHandlerData[gunSn].m_sendFrame.Data[7] = 0x00;
    }
    
    if (sequenceNumber >= canHandlerData[gunSn].canLongFrame.numberOfPackets)
    {
        if (canhandler_gb_unimportant_long_frame(canHandlerData[gunSn].canLongFrame.pgn) == BOOL_TRUE)
        {
            goto canhandle_exframe_recv_end;
        }
    }
    
    if (canHandlerData[gunSn].canLongFrame.sequenceRx > canHandlerData[gunSn].canLongFrame.numberOfPackets)
    {
canhandle_exframe_recv_end:
        //报文结束应答（TP.CM_EndofMsgAck）
        memset(&canHandlerData[gunSn].m_sendFrame, 0, sizeof(CanTxMsg));
        
        canHandlerData[gunSn].sendCanHeader.header.bype1.pp = 0x1C;
        canHandlerData[gunSn].sendCanHeader.header.pf = CAN_TP_CM_RTS;
        canHandlerData[gunSn].sendCanHeader.header.da = canHandlerData[gunSn].recvCanHeader.header.sa;
        canHandlerData[gunSn].sendCanHeader.header.sa = canHandlerData[gunSn].recvCanHeader.header.da;

        canHandlerData[gunSn].m_sendFrame.ExtId = canHandlerData[gunSn].sendCanHeader.all;
        
        canHandlerData[gunSn].m_sendFrame.Data[0] = 0x13;
        canHandlerData[gunSn].m_sendFrame.Data[1] = (unsigned char)(canHandlerData[gunSn].canLongFrame.numberOfBytes & 0xFF);
        canHandlerData[gunSn].m_sendFrame.Data[2] = (unsigned char)((canHandlerData[gunSn].canLongFrame.numberOfBytes >> 8) & 0xFF);
        canHandlerData[gunSn].m_sendFrame.Data[3] = canHandlerData[gunSn].canLongFrame.numberOfPackets;
        canHandlerData[gunSn].m_sendFrame.Data[4] = 0xFF;
        canHandlerData[gunSn].m_sendFrame.Data[5] = 0x00;
        canHandlerData[gunSn].m_sendFrame.Data[6] = canHandlerData[gunSn].canLongFrame.pgn;
        canHandlerData[gunSn].m_sendFrame.Data[7] = 0x00;
        canHandlerData[gunSn].m_sendFrame.DLC = 8;
        send_bms_can_frame(&canHandlerData[gunSn].m_sendFrame,gunSn);
        
        canhandle_exframe_handle(gunSn);
        
        canHandlerData[gunSn].canLongFrame.pgn = 0;
        canHandlerData[gunSn].canLongFrame.recvTickMsCnt = 0;
        canHandlerData[gunSn].canLongFrame.sequenceRx = 1;
        canHandlerData[gunSn].canLongFrame.numberOfPackets = 0;
    }
}

static void canhandle_gb2015_frame_handle(const CanRxMsg* frame,uint8_t gunSn)
{
    switch (canHandlerData[gunSn].recvCanHeader.header.pf)
    {
        case CAN_TP_CM_RTS:
                canhandle_exframe_accpet(frame,gunSn);
             break;
        case CAN_TP_DT_RTS://传输协议：数据（TP.DT）
                canhandle_exframe_recv(frame,gunSn);
             break;
        default:
        {
            int index = frame_gb_pf_to_index(canHandlerData[gunSn].recvCanHeader.header.pf);
            if (index < 0)
            {
                return ;
            }
            
            if (unCanhandleMode[gunSn].bit.bCheckFrameLongEnable)//是否检查报文的长短包
            {
	            if (canhandler_gb2015_long_frame(canHandlerData[gunSn].recvCanHeader.header.pf) == BOOL_TRUE)
	            {
	                return ;
	            }
			}
            
            if (canhandler_gb2015_headframe_check(gunSn) == BOOL_FALSE)
            {
                return ;
            }
                        
            memcpy(&bmsDevice[gunSn].frameData.canFrameString[bmsFrameGb[index].dataStartIndex],frame->Data,frame->DLC);
            bmsFrameGb[index].flag[gunSn].bit.bexit = 1;
            bmsFrameGb[index].flag[gunSn].bit.brecv = 1;
        }
        break;
    }
}



const PCanFrame *get_canheadler_bms_can_frame(BMS_PF_TYPE canIndex,uint8_t clearFlag,uint8_t gunSn)
{
    int8_t is_exit = 0;
    int32_t index = frame_id_to_index(canIndex);
    
    if (index < 0)
    {
        return 0;
    }
    
    if (bmsFrameGb[index].flag[gunSn].bit.brecv)
    {
        p_mCanFrame[gunSn].data = &bmsDevice[gunSn].frameData.canFrameString[bmsFrameGb[index].dataStartIndex];
        p_mCanFrame[gunSn].len = bmsFrameGb[index].maxLen;
        
        is_exit = 1;
        
        if (clearFlag)
        {
            bmsFrameGb[index].flag[gunSn].bit.brecv = 0;
        }
        
        p_mCanFrame[gunSn].flag = bmsFrameGb[index].flag[gunSn];
    }
    
    if (!is_exit)
    {
        return 0;
    }
    
    return &p_mCanFrame[gunSn];
}

int get_bms_can_frame_exit_flag(BMS_PF_TYPE canIndex,uint8_t gunSn)
{
    short index = -1;
    
    index = frame_id_to_index(canIndex);
    
    if (index < 0)
    {
        return 0;
    }
    
    return bmsFrameGb[index].flag[gunSn].bit.bexit;
}

BOOL canhandle_check_id_addr(uint8_t gunSn)
{
    if (!unCanhandleMode[gunSn].bit.bCheckSourceAddrEnable)//是否检查报文的源地址 
    {
        return BOOL_TRUE;//不启动使能的话，直接回复真
    }
    
    if ((BMS_GB_2015_TYPE == sSysConfig.sInt.bmsType)||(BMS_GB_TYPE == sSysConfig.sInt.bmsType))
    {
        //检查目的地址
        if ((canHandlerData[gunSn].recvCanHeader.header.da != GB_CHARGER_CAN_ADDR))
        {
            return BOOL_FALSE;
        }
        
        //检查源地址
        if ((canHandlerData[gunSn].recvCanHeader.header.sa != GB_BMS_CAN_ADDR))
        {
            return BOOL_FALSE;
        }
    }
    
    return BOOL_TRUE;
}

int set_canhandle_work_mode(unionCanhandleMode mode,uint8_t gunSn)
{
    unCanhandleMode[gunSn] = mode;
    
    return 0;
}

void canhandle_task_1ms(void)
{
    TCanData can_data = {0};
	uint16_t canDevIndex = 0;

	for(uint8_t gunSn = 0;gunSn < GUN_SN_MAX;gunSn++)
	{
	    if (canHandlerData[gunSn].canLongFrame.pgn != 0)
	    {
	        if (canHandlerData[gunSn].canLongFrame.recvTickMsCnt < 6000)
	        {
	            canHandlerData[gunSn].canLongFrame.recvTickMsCnt++;
	        }
	        else
	        {
	//            ERROR("gun%d:长帧超时6S",gunSn);
	            canHandlerData[gunSn].canLongFrame.pgn = 0;
	            canHandlerData[gunSn].canLongFrame.recvTickMsCnt = 0;
	        }
	    }
		
		canDevIndex = (gunSn == 0)?kSfDevNumCan0:kSfDevNumCan1;
	    while (KfcDevRead(canDevIndex, &can_data, sizeof(can_data)) > 0)
	    {
       	    uint32_t uExtId_temp = can_data.id;


			if((uExtId_temp&0xff) == 0x8a)
			{
				tcu_can_data.id = can_data.id;
				tcu_can_data.len = can_data.length;
				memcpy(&tcu_can_data.data,can_data.data,can_data.length);
				if(0 == u32_simpcan_rx(0, &t_sg_data.t_rx_data, &t_sg_data.t_sing_data)){
					recv_handle(t_ports,&t_sg_data,&t_config);
				}
			}
			else 
			{
			
			    canHandlerData[gunSn].m_rxMessage.RTR = can_data.frame_type;
			    canHandlerData[gunSn].m_rxMessage.IDE = can_data.id_type;
			    canHandlerData[gunSn].m_rxMessage.DLC = can_data.length;
		        canHandlerData[gunSn].m_rxMessage.ExtId = can_data.id;
				for(uint8_t i=0;i<8;i++)
				{
					canHandlerData[gunSn].m_rxMessage.Data[i] = can_data.data[i];
				}
				
				
		    
		        canHandlerData[gunSn].recvCanHeader.all = canHandlerData[gunSn].m_rxMessage.ExtId;

		//        bms_saveWriteEx(canHandlerData[gunSn].recvCanHeader.all, canHandlerData[gunSn].m_rxMessage.Data, canHandlerData[gunSn].m_rxMessage.DLC);

		        /*if(get_sys_bit(SYS_DATA_BMS_COM_DEBUG_BIT_INDEX))
		        {
		            log_hex("RX", canHandlerData[gunSn].m_rxMessage.Data, canHandlerData[gunSn].m_rxMessage.DLC, canHandlerData[gunSn].m_rxMessage.ExtId);
		        } */       

		        if (canhandle_check_id_addr(gunSn) != BOOL_TRUE)
		        {
		            continue;
		        }
		        
		        switch (sSysConfig.sInt.bmsType)
		        {
		            case BMS_GB_TYPE:
		            case BMS_GB_2015_TYPE:
		            {
		                canhandle_gb2015_frame_handle(&canHandlerData[gunSn].m_rxMessage,gunSn);
		            }
		            break;
		            
		            default:
		            break;
		        }
				
			}
	    }
	}
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/
