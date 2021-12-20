/******************************
    > File Name: cantcu.c
    > Author:Zhx
    > Mail: 862145496@qq.com 
    > Created Time: Sat 18 Sep 2021 02:15:35 AM PDT
 *****************************/
#include "mid_delay.h"
#include "cantcu.h"
#include <string.h>
#include "bms_device.h"
#include "mutual_data.h"
#include "log.h"

/****ceshi include *****/


//static uint32_t instant_fault[2][8];
static uint32_t all_fault[2][8];

//tmp
T_TCU_PORT	t_ports[2];
T_SG_DATA       t_sg_data;
T_SYS_CONFIG    t_config;

//todo km data to switch
//

static uint32_t set_all_fault(uint8_t port,uint32_t fault)
{
	uint8_t i = 0,j = 0;
	i = fault/32;
	j = fault%32;
	all_fault[port][i]|=1<<j;
	return SYS_OK;
}

#if 0
static uint32_t clear_all_fault(uint8_t port,uint32_t fault)
{
	uint8_t i = 0,j = 0;
	i = fault/32;
	j = fault%32;
	all_fault[port][i]&=~(1<<j);
	return SYS_OK;
}

static uint32_t set_real_fault(uint8_t port,uint32_t fault)
{
	uint8_t i = 0,j = 0;
	i = fault/32;
	j = fault%32;
	real_time_fault[port][i]|=1<<j;
	return SYS_OK;
}

static uint32_t clear_real_fault(uint8_t port,uint32_t fault)
{
	uint8_t i = 0,j = 0;
	i = fault/32;
	j = fault%32;
	real_time_fault[port][i]&=~(1<<j);
	return SYS_OK;
}
#endif

static uint32_t fault_check(uint8_t port,uint32_t fault)
{
	uint8_t i = 0,j = 0;
	i=fault/32;
	j=fault%32;
	if(all_fault[port][i]&(1<<j)){
		return 1;
	}else {
		return 0;
	}

}

static T_CANTCU_TX_TABLE	t_cantcu_init[E_TOTAL] ={
		{0x00,0x00,0x00,0x00,0x00},//DILE
		{E_START_CHARGE_ACK,TCU_PGN_START_ACK,4,250,2000},
		{E_STOP_CHARGE_ACK,TCU_PGN_STOP_ACK,4,250,2000},
		{E_TIME_CHECK_ACK,TCU_PGN_TIME_SYNC_ACK,6,500,2000},
		{E_VERSION_CHECK_ACK,TCU_PGN_VERSION_ACK,6,500,2000},
		{E_CHARGE_PARAM_ACK,TCU_PGN_PARAM_ACK,6,500,2000},
		{E_CHARGE_SERVICE_CONTROL_ACK,TCU_PGN_CHARGING_SERVICE_ACK,4,250,2000},
		{E_LOCAK_CONTROL_ACK,TCU_PGN_LOCK_CTRL_ACK,4,250,2000},
		{E_POWER_CONTROL_ACK,TCU_PGN_ADJUST_POWER_ACK,4,250,2000},
		{E_START_CHARGE_COMPLETE,TCU_PGN_START_FINISH,4,250,5000},
		{E_STOP_CHARGE_COMPLETE,TCU_PGN_STOP_FINISH,4,250,5000},
		{E_CHARGE_STATE1,TCU_PGN_DC_STATE,6,500,5000},
		{E_CHARGE_STATE2,TCU_PGN_AC_STATE,6,500,5000},
		{E_DC_YC,TCU_PGN_YC,6,1000,1},
		{E_DC_YX1,TCU_PGN_YX,4,500,1},
		{E_DC_YX2,TCU_PGN_YX1,4,500,1},
		{E_VIN_VERITY,TCU_PGN_VIN,6,1000,60000},
		{E_AC_YC,TCU_PGN_AC_YC,6,1000,1},
		{E_AC_YX1,TCU_PGN_AC_YX,4,500,1},
		{E_AC_YX2,TCU_PGN_AC_YX1,4,500,1},
		{E_HEART,TCU_PGN_TICK_ACK,6,1000,1},
		{E_ERROR,TCU_PGN_TIMEOUT,4,250,5000},
		{E_CHARGE_CFG_INQUIRE_ACK,TCU_PGN_CONFIG_ACK,6,500,2000},
		{E_UPLOAD_HEART_ACK,TCU_PGN_UPGRADE_TICK_ACK,4,1000,1},
		{E_START_UPLOAD_ACK,TCU_PGN_START_UPLOAD_ACK,4,500,2000},
		{E_ASK_FOR_BLOAK_ACK1,TCU_PGN_REQUEST_INTERVAL_ACK1,4,500,2000},
		{E_ASK_FOR_BLOAK_ACK2,TCU_PGN_REQUEST_INTERVAL_ACK2,4,500,2000},
		{E_START_GROUP_PACK_ACK,TCU_PGN_START_GROUP_PACK_ACK,4,500,2000},
		{E_FULFILL_GROUP_PACK_ACK,TCU_PGN_FULFILL_GROUP_PACK_ACK,4,500,2000},
		{E_PROGRAM_CHECK_ACK,TCU_PGN_PROGRAM_CHECK_ACK,4,500,2000},
		{E_IMMEDIATELY_RESET_ACK,TCU_PGN_RESET_INMEDIATELY_ACK,4,500,1},
		{E_CONSTANT_VALUE_SET_ACK,TCU_PGN_SETTING_ACK,6,1,1},
		{E_CONSTANT_VALUE_INQUIRE_ACK,TCU_PGN_SETTING_QUERY_ACK,6,1,1},
		{E_MODULE_VERITY,TCU_PGN_MODULE_VERIFY,6,1,1},
		{E_ASSIST_PARAM_ACK,TCU_PGN_PARAM_SET_ACK,6,1,2000},
		{E_CMD_CONTROL_ACK,TCU_PGN_CONTROL_CMD_ACK,6,1,2000},
};


static int32_t check_start_ack_data(uint8_t port,T_TCU_PORT* pt_port)
{
	uint8_t fause = 0;
	if(0 != fault_check(port,E_FAULT_TCU_COMMUNICATION)){
		fause = 0x02;
	}else if(0 == pt_port->t_tcu_flag.bits.get_version){
		fause = 0x03;
	}else if(0 == pt_port->t_tcu_flag.bits.get_param){
		fause = 0x04;
	}
	//充电桩暂停服务
	/*else if(0 != fault_check(port,E_FAULT_COLLECT_EMG)){
		fause = 0x05;
	}*/else if(pt_port->flow_state > E_TCU_IDLE){
		fause = 0x06;
	}
	//充电桩处于暂停状态
	/*else if(pt_port->flow_state > E_TCU_IDLE){
		fause = 0x07;
	}*/else if(0 != pt_port->t_yx1._ccu_di_carconnect){
		fause = 0x08;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_EMG)){
		fause = 0x09;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_DOOR)){
		fause = 0x0A;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_SPD)){
		fause = 0x0B;
	}else if(0 != fault_check(port,E_FAULT_SMOG)){
		fause = 0x0C;
	}else if(0 != fault_check(port,E_FAULT_TEMP_OVER)){
		fause = 0x0D;
	}else if(0 != fault_check(port,E_FAULT_GUN_TEMP_OVER)){
		fause = 0x0E;
	}else if(0 != fault_check(port,E_FAULT_LOCK1_LOST_CTRL)){
		fause = 0x0F;
	}else if(0 != fault_check(port,E_FAULT_FLOW_INSLT)){
		fause = 0x10;
	}else if(0 != fault_check(port,E_FAULT_BAT_REVERSE)){
		fause = 0x11;
	}else if(0 != fault_check(port,E_FAULT_BMS_COMMUNICATION)){
		fause = 0x12;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_BREAKER)){
		fause = 0x13;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_RELAY)){
		fause = 0x14;
	}else if(0 != fault_check(port,E_FAULT_AC_RELAY_STICKY)){
		fause = 0x15;
	}else if((0 != fault_check(port,E_FAULT_DC_RELAY_P))
			|| (0 != fault_check(port,E_FAULT_DC_RELAY_N))){
		fause = 0x16;
	}else if(0 != fault_check(port,E_FAULT_RELAY1_STICKY)){
		fause = 0x17;
	}else if(0 != fault_check(port,E_FAULT_DC_FUSE)){
		fause = 0x18;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_LOST_CTRL)){
		fause = 0x19;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_STICKY)){
		fause = 0x1A;
	}else if((0 != fault_check(port,E_FAULT_FLOW_BLEED))
			||(0 != fault_check(port,E_FAULT_FLOW_BLEED_TO))
			||(0 != fault_check(port,E_FAULT_COLLECT_BLEED))){
		fause = 0x1B;
	}else if(0 != fault_check(port,E_FAULT_BOARD_POWER)){
		fause = 0x1C;
	}else if(0 != fault_check(port,E_FAULT_RECT_COMM)){
		fause = 0x1D;
	}else if((0!=fault_check(port,E_FAULT_AC_VOL_BELOW))
            ||(0!=fault_check(port,E_FAULT_AC_VOL_OVER))
            ||(0!=fault_check(port,E_FAULT_AC_PHASE_LOSS))){
		fause = 0x1E;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_OVER)){
		fause = 0x1F;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		fause = 0x20;
	}else if(0 != fault_check(port,E_FAULT_DC_CUR_OVER)){
		fause = 0x21;
	}else if(0 != fault_check(port,E_FAULT_DC_SHORT_CUT)){
		fause = 0x22;
	}else if(0 != fault_check(port,E_FAULT_NO_RECT_AVAILABLE)){
		fause = 0x23;
	}
	//预留
	/*else if(0 != fault_check(port,E_FAULT_BMS_COMMUNICATION)){
		fause = 0x24;
	}
	*/else if(0 != fault_check(port,E_FAULT_COLLECT_DOOR_B)){
		fause = 0x25;
	}else if(0 != fault_check(port,E_FAULT_WATER)){
		fause = 0x26;
	}else if(0 != fault_check(port,E_FAULT_WATER_BACK)){
		fause = 0x27;
	}else if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		fause = 0x28;
	}else if(0 != fault_check(port,E_FAULT_SWITCH_COMM)){
		fause = 0x29;
	}else if(0 != fault_check(port,E_FAULT_SWITCH_COMM)){
		fause = 0x2A;
	}
	//绝缘监测仪通信故障
	/*
	else if(0 != fault_check(port,E_FAULT_BMS_COMMUNICATION)){
		fause = 0x2B;
	}*/
	//环境监控板通信故障
	/*
	else if(0 != fault_check(port,E_FAULT_BMS_COMMUNICATION)){
		fause = 0x2C;
	}*/
	else if(0 != fault_check(port,E_FAULT_LED_COMM)){
		fause = 0x2D;
	}else if(0 != fault_check(port,E_FAULT_METER_COMM)){
		fause = 0x2E;
	}
	//充电桩其他故障
	/*
	 else if(0 != fault_check(port,E_FAULT_BMS_COMMUNICATION)){
		fause = 0xFF;
	}*/
	pt_port->t_start_ack_data.fail_cause = fause;
	return SYS_OK;
}

static int32_t send_start_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_START_CHARGE_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_START_CHARGE_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    pt_msg->data[1] = pt_port->t_start_data.load_switch;
    check_start_ack_data(port,pt_port);
    if(0 != pt_port->t_start_ack_data.fail_cause){
    	pt_port->t_start_ack_data.success_sign = 1;
    }else{
    	pt_port->t_start_ack_data.success_sign = 0;
    }
    pt_msg->data[2] = pt_port->t_start_ack_data.success_sign;
    pt_msg->data[3] = pt_port->t_start_ack_data.fail_cause;

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}

static int32_t send_stop_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_STOP_CHARGE_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_STOP_CHARGE_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    pt_msg->data[1] = pt_port->t_stop_ack_data.success_sign;

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}

static int32_t send_time_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_TIME_CHECK_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_TIME_CHECK_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    pt_msg->data[1] = pt_port->t_stop_ack_data.success_sign;

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 发送版本应答
 */
static int32_t send_version_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_VERSION_CHECK_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_VERSION_CHECK_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    pt_msg->data[1] = CAN_CCU_VERSION >> 8;
    pt_msg->data[2] = CAN_CCU_VERSION & 0xff;
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 *
 */
static int32_t send_charge_param_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CHARGE_PARAM_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CHARGE_PARAM_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    if(0 != pt_port->t_tcu_flag.bits.cumm_fault){
    	pt_msg->data[2] = 2;
    }
    if(0 == pt_port->t_tcu_flag.bits.get_version){
    	pt_msg->data[2] = 3;
    }
    if(0 != pt_msg->data[2]){
    	pt_msg->data[3] = 1;
    }
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 充电服务启停应答
 */
static int32_t send_charge_service_control_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CHARGE_SERVICE_CONTROL_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CHARGE_SERVICE_CONTROL_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 电子锁控制应答
 */
static int32_t send_lock_control_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_LOCAK_CONTROL_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_LOCAK_CONTROL_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 功率控制应答
 */
static int32_t send_power_control_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_POWER_CONTROL_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_POWER_CONTROL_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}

#if 0
static int32_t check_start_complete_data(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	uint8_t fause = 0;
	if(0 != fault_check(port,E_FAULT_GUN_CC)){
		fause = 0x01;
	}else if(0 != fault_check(port,E_FAULT_TCU_COMMUNICATION)){
		fause = 0x02;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_EMG)){
		fause = 0x03;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_DOOR)){
		fause = 0x04;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_SPD)){
		fause = 0x05;
	}else if(0 != fault_check(port,E_FAULT_SMOG)){
		fause = 0x06;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_BREAKER)){
		fause = 0x07;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_RELAY)){
		fause = 0x08;
	}else if(0 != fault_check(port,E_FAULT_AC_RELAY_STICKY)){
		fause = 0x09;
	}else if((0!=fault_check(port,E_FAULT_AC_VOL_OVER))
            ||(0!=fault_check(port,E_FAULT_AC_VOL_BELOW))
            ||(0!=fault_check(port,E_FAULT_AC_PHASE_LOSS))){
		fause = 0x0a;
    }else if(0 != fault_check(port,E_FAULT_TEMP_OVER)){
		fause = 0x0b;
	}else if(0 != fault_check(port,E_FAULT_GUN_TEMP_OVER)){
		fause = 0x0c;
	}else if((0!=fault_check(port,E_FAULT_LOCK1_STICKY))
            ||(0!=fault_check(port,E_FAULT_LOCK1_LOST_CTRL))){
		fause = 0x0d;
    }else if(0 != fault_check(port,E_FAULT_FLOW_INSLT)){
		fause = 0x0e;
	}else if(0 != fault_check(port,E_FAULT_BAT_REVERSE)){
		fause = 0x0f;
	}else if((0!=fault_check(port,E_FAULT_DC_RELAY_P))
            ||(0!=fault_check(port,E_FAULT_DC_RELAY_N))){
		fause = 0x10;
    }else if(0 != fault_check(port,E_FAULT_RELAY1_STICKY)){
		fause = 0x11;
	}else if(0 != fault_check(port,E_FAULT_DC_FUSE)){
		fause = 0x12;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_LOST_CTRL)){
		fause = 0x13;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_STICKY)){
		fause = 0x14;
	}else if((0!=fault_check(port,E_FAULT_FLOW_BLEED))
            ||(0!=fault_check(port,E_FAULT_FLOW_BLEED_TO))
            ||(0!=fault_check(port,E_FAULT_COLLECT_BLEED))){
		fause = 0x15;
    }else if(0 != fault_check(port,E_FAULT_BOARD_POWER)){
		fause = 0x16;
	}else if(0 != fault_check(port,E_FAULT_RECT_COMM)){
		fause = 0x17;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_OVER)){
		fause = 0x18;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		fause = 0x19;
	}else if(0 != fault_check(port,E_FAULT_DC_CUR_OVER)){
		fause = 0x1A;
	}else if(0 != fault_check(port,E_FAULT_DC_SHORT_CUT)){
		fause = 0x1B;
	}else if(0 != fault_check(port,E_FAULT_BAT_VOL_MAX_BELOW_RECT_MIN)){
		fause = 0x1C;
	}else if(0 != fault_check(port,E_FAULT_FLOW_OVER10)){
		fause = 0x1D;
	}else if(0 != fault_check(port,E_FAULT_FLOW_5PERCENT)){
		fause = 0x1E;
	}else if(0 != fault_check(port,E_FAULT_PRECHARGE_BAT_VOL_BLELOW_RECT_MIN)){
		fause = 0x1F;
	}else if(0 != fault_check(port,E_FAULT_PRECHARGE_BAT_VOL_OVER_RECT_MAX)){
		fause = 0x20;
	}else if(0 != fault_check(port,E_FAULT_VOL_OVER_BAT_VOL_MAX)){
		fause = 0x21;
	}else if(0 != fault_check(port,E_FAULT_BMS_BRM_TO)){
		fause = 0x22;
	}else if(0 != fault_check(port,E_FAULT_BRM_DATA_ERROR)){
		fause = 0x23;
	}else if(0 != fault_check(port,E_FAULT_BMS_BCP_TO)){
		fause = 0x24;
	}else if(0 != fault_check(port,E_FAULT_BCP_DATA_ERROR)){
		fause = 0x25;
	}else if(0 != fault_check(port,E_FAULT_BMS_BRO_00_TO)){
		fause = 0x26;
	}else if(0 != fault_check(port,E_FAULT_BMS_BRO_AA_TO)){
		fause = 0x27;
	}
	//28 预留
	/*
	else if(0 != fault_check(port,E_FAULT_FLOW_OVER10)){
		fause = 0x28;
	}*/
	else if(0 != fault_check(port,E_FAULT_BMS_BCL_TO)){
		fause = 0x29;
	}else if(0 != fault_check(port,E_FAULT_BMS_BCS_TO)){
		fause = 0x2a;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_DOOR_B)){
		fause = 0x2b;
	}else if(0 != fault_check(port,E_FAULT_WATER)){
		fause = 0x2c;
	}else if(0 != fault_check(port,E_FAULT_WATER_BACK)){
		fause = 0x2d;
	}else if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		fause = 0x2e;
	}else if(0 != fault_check(port,E_FAULT_SWITCH_COMM)){
		fause = 0x2f;
	}else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x30;
	}else if(0 != fault_check(port,E_FAULT_BMS_VER)){
		fause = 0x31;
	}
	//绝缘监测仪通信故障
	/*else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x32;
	}*/
	else if(0 != fault_check(port,E_FAULT_SELF_CHECKING)){
		fause = 0x33;
	}else if(0 != fault_check(port,E_FAULT_POWER_BUDGET)){
		fause = 0x34;
	}
	//预充阶段调压失败
	/*else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x35;
	}*/
	else if(0 != fault_check(port,E_FAULT_TCU_START_TIMEOUT)){
		fause = 0x36;
	}else if(0 != fault_check(port,E_FAULT_RECT_START_TO)){
		fause = 0x37;
	}
	//环境监控板通信故障
	/*else if(0 != fault_check(port,E_FAULT_RECT_START_TO)){
		fause = 0x38;
	}*/
	//自检功率分配超时
	/*else if(0 != fault_check(port,E_FAULT_RECT_START_TO)){
		fause = 0x39;
	}*/
	else if(0 != fault_check(port,E_FAULT_LED_COMM)){
		fause = 0x3a;
	}else if(0 != fault_check(port,E_FAULT_FLOW_INSLT_TO)){
		fause = 0x3b;
	}else if(0 != fault_check(port,E_FAULT_METER_COMM)){
		fause = 0x3c;
	}else if(0 != fault_check(port,E_FAULT_VIN_VERIFY_TIMEOUT)){
		fause = 0x3d;
	}else if(0 != fault_check(port,E_FAULT_TCU_VIN_VERIFY_FAIL)){
		fause = 0x3e;
	}else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0x3f;
	}
	//功率控制模块故障 \ BRO准备就绪后取消 \ 充电桩水浸故障 \充电机其他故障
	/*
	else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0x40;
	}else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0x41;
	}else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0x42;
	}else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0xff;
	}*/
	pt_port->start_fause = fause;
	return SYS_OK;
}
#endif

/*
 * 启动完成
 */
static int32_t send_start_complete(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_START_FINISH *pt_data = (T_START_FINISH *)pt_msg->data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_START_CHARGE_COMPLETE].pri;
	id.bits.PF = (t_cantcu_init[E_START_CHARGE_COMPLETE].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(T_START_FINISH);

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_data->port = 0;
    pt_data->load_switch = pt_port->t_start_data.load_switch;
    if(0 != pt_port->start_fause){
    	pt_data->start_result = 1;
    }else{
    	pt_data->start_result = 0;
    }
    pt_data->cause_failure = pt_port->start_fause;
    memcpy(pt_data->ccu_bms_ver,pt_port->ccu_bms_ver,sizeof(pt_data->ccu_bms_ver));
    memcpy(pt_data->bms_ccu_ver,pt_port->bms_ccu_ver,sizeof(pt_data->bms_ccu_ver));
    pt_data->handshake_result = pt_port->handshake_result;
    pt_data->battery_type = pt_port->battery_type;
    pt_data->max_allow_temp = (uint8_t)(pt_port->max_allow_temp + 50);
    pt_data->bms_max_allow_voltage = (uint16_t)(pt_port->bms_max_allow_voltage*10);
    pt_data->unit_max_allow_voltage = (uint16_t)(pt_port->unit_max_allow_voltage*100);
    pt_data->max_allow_electricity = (uint16_t)((400- pt_port->max_allow_electricity)*10);
    pt_data->battery_total_voltage = (uint16_t)(pt_port->battery_total_voltage*10);
    pt_data->battery_current_voltage = (uint16_t)(pt_port->battery_current_voltage*10);
    pt_data->battery_rated_capacity = (uint16_t)(pt_port->battery_rated_capacity*10);
    pt_data->battery_nominal_energy = (uint16_t)(pt_port->battery_nominal_energy*10);
    pt_data->battery_capacity = (uint16_t)(pt_port->battery_capacity*10);
    pt_data->ccu_max_out_voltage = (uint16_t)(pt_port->ccu_max_out_voltage*10);
    pt_data->ccu_min_out_voltage = (uint16_t)(pt_port->ccu_min_out_voltage*10);
    pt_data->ccu_max_out_electricity = (uint16_t)((400- pt_port->ccu_max_out_electricity)*10);
    pt_data->ccu_min_out_electricity = (uint16_t)((400- pt_port->ccu_min_out_electricity)*10);
    memcpy(pt_data->vin,pt_port->vin,sizeof(pt_port->vin));
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}

static int32_t check_stop_complete_data(uint8_t port,T_TCU_PORT* pt_port)
{
	uint8_t fause = 0;
	if(0 != fault_check(port,E_FAULT_GUN_CC)){
		fause = 0x05;
	}else if(0 != fault_check(port,E_FAULT_TCU_COMMUNICATION)){
		fause = 0x06;
	}
	//充电桩充电中暂停超时（bms暂停超时）
	/*else if(0 != fault_check(port,E_FAULT_TCU_COMMUNICATION)){
		fause = 0x07;
	}*/
	else if(0 != fault_check(port,E_FAULT_COLLECT_EMG)){
		fause = 0x08;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_DOOR)){
		fause = 0x09;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_SPD)){
		fause = 0x0a;
	}else if(0 != fault_check(port,E_FAULT_SMOG)){
		fause = 0x0b;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_BREAKER)){
		fause = 0x0c;
	}else if(0 != fault_check(port,E_FAULT_COLLECT_AC_RELAY)){
		fause = 0x0d;
	}else if(0 != fault_check(port,E_FAULT_AC_RELAY_STICKY)){
		fause = 0x0e;
	}else if((0!=fault_check(port,E_FAULT_AC_VOL_OVER))
            ||(0!=fault_check(port,E_FAULT_AC_VOL_BELOW))
            ||(0!=fault_check(port,E_FAULT_AC_PHASE_LOSS))){
		fause = 0x0f;
    }else if(0 != fault_check(port,E_FAULT_TEMP_OVER)){
		fause = 0x10;
	}else if(0 != fault_check(port,E_FAULT_GUN_TEMP_OVER)){
		fause = 0x11;
	}else if((0!=fault_check(port,E_FAULT_LOCK1_STICKY))
            ||(0!=fault_check(port,E_FAULT_LOCK1_LOST_CTRL))){
		fause = 0x12;
    }else if((0!=fault_check(port,E_FAULT_DC_RELAY_P))
            ||(0!=fault_check(port,E_FAULT_DC_RELAY_N))){
		fause = 0x13;
    }else if(0 != fault_check(port,E_FAULT_RELAY1_STICKY)){
		fause = 0x14;
	}else if(0 != fault_check(port,E_FAULT_DC_FUSE)){
		fause = 0x15;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_LOST_CTRL)){
		fause = 0x16;
	}else if(0 != fault_check(port,E_FAULT_M_RELAY_STICKY)){
		fause = 0x17;
	}else if((0!=fault_check(port,E_FAULT_FLOW_BLEED))
            ||(0!=fault_check(port,E_FAULT_FLOW_BLEED_TO))
            ||(0!=fault_check(port,E_FAULT_COLLECT_BLEED))){
		fause = 0x18;
    }else if(0 != fault_check(port,E_FAULT_BOARD_POWER)){
		fause = 0x19;
	}else if(0 != fault_check(port,E_FAULT_RECT_COMM)){
		fause = 0x1a;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_OVER)){
		fause = 0x1b;
	}else if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		fause = 0x1c;
	}else if(0 != fault_check(port,E_FAULT_DC_CUR_OVER)){
		fause = 0x1d;
	}else if(0 != fault_check(port,E_FAULT_DC_SHORT_CUT)){
		fause = 0x1e;
	}else if(0 != fault_check(port,E_FAULT_BMS_BCL_TO)){
		fause = 0x1f;
	}else if(0 != fault_check(port,E_FAULT_BMS_BCS_TO)){
		fause = 0x20;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_TO)){
		fause = 0x21;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_CELL_OV)){
		fause = 0x22;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_CELL_LV)){
		fause = 0x23;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_SOC_OVER)){
		fause = 0x24;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_SOC_LOWER)){
		fause = 0x25;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_BAT_OC)){
		fause = 0x26;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_BAT_OT)){
		fause = 0x27;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_INSLT)){
		fause = 0x28;
	}else if(0 != fault_check(port,E_FAULT_BMS_BSM_CONNECTOR)){
		fause = 0x29;
	}else if(0 != fault_check(port,E_STOP_BMS_NORMAL)){
		fause = 0x2a;
	}else if(0 != fault_check(port,E_STOP_BMS_FAULT)){
		fause = 0x2b;
	}else if(0 != fault_check(port,E_STOP_BMS_CHARGE_FAULT)){
		fause = 0x2c;
	}
	//预留
	/*else if(0 != fault_check(port,E_STOP_BMS_CHARGE_FAULT)){
		fause = 0x2d;
	}*/
	else if(0 != fault_check(port,E_FAULT_NO_CUR)){
		fause = 0x2e;
	}
	//环境监控板通信故障
	/*else if(0 != fault_check(port,E_STOP_BMS_CHARGE_FAULT)){
		fause = 0x2f;
	}*/
	else if(0 != fault_check(port,E_FAULT_LED_COMM)){
		fause = 0x30;
	}else if(0 != fault_check(port,E_FAULT_METER_COMM)){
		fause = 0x31;
	}else if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		fause = 0x32;
	}
	//功率控制模块故障
	/*
	else if(0 != fault_check(port,E_FAULT_LED_COMM)){
		fause = 0x33;
	}*/
	else if(0 != fault_check(port,E_FAULT_SWITCH_COMM)){
		fause = 0x34;
	}else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x35;
	}else if(0 != fault_check(port,E_FAULT_FAN)){
		fause = 0x36;
	}else if(0 != fault_check(port,E_FAULT_WATER)){
		fause = 0x37;
	}
	//充电机输出电流与BCS报文电流不匹配 \ 充电机输出电压与BCS报文电压不匹配 \ 充电桩其他故障
	/*else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x38;
	}else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0x39;
	}else if(0 != fault_check(port,E_FAULT_SWITCH)){
		fause = 0xff;
	}
	*/
	pt_port->stop_fause = fause;
	return SYS_OK;
}

/*
 * 停止完成
 */
static int32_t send_stop_complete(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_STOP_FINISH *pt_data = (T_STOP_FINISH *)pt_msg->data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_STOP_CHARGE_COMPLETE].pri;
	id.bits.PF = (t_cantcu_init[E_STOP_CHARGE_COMPLETE].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(T_STOP_FINISH);
    check_stop_complete_data(port,pt_port);
    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_data->port = 0;
    pt_data->stop_reason = pt_port->stop_fause;
    pt_data->stop_result = pt_port->stop_result;
    pt_data->t_bst = pt_port->t_bst;
    pt_data->stop_soc_status = pt_port->stop_soc;
    pt_data->battery_monomer_min_voltage = (uint16_t)(pt_port->batt_min_vol*100);
    pt_data->battery_monomer_max_voltage = (uint16_t)(pt_port->batt_max_vol*100);
    pt_data->battery_min_temp = (uint8_t)(pt_port->batt_min_temp+50);
    pt_data->battery_max_temp = (uint8_t)(pt_port->batt_max_temp+50);
    pt_data->t_bem = pt_port->t_bem;
    pt_data->bms_brm_timeout = fault_check(port,E_FAULT_BMS_BRM_TO);
    pt_data->bms_bcp_timeout = fault_check(port,E_FAULT_BMS_BCP_TO);
    pt_data->bms_broaa_timeout = fault_check(port,E_FAULT_BMS_BRO_AA_TO);
    pt_data->bms_bcs_timeout = fault_check(port,E_FAULT_BMS_BCS_TO);
    pt_data->bms_bcl_timeout = fault_check(port,E_FAULT_BMS_BCL_TO);
    pt_data->bms_bst_timeout = fault_check(port,E_FAULT_BMS_BST_TO);
    pt_data->bms_bsd_timeout = fault_check(port,E_FAULT_BMS_BSD_TO);

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 充电桩状态信息
 */
static int32_t send_charge_state1(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CHARGE_STATE1].pri;
	id.bits.PF = (t_cantcu_init[E_CHARGE_STATE1].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}

//交流的暂时不做
static int32_t send_charge_state2(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CHARGE_STATE2].pri;
	id.bits.PF = (t_cantcu_init[E_CHARGE_STATE2].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 发送遥测数据
 */
static int32_t send_dc_yc(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_YC        *pt_data = (T_YC *)pt_msg->data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_DC_YC].pri;
	id.bits.PF = (t_cantcu_init[E_DC_YC].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(T_YC);

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_data->port = 0;
    pt_data->out_vol = (uint16_t)pt_port->out_vol*10;
    pt_data->out_cur = (uint16_t)(400-pt_port->out_cur)*10;
    pt_data->soc = pt_port->soc;
    pt_data->bat_min_temp = (uint8_t)(pt_port->bat_min_temp + 50);
    pt_data->bat_max_temp = (uint8_t)(pt_port->bat_max_temp + 50);
    pt_data->cell_max_vol = (uint16_t)pt_port->cell_max_vol*100;
    pt_data->cell_min_vol = (uint16_t)pt_port->cell_min_vol*100;
    pt_data->dev_temp = (uint8_t)(pt_port->dev_temp + 50);
    pt_data->guide_vol = (uint16_t)pt_port->guide_vol*100;
    pt_data->bms_need_vol = (uint16_t)pt_port->bms_need_vol*10;
    pt_data->bms_need_cur = (uint16_t)(400 - pt_port->bms_need_cur)*10;
    pt_data->charge_type = pt_port->charge_type;
    pt_data->bms_meas_vol = (uint16_t)pt_port->bms_meas_vol*10;
    pt_data->bms_meas_cur = (uint16_t)(400 - pt_port->bms_meas_cur)*10;
    pt_data->remain_charge_time = pt_port->remain_charge_time;
    pt_data->dc1_temp = (uint8_t)(pt_port->dc1_temp + 50);
    pt_data->dc2_temp = (uint8_t)(pt_port->dc2_temp + 50);
    pt_data->dc3_temp = (uint8_t)(pt_port->dc3_temp + 50);
    pt_data->dc4_temp = (uint8_t)(pt_port->dc4_temp + 50);
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 采集遥信1数据
 */
static int32_t check_yx1_data(uint8_t port,T_TCU_PORT* pt_port)
{
	//检查桩状态
	if(E_TCU_IDLE == pt_port->flow_state){
		pt_port->t_yx1.working_status = 0;
	}else if(E_TCU_IDLE < pt_port->flow_state){
		pt_port->t_yx1.working_status = 1;
	}else if(E_TCU_STOP_FINISH == pt_port->flow_state){
		pt_port->t_yx1.working_status = 2;
	}else if(E_TCU_SUSPEND == pt_port->flow_state){
		pt_port->t_yx1.working_status = 3;
	}

	if(0 != fault_check(port,E_FAULT_COLLECT_EMG)){
		pt_port->t_yx1.exigency_failure = 1;
	}else{
		pt_port->t_yx1.exigency_failure = 0;
	}
	if(0 != fault_check(port,E_FAULT_SMOG)){
		pt_port->t_yx1.smoke_failure = 1;
	}else {
		pt_port->t_yx1.smoke_failure = 0;
	}
	if(0 != fault_check(port,E_FAULT_COLLECT_AC_BREAKER)){
		pt_port->t_yx1.ac_input_breaker_failure = 1;
	}else{
		pt_port->t_yx1.ac_input_breaker_failure = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_RELAY_P) || 0 != fault_check(port,E_FAULT_DC_RELAY_N)){
		pt_port->t_yx1.failure_28 = 1;
	}else{
		pt_port->t_yx1.failure_28 = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_FUSE)){
		pt_port->t_yx1._ccu_di_fuse = 1;
	}else{
		pt_port->t_yx1._ccu_di_fuse = 0;
	}
	if(0 != fault_check(port,E_FAULT_LOCK1_LOST_CTRL)){
		pt_port->t_yx1._ccu_di_gunlock = 1;
	}else{
		pt_port->t_yx1._ccu_di_gunlock = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_FAN)){
		pt_port->t_yx1._ccu_di_fan = 1;
	}else{
		pt_port->t_yx1._ccu_di_fan = 0;
	}
	if(0 != fault_check(port,E_FAULT_COLLECT_SPD)){
		pt_port->t_yx1._ccu_di_spd = 1;
	}else{
		pt_port->t_yx1._ccu_di_spd = 0;
	}
	if(0 != fault_check(port,E_FAULT_FLOW_INSLT)){
		pt_port->t_yx1._ccu_di_iso = 1;
	}else{
		pt_port->t_yx1._ccu_di_iso = 0;
	}
	if(0 != fault_check(port,E_FAULT_BAT_REVERSE)){
		pt_port->t_yx1._ccu_di_battery = 1;
	}else{
		pt_port->t_yx1._ccu_di_battery = 0;
	}
	if(0 != fault_check(port,E_FAULT_GUN_CC)){
		pt_port->t_yx1._ccu_di_daoyin = 1;
	}else{
		pt_port->t_yx1._ccu_di_daoyin = 0;
	}
	if(0 != fault_check(port,E_FAULT_TEMP_OVER)){
		pt_port->t_yx1._ccu_di_piletemperr = 1;
	}else{
		pt_port->t_yx1._ccu_di_piletemperr = 0;
	}
	if(0 != fault_check(port,E_FAULT_GUN_TEMP_OVER)){
		pt_port->t_yx1._ccu_di_guntemperr = 1;
	}else{
		pt_port->t_yx1._ccu_di_guntemperr = 0;
	}
	if(0 != fault_check(port,E_ALARM_GUN_PARKED)){
		pt_port->t_yx1._ccu_di_gunposition = 1;
	}else{
		pt_port->t_yx1._ccu_di_gunposition = 0;
	}
	if(0 != fault_check(port,E_ALARM_GUN_PARKED)){
		pt_port->t_yx1._ccu_di_bmscom = 1;
	}else{
		pt_port->t_yx1._ccu_di_bmscom = 0;
	}
	if(0 != fault_check(port,E_FAULT_AC_VOL_OVER)){
		pt_port->t_yx1._ccu_di_voltoverin = 1;
	}else{
		pt_port->t_yx1._ccu_di_voltoverin = 0;
	}
	if(0 != fault_check(port,E_FAULT_AC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_voltlessin = 1;
	}else{
		pt_port->t_yx1._ccu_di_voltlessin = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_VOL_OVER)){
		pt_port->t_yx1._ccu_di_busvoltover = 1;
	}else{
		pt_port->t_yx1._ccu_di_busvoltover = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_busvoltless = 1;
	}else{
		pt_port->t_yx1._ccu_di_busvoltless = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_CUR_OVER)){
		pt_port->t_yx1._ccu_di_buscurrover = 1;
	}else{
		pt_port->t_yx1._ccu_di_buscurrover = 0;
	}
	//todo
	if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_rmerr = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmerr = 0;
	}
	if(0 != fault_check(port,E_FAULT_DC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_rmacerr = 1;
	}
	/***********************************/
	if(0 != fault_check(port,E_FAULT_RECT_AC_VOL_OVER)){
		pt_port->t_yx1._ccu_di_rmacvoltover = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmacvoltover = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_AC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_rmacvoltless = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmacvoltless = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_AC_PHASE_LOSS)){
		pt_port->t_yx1._ccu_di_rmacphaseless = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmacphaseless = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_SHORT_CUT)){
		pt_port->t_yx1._ccu_di_rmdcshort = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmdcshort = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_DC_CUR_OVER)){
		pt_port->t_yx1._ccu_di_rmdccurrover = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmdccurrover = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_DC_VOL_OVER)){
		pt_port->t_yx1._ccu_di_rmdcvoltover = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmdcvoltover = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_DC_VOL_BELOW)){
		pt_port->t_yx1._ccu_di_rmdcvoltless = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmdcvoltless = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_TEMP_OVER)){
		pt_port->t_yx1._ccu_di_rmtemp = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmtemp = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_COMM)){
		pt_port->t_yx1._ccu_di_rmcomm = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmcomm = 0;
	}
	if(0 != fault_check(port,E_FAULT_RECT_FAN)){
		pt_port->t_yx1._ccu_di_rmfan = 1;
	}else{
		pt_port->t_yx1._ccu_di_rmfan = 0;
	}
	//todo 车辆连接状态
//	pt_port->t_yx1._ccu_di_carconnect = 0;

	//todo 枪座状态
//	pt_port->t_yx1._ccu_di_gunseat = 0;
	//todo 电子锁状态
//	pt_port->t_yx1._ccu_di_interfacelock = 0;
	//todo 直流输出接触器状态
//	pt_port->t_yx1._ccu_di_dcswstatus = 0;
	//其它类型故障
//	pt_port->t_yx1._ccu_other_failure[0] = 0;
//	pt_port->t_yx1._ccu_other_failure[1] = 0;
	return SYS_OK;
}
/*
 *发送遥信1数据
 */
static int32_t send_dc_yx1(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_DC_YX1].pri;
	id.bits.PF = (t_cantcu_init[E_DC_YX1].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(pt_port->t_yx1);

    check_yx1_data(port,pt_port);
    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    memcpy(&pt_msg->data[1],&pt_port->t_yx1,sizeof(pt_port->t_yx1));
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 采集遥信2数据
 */
static int32_t check_yx2_data(uint8_t port,T_TCU_PORT* pt_port)
{
	if(0 != fault_check(port,E_FAULT_COLLECT_DOOR)){
		pt_port->t_yx2._ccu_di_door = 1;
	}else {
		pt_port->t_yx2._ccu_di_door = 0;
	}
	if(0 != fault_check(port,E_FAULT_RELAY1_STICKY)){
		pt_port->t_yx2._ccu_di_dcpaste = 1;
	}else {
		pt_port->t_yx2._ccu_di_dcpaste = 0;
	}
	if(0 != fault_check(port,E_ALARM_FLOW_INSLT)){
		pt_port->t_yx2._ccu_di_isoalarm = 1;
	}else{
		pt_port->t_yx2._ccu_di_isoalarm = 0;
	}
	if(0 != fault_check(port,E_FAULT_FLOW_BLEED)){
		pt_port->t_yx2._ccu_di_reserr = 1;
	}else{
		pt_port->t_yx2._ccu_di_reserr = 0;
	}
	if(0 != fault_check(port,E_ALARM_DEV_TEMP)){
		pt_port->t_yx2._ccu_di_piletempalarm = 1;
	}else{
		pt_port->t_yx2._ccu_di_piletempalarm = 0;
	}
	if(0 != fault_check(port,E_ALARM_GUN_TEMP)){
		pt_port->t_yx2._ccu_di_guntempalarm = 1;
	}else{
		pt_port->t_yx2._ccu_di_guntempalarm = 0;
	}
	if(0 != fault_check(port,E_FAULT_COLLECT_AC_RELAY)){
		pt_port->t_yx2._ccu_di_ackmnotdo = 1;
	}else {
		pt_port->t_yx2._ccu_di_ackmnotdo = 0;
	}
	if(0 != fault_check(port,E_FAULT_AC_RELAY_STICKY)){
		pt_port->t_yx2._ccu_di_ackmpaste = 1;
	}else{
		pt_port->t_yx2._ccu_di_ackmpaste = 0;
	}
	if(0 != fault_check(port,E_FAULT_BOARD_POWER)){
		pt_port->t_yx2._ccu_di_bmspower = 1;
	}else {
		pt_port->t_yx2._ccu_di_bmspower = 0;
	}
	if(0 != fault_check(port,E_FAULT_M_RELAY_LOST_CTRL)){
		pt_port->t_yx2._ccu_di_buskmnotdo = 1;
	}else{
		pt_port->t_yx2._ccu_di_buskmnotdo = 0;
	}
	if(0 != fault_check(port,E_FAULT_M_RELAY_STICKY)){
		pt_port->t_yx2._ccu_di_buskmpaste = 1;
	}else{
		pt_port->t_yx2._ccu_di_buskmpaste = 0;
	}
	//todo 并联接触器状态
	pt_port->t_yx2._ccu_other_start1 = 0;
	//todo
	pt_port->t_yx2._ccu_other_fault1 = 0;

	if(0 != fault_check(port,E_FAULT_WATER)){
		pt_port->t_yx2._ccu_other_fault2 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault2 = 0;
	}
	if(0 != fault_check(port,E_FAULT_COLLECT_DOOR_B)){
		pt_port->t_yx2._ccu_other_fault3 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault3 = 0;
	}
	if(0 != fault_check(port,E_FAULT_WATER_BACK)){
		pt_port->t_yx2._ccu_other_fault4 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault4 = 0;
	}
	if(0 != fault_check(port,E_FAULT_SMOG_B)){
		pt_port->t_yx2._ccu_other_fault5 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault5 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault6 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault6 = 0;
	}
	// 功率控制通讯告警---什么鬼
	/*
	if(0 != fault_check(port,E_FAULT_SMOG)){
		pt_port->t_yx2._ccu_other_fault7 = 1;
	}
	*/
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault8 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault8 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault9 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault9 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault10 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault10 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault11 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault11 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault12 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault12 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault13 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault13 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault14 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault14 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault15 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault15 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault16 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault16 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault17 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault17 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault18 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault18 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault19 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault19 = 0;
	}
	if(0 != fault_check(port,E_FAULT_MAS_POWER_CTRL_CUMM)){
		pt_port->t_yx2._ccu_other_fault20 = 1;
	}else{
		pt_port->t_yx2._ccu_other_fault20 = 0;
	}
	return SYS_OK;
}
/*
 * 发送遥信2数据
 */
static int32_t send_dc_yx2(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_DC_YX2].pri;
	id.bits.PF = (t_cantcu_init[E_DC_YX2].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(pt_port->t_yx2);
    check_yx2_data(port,pt_port);
    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    memcpy(&pt_msg->data[1],&pt_port->t_yx2,sizeof(pt_port->t_yx2));
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 发送VIN码信息
 */
static int32_t send_vin_verity(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	uint8_t len = 0;
	uint16_t temp = 0;
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_VIN_VERITY].pri;
	id.bits.PF = (t_cantcu_init[E_VIN_VERITY].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(T_VIN_VERITY) + 1;
    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;
    len += 1;
    memcpy(&pt_msg->data[len],pt_port->t_vin_verity.vin,17);
    len += 17;
    memcpy(&pt_msg->data[len],pt_port->t_vin_verity.charge_cont,3);
    len += 3;
    temp = pt_port->t_vin_verity.soc;
    pt_msg->data[len] = temp & 0xff;
    len += 1;
    pt_msg->data[len] = (temp >> 8) & 0xff;
    len += 1;
    temp = pt_port->t_vin_verity.bat_vol;
    pt_msg->data[len] = temp & 0xff;
    len += 1;
    pt_msg->data[len] = (temp >> 8) & 0xff;
    len += 1;
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 发送心跳
 */
static int32_t send_heart(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_HEART].pri;
	id.bits.PF = (t_cantcu_init[E_HEART].pgn >> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;

    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));
    pt_msg->data[0] = 0;

    pt_msg->data[1] = pt_port->tcu_tick;
    ++pt_port->tcu_tick;
    if (0==pt_port->tcu_tick)
    	pt_port->tcu_tick = 1;

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 发送错误帧
 */
static int32_t send_error(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_ERROR].pri;
	id.bits.PF = (t_cantcu_init[E_ERROR].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 充电桩配置信息应答
 */
static int32_t send_charge_cfg_inquire_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CHARGE_CFG_INQUIRE_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CHARGE_CFG_INQUIRE_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = sizeof(T_PARAM_INFO);

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    memcpy(pt_msg->data,&pt_port->t_param_info,sizeof(T_PARAM_INFO));
    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 升级心跳应答
 */
static int32_t send_upload_heart_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_UPLOAD_HEART_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_UPLOAD_HEART_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 启动下载应答
 */
static int32_t send_start_upload_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_START_UPLOAD_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_START_UPLOAD_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 索要区间应答1
 */
static int32_t send_ask_for_block_ack1(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_ASK_FOR_BLOAK_ACK1].pri;
	id.bits.PF = (t_cantcu_init[E_ASK_FOR_BLOAK_ACK1].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 索要区间应答2
 */
static int32_t send_ask_for_block_ack2(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_ASK_FOR_BLOAK_ACK2].pri;
	id.bits.PF = (t_cantcu_init[E_ASK_FOR_BLOAK_ACK2].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 启动组包应答
 */
static int32_t send_start_group_pack_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_START_GROUP_PACK_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_START_GROUP_PACK_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 完成组包应答
 */
static int32_t send_fullfill_group_pack_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_FULFILL_GROUP_PACK_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_FULFILL_GROUP_PACK_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 程序校验应答
 */
static int32_t send_program_check_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_PROGRAM_CHECK_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_PROGRAM_CHECK_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 立即复位应答
 */
static int32_t send_reset_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_IMMEDIATELY_RESET_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_IMMEDIATELY_RESET_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 定制设置命令应答
 */
static int32_t send_value_set_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CONSTANT_VALUE_SET_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CONSTANT_VALUE_SET_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 定制信息查询应答
 */
static int32_t send_value_inquire_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CONSTANT_VALUE_INQUIRE_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CONSTANT_VALUE_INQUIRE_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 模块数据验证
 */
static int32_t send_module_veriry(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_MODULE_VERITY].pri;
	id.bits.PF = (t_cantcu_init[E_MODULE_VERITY].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 辅助参数应答
 */
static int32_t send_assist_param_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_ASSIST_PARAM_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_ASSIST_PARAM_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 控制指令应答
 */
static int32_t send_cmd_control_ack(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	T_CAN_SG_DATA *pt_msg = &pt_sg_data->t_tx_data;
	T_CANTCU_ID id;
	id.val = 0;
	id.bits.P =  t_cantcu_init[E_CMD_CONTROL_ACK].pri;
	id.bits.PF = (t_cantcu_init[E_CMD_CONTROL_ACK].pgn>> 8) & 0xff;
	id.bits.SA = SRC_ADDR+port;
	id.bits.PS = DES_ADDR;
	pt_msg->id = id.val;
    pt_msg->len = 8;

    memset(pt_msg->data, 0, sizeof(pt_msg->data));

    u32_sg_can_tx(CAN_UP,pt_msg);
    return SYS_OK;
}
/*
 * 数据发送处理
 */
static int32_t tcu_send_handle(uint8_t port,T_TCU_PORT* pt_port,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	if(u32_time_past(pt_port->t_time_cycle.time_1s)>TIME_1S){

		send_heart(port,pt_port,pt_sg_data,pt_config);
		if(0 != pt_port->t_tcu_flag.bits.get_param){
			send_dc_yc(port,pt_port,pt_sg_data,pt_config);
		}
		if(0 < pt_port->t_send_cnt.vin_ctrl){
			send_vin_verity(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.vin_ctrl;
		}

		pt_port->t_time_cycle.time_1s=u32_time_get();
	}
	if(u32_time_past(pt_port->t_time_cycle.time_500ms)>TIME_500MS){

		if(0 < pt_port->t_send_cnt.time_ack){
			send_time_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.time_ack;
		}
		if(0 < pt_port->t_send_cnt.version_ack){
			XJ_LOG("send ver ack\n");
			send_version_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.version_ack;
		}
		if(0 < pt_port->t_send_cnt.charge_param_ack){
			send_charge_param_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.charge_param_ack;
		}
		if(0 != pt_port->t_tcu_flag.bits.get_param){
			send_dc_yx1(port,pt_port,pt_sg_data,pt_config);
			send_dc_yx2(port,pt_port,pt_sg_data,pt_config);
		}

		if(0 < pt_port->t_send_cnt.param_ack){
			send_charge_cfg_inquire_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.param_ack;
		}


		pt_port->t_time_cycle.time_500ms=u32_time_get();
	}
	if(u32_time_past(pt_port->t_time_cycle.time_250ms)>TIME_250MS){
		if(0 < pt_port->t_send_cnt.start_ack){
			send_start_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.start_ack;
		}
		if(0 < pt_port->t_send_cnt.stop_ack){
			send_stop_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.stop_ack;
		}

		if(0 < pt_port->t_send_cnt.service_control){
			send_charge_service_control_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.service_control;
		}
		if(0 < pt_port->t_send_cnt.lock_ctrl){
			send_lock_control_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.lock_ctrl;
		}
		if(0 < pt_port->t_send_cnt.power_ctrl){
			send_power_control_ack(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.power_ctrl;
		}

		if(0 < pt_port->t_send_cnt.start_finish){
			send_start_complete(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.start_finish;
		}
		if(0 < pt_port->t_send_cnt.stop_finish){
			send_stop_complete(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.stop_finish;
		}
		if(0 < pt_port->t_send_cnt.error){
			send_error(port,pt_port,pt_sg_data,pt_config);
			--pt_port->t_send_cnt.error;
		}
		pt_port->t_time_cycle.time_250ms=u32_time_get();
	}
	if(u32_time_past(pt_port->t_time_cycle.time_2s)>TIME_1S*2){

		pt_port->t_time_cycle.time_2s=u32_time_get();
	}
	if(u32_time_past(pt_port->t_time_cycle.time_5s)>TIME_1S*5){

		pt_port->t_time_cycle.time_5s=u32_time_get();
	}
	if(0 != pt_port->t_tcu_flag.bits.get_time){
		XJ_LOG("send time ack %d\n",port);
		send_time_ack(port,pt_port,pt_sg_data,pt_config);
		pt_port->t_tcu_flag.bits.get_time = 0;
	}
	if(0 != pt_port->t_tcu_flag.bits.get_set_val){
		send_value_set_ack(port,pt_port,pt_sg_data,pt_config);
		pt_port->t_tcu_flag.bits.get_set_val = 0;
	}
	if(0 != pt_port->t_tcu_flag.bits.get_set_quire){
		send_value_inquire_ack(port,pt_port,pt_sg_data,pt_config);
		pt_port->t_tcu_flag.bits.get_set_quire = 0;
	}
//	if(0 != pt_port->get_service_control)
	return SYS_OK;
}
/*
 * 数据接收处理
 */
int32_t recv_handle(T_TCU_PORT* pt_ports,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config)
{
	uint8_t recv_len = 0;
	uint8_t port = 0;
	uint32_t pgn = 0;
	T_TCU_PORT      *pt_port;
	T_CAN_SING_DATA *pt_msg         = &pt_sg_data->t_sing_data;
	//T_CAN_SG_DATA   *pt_multi_msg   = &pt_sg_data->t_rx_data;

	pgn = (pt_msg->id >> 8) & 0x00ff00;
	port = (pt_msg->id >> 8) & 0xff - 0xE0;

	pt_port = &pt_ports[port];
	switch(pgn){
	case TCU_PGN_START:
		if(0 >= pt_port->t_send_cnt.start_ack){
			//接收数据保存
			pt_port->t_start_data.port = pt_msg->data[0];
			pt_port->t_start_data.load_switch = pt_msg->data[1];
			pt_port->t_start_data.vin_start = pt_msg->data[2];
			pt_port->t_start_data.apu_type = pt_msg->data[3];
			pt_port->t_start_data.co_charge_flag = pt_msg->data[4];

			//接收到启动指令，发送启动应答，250ms 2s超时 250ms*8 = 2s
			pt_port->t_send_cnt.start_ack = 8;
			//接收到启动指令,置位标志
			pt_port->t_flow_ctrl.bits.start_cmd = 1;
		}
		break;
	case TCU_PGN_STOP:
		if(0 >= pt_port->t_send_cnt.stop_ack){
			pt_port->t_stop_data.port = pt_msg->data[0];
			pt_port->t_stop_data.cause = pt_msg->data[1];
			pt_port->t_stop_data.fault_code = pt_msg->data[2];

			pt_port->t_send_cnt.stop_ack = 8;
			pt_port->t_flow_ctrl.bits.stop_cmd = 1;
		}
		break;
	case TCU_PGN_TIME_SYNC:
		pt_port->t_cp56_time2a.year = pt_msg->data[1];
		pt_port->t_cp56_time2a.month = pt_msg->data[2];
		pt_port->t_cp56_time2a.wday = pt_msg->data[3] & 0x1F;
		pt_port->t_cp56_time2a.mday = (pt_msg->data[3] >>5) & 0xff;
		pt_port->t_cp56_time2a.hour = pt_msg->data[4];
		pt_port->t_cp56_time2a.min = pt_msg->data[5];
		pt_port->t_cp56_time2a.ms = pt_msg->data[6];
		pt_port->t_cp56_time2a.ms = (pt_port->t_cp56_time2a.ms << 8) | pt_msg->data[7];
//todo sync time
		pt_port->t_tcu_flag.bits.get_time = 1;
		break;
	case TCU_PGN_CHECK_VERSION:
		if(0 >= pt_port->t_send_cnt.version_ack){
			pt_port->cumm_ver[0] = pt_msg->data[1];
			pt_port->cumm_ver[1] = pt_msg->data[2];

			pt_port->t_send_cnt.version_ack = 8;
			pt_port->t_tcu_flag.bits.get_version = 1;
		}
		break;
	case TCU_PGN_PARAM:
		if(0 >= pt_port->t_send_cnt.charge_param_ack){
			//data 预留
			pt_port->t_send_cnt.charge_param_ack = 8;
			pt_port->t_send_cnt.version_ack = 0;
			pt_port->t_tcu_flag.bits.get_param = 1;
		}
		break;
	case TCU_PGN_CHARGING_SERVICE:
		if(0 >= pt_port->t_send_cnt.service_control){
			pt_port->start_stop_control = pt_msg->data[1];

			pt_port->t_send_cnt.service_control = 8;
			pt_port->t_tcu_flag.bits.get_service_control = 1;
		}
		break;
	case TCU_PGN_LOCK_CTRL:
		if(0 >= pt_port->t_send_cnt.lock_ctrl){
			pt_port->lock_num = pt_msg->data[1];
			pt_port->lock_operate = pt_msg->data[2];

			pt_port->t_send_cnt.lock_ctrl = 8;
		}
		break;
	case TCU_PGN_ADJUST_POWER:
		if(0 >= pt_port->t_send_cnt.power_ctrl){
			pt_port->power_cmd_type = pt_msg->data[1];
			pt_port->power_param = pt_msg->data[2];
			pt_port->power_param = pt_port->power_param << 8 | pt_msg->data[2];

			pt_port->t_send_cnt.power_ctrl = 8;
		}
		break;
	case TCU_PGN_START_FINISH_ACK:
		if(0 != pt_port->t_send_cnt.start_finish){
			pt_port->t_start_finish_ack.port = pt_msg->data[0];
			pt_port->t_start_finish_ack.load_switch = pt_msg->data[1];
			pt_port->t_start_finish_ack.success_sign = pt_msg->data[2];

			pt_port->t_send_cnt.start_finish = 0;
			pt_port->t_flow_ctrl.bits.start_finish_ack = 1;
		}
		break;
	case TCU_PGN_STOP_FINISH_ACK:
		if(0 != pt_port->t_send_cnt.stop_finish){
			pt_port->t_stop_finish_ack.port = pt_msg->data[0];
			pt_port->t_stop_finish_ack.cause = pt_msg->data[1];
			pt_port->t_stop_finish_ack.sign = pt_msg->data[2];

			pt_port->t_send_cnt.stop_finish = 0;
			pt_port->t_flow_ctrl.bits.stop_finish_ack = 1;
		}
		break;
	case TCU_PGN_YC_ACK:

		pt_port->t_yc_ack.port = pt_msg->data[0];
		pt_port->t_yc_ack.kWh = pt_msg->data[1];
		pt_port->t_yc_ack.kWh = (pt_port->t_yc_ack.kWh >> 8) | pt_msg->data[2];
		pt_port->t_yc_ack.time = pt_msg->data[3];
		pt_port->t_yc_ack.time = pt_port->t_yc_ack.time >> 8 | pt_msg->data[4];


		break;
	case TCU_PGN_VIN_ACK:
		if(0 != pt_port->t_send_cnt.vin_ctrl){
			pt_port->t_vin_verity_ack.port = pt_msg->data[0];
			pt_port->t_vin_verity_ack.success_sign = pt_msg->data[1];
			pt_port->t_vin_verity_ack.cause = pt_msg->data[2];

			pt_port->t_send_cnt.vin_ctrl = 0;
			pt_port->t_tcu_flag.bits.get_verity_vin = 1;
		}
		break;
	case TCU_PGN_TICK:
		pt_port->t_time_check.tcu_cumm_fault_time=u32_time_get();
		break;
	case TCU_PGN_TIMOUT:

		break;
	case TCU_PGN_CONFIG_QUERY:
		if(0 >= pt_port->t_send_cnt.param_ack){
			pt_port->t_send_cnt.param_ack = 8;
		}
		break;
	case TCU_PGN_SETTING:
		pt_port->t_setting_data.port = pt_msg->data[recv_len++];
		pt_port->t_setting_data.total_dev_type = pt_msg->data[recv_len++];
		pt_port->t_setting_data.dev_addr = pt_msg->data[recv_len++];
		pt_port->t_setting_data.set_num = pt_msg->data[recv_len++];
		pt_port->t_setting_data.set_num = pt_port->t_setting_data.set_num << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.pre = pt_msg->data[recv_len++];
		memcpy(pt_port->t_setting_data.dev_name,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_name));
		recv_len += sizeof(pt_port->t_setting_data.dev_name);
		pt_port->t_setting_data.charge_dev_type = pt_msg->data[recv_len++];
		pt_port->t_setting_data.charge_dev_type = pt_port->t_setting_data.charge_dev_type << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.dev_type = pt_msg->data[recv_len++];
		memcpy(pt_port->t_setting_data.dev_manu,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_manu));
		recv_len += sizeof(pt_port->t_setting_data.dev_manu);
		memcpy(pt_port->t_setting_data.dev_hard_ver,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_hard_ver));
		recv_len += sizeof(pt_port->t_setting_data.dev_hard_ver);
		memcpy(pt_port->t_setting_data.dev_soft_ver,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_soft_ver));
		recv_len += sizeof(pt_port->t_setting_data.dev_soft_ver);
		memcpy(pt_port->t_setting_data.dev_soft_date,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_soft_date));
		recv_len += sizeof(pt_port->t_setting_data.dev_soft_date);
		memcpy(pt_port->t_setting_data.dev_soft_verity,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.dev_soft_verity));
		recv_len += sizeof(pt_port->t_setting_data.dev_soft_verity);
		pt_port->t_setting_data.dev_run_model = pt_msg->data[recv_len++];
		pt_port->t_setting_data.ccu_addr = pt_msg->data[recv_len++];
		pt_port->t_setting_data.ccu_tcu_timeout = pt_msg->data[recv_len++];
		memcpy(pt_port->t_setting_data.ccu_tcu_pro_ver,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.ccu_tcu_pro_ver));
		recv_len += sizeof(pt_port->t_setting_data.ccu_tcu_pro_ver);
		pt_port->t_setting_data.model_addr = pt_msg->data[recv_len++];
		memcpy(pt_port->t_setting_data.ccu_model_pro_ver,&pt_msg->data[recv_len],sizeof(pt_port->t_setting_data.ccu_model_pro_ver));
		recv_len += sizeof(pt_port->t_setting_data.ccu_model_pro_ver);
		pt_port->t_setting_data.rated_out_vol = pt_msg->data[recv_len++];
		pt_port->t_setting_data.rated_out_vol = pt_port->t_setting_data.rated_out_vol << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.rated_out_cur = pt_msg->data[recv_len++];
		pt_port->t_setting_data.rated_out_cur = pt_port->t_setting_data.rated_out_cur << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.max_out_vol = pt_msg->data[recv_len++];
		pt_port->t_setting_data.max_out_vol = pt_port->t_setting_data.max_out_vol << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.max_out_cur = pt_msg->data[recv_len++];
		pt_port->t_setting_data.max_out_cur = pt_port->t_setting_data.max_out_cur << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.min_out_cur = pt_msg->data[recv_len++];
		pt_port->t_setting_data.min_out_cur = pt_port->t_setting_data.min_out_cur << 8 | pt_msg->data[recv_len++];
		pt_port->t_setting_data.bms_pro_ver = pt_msg->data[recv_len++];
		pt_port->t_setting_data.yx_cycle = pt_msg->data[recv_len++];
		pt_port->t_setting_data.yc_cycle = pt_msg->data[recv_len++];

		pt_port->t_tcu_flag.bits.get_set_val = 1;
		break;
	case TCU_PGN_SETTING_QUERY:
		pt_port->t_setting_query.port = pt_msg->data[0];
		pt_port->t_setting_query.dev_type = pt_msg->data[1];
		pt_port->t_setting_query.dev_addr = pt_msg->data[2];
		pt_port->t_setting_query.set_num = pt_msg->data[3];
		pt_port->t_setting_query.set_num = pt_port->t_setting_query.set_num << 8 | pt_msg->data[4];

		pt_port->t_tcu_flag.bits.get_set_quire = 1;
		break;
	case TCU_PGN_MODULE_VERIFY_ACK:

		pt_port->t_tcu_flag.bits.get_model_verity_ack = 1;
		break;
	case TCU_PGN_PARAM_SET:
		//todo
		break;
	case TCU_PGN_CONTROL_CMD:
		//todo
		break;
	default:
		break;
	}
	return SYS_OK;
}

//等待TCU下发启动指令
static int32_t check_start(uint8_t port,T_TCU_PORT* pt_port)
{
//	if()
	if(0!=pt_port->t_flow_ctrl.bits.start_cmd && 0 >= pt_port->t_send_cnt.start_ack){
		pt_port->t_flow_ctrl.bits.start_cmd = 0;
		pt_port->flow_state = E_START_CHARGE_ACK;
	}
	return SYS_OK;
}

static int32_t	check_start_ack(uint8_t port,T_TCU_PORT* pt_port)
{
	if(0 != pt_port->t_start_ack_data.success_sign){
		//日志记录	启动应答失败，状态机 清除数据
		pt_port->flow_state = E_TCU_STOP_CLEAN;
		return SYS_FAIL;
	}
	//km start
	tcu_start_charger_interface(port);//
	if(0 != pt_port->t_start_data.vin_start){
		pt_port->flow_state = E_TCU_WAIT_VIN_CODE;
		pt_port->t_time_check.get_vin_timeout=u32_time_get();
	}else{
		pt_port->t_time_check.get_start_finish_timeout =u32_time_get();
		pt_port->flow_state = E_TCU_STARTING;
	}
	return SYS_OK;
}

//等待VIN码数据
static int32_t wait_vin_data(uint8_t port,T_TCU_PORT* pt_port)
{
	if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.vin_state){
		pt_port->flow_state = E_TCU_WAIT_VIN_VERIFY;
		pt_port->t_send_cnt.vin_ctrl = 60; 									//发送VIN数据
		pt_port->t_time_check.verity_vin_timeout = u32_time_get(); 			//获取发送VIN码时间，用以判断30sVIN码鉴权超时
	}else if(u32_time_past(pt_port->t_time_check.get_vin_timeout) > TIME_1S*100){
		//日志记录 VIN码获取超时
		pt_port->flow_state = E_TCU_STOPPING;
		//todo printf
	}
	return SYS_OK;
}

//等待TCU验证结果
static int32_t wait_vin_verity(uint8_t port,T_TCU_PORT* pt_port)
{
	if(1 == pt_port->t_tcu_flag.bits.get_verity_vin){
		pt_port->t_tcu_flag.bits.get_verity_vin = 0;
		if((0 == pt_port->t_vin_verity_ack.success_sign) && (0 == pt_port->t_vin_verity_ack.cause)){
			//todo 通知接触器吸合等
			pt_port->flow_state = E_TCU_STARTING;
		}else{
			//通知模块关机等
		}
	}else if(u32_time_past(pt_port->t_time_check.verity_vin_timeout) > TIME_1S*30){
		//VIN验证超时
		//printf
		//通知模块关机等
	}
	return SYS_OK;
}

//等待开始
static int32_t wait_start(uint8_t port,T_TCU_PORT* pt_port)
{
	if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.startok){
		pt_port->t_mutual_data.t_mutual_data_state.bits.startok = 0;
		pt_port->flow_state = E_TCU_START_FINISH;
		XJ_LOG("============state %d\n",pt_port->flow_state);
		//start success
		pt_port->t_send_cnt.start_finish = 8;

		pt_port->t_time_check.get_start_finish_ack_timeout = u32_time_get();
	}else if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.startfail){
		//start fail
		pt_port->flow_state = E_TCU_START_FINISH;
		XJ_LOG("============state fail%d\n",pt_port->flow_state);
		pt_port->t_send_cnt.start_finish = 8;
	}else if(u32_time_past(pt_port->t_time_check.get_start_finish_timeout) > TIME_1S*120){
		//start timeout
		pt_port->flow_state = E_TCU_START_FINISH;
		set_all_fault(port,E_FAULT_TCU_START_TIMEOUT);
		pt_port->t_send_cnt.start_finish = 8;
	}
	return SYS_OK;
}

//
static int32_t wait_start_finish_ack(uint8_t port,T_TCU_PORT* pt_port)
{
	if(1 == pt_port->t_flow_ctrl.bits.start_finish_ack){
		pt_port->t_flow_ctrl.bits.start_finish_ack = 0;
		pt_port->flow_state = E_TCU_CHARGINE;
		XJ_LOG("============state %d\n",pt_port->flow_state);
	}else if(u32_time_past(pt_port->t_time_check.get_start_finish_ack_timeout) > TIME_1S*5){
		//启动完成应答帧 超时
		//todo
		pt_port->flow_state = E_TCU_CHARGINE;
	}
	return SYS_OK;
}


static int32_t wait_stop_finish_ack(uint8_t port,T_TCU_PORT* pt_port)
{
	if(1 == pt_port->t_flow_ctrl.bits.stop_finish_ack){
		pt_port->t_flow_ctrl.bits.stop_finish_ack = 0;
		pt_port->flow_state = E_TCU_STOP_CLEAN;
	}else if(u32_time_past(pt_port->t_time_check.get_stop_finish_ack_timeout) > TIME_1S*5){
		//完成应答帧 超时
		//todo
		pt_port->flow_state = E_TCU_STOP_CLEAN;
	}
	return SYS_OK;
}

static int32_t wait_stop_ok(uint8_t port,T_TCU_PORT* pt_port)
{
	if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.over){
		pt_port->t_mutual_data.t_mutual_data_state.bits.over = 0;
		pt_port->t_send_cnt.stop_finish = 20;
		pt_port->flow_state = E_TCU_STOP_FINISH;
	}else if(u32_time_past(pt_port->t_time_check.get_stop_finish_timeout) > TIME_1S*50){
		pt_port->t_send_cnt.stop_finish = 20;
		pt_port->flow_state = E_TCU_STOP_FINISH;
		//停止失败
		pt_port->stop_result = 1;
		//停止超时
	}
	return SYS_OK;
}

static int32_t 	check_fault(uint8_t port,T_TCU_PORT* pt_port)
{
	if((u32_time_past(pt_port->t_time_check.tcu_cumm_fault_time)>(TIME_1S*3))){
		pt_port->t_tcu_flag.bits.get_param = 0;
		pt_port->t_tcu_flag.bits.get_version = 0;
		pt_port->t_tcu_flag.bits.cumm_fault = 1;
	}else{
		pt_port->t_tcu_flag.bits.cumm_fault = 0;
	}
	return SYS_OK;
}

static int32_t data_clean(uint8_t port,T_TCU_PORT* pt_port)
{
	pt_port->flow_state = E_TCU_IDLE;

	pt_port->stop_result = 0;				//清空停止结果数据
	//todo
	return SYS_OK;
}

static int32_t stop_check(uint8_t port,T_TCU_PORT  *pt_port)
{
	switch(pt_port->flow_state){
	case E_TCU_START_FINISH:
	case E_START_CHARGE_ACK:
	case E_TCU_WAIT_VIN_CODE:
	case E_TCU_WAIT_VIN_VERIFY:
	case E_TCU_CHARGINE:
		//TCU 下发停机指令 或 TCU-CCU通讯故障
		if((0 != pt_port->t_flow_ctrl.bits.stop_cmd) || (pt_port->t_tcu_flag.bits.cumm_fault == 1)){
			pt_port->t_flow_ctrl.bits.stop_cmd = 0;
			//通知km停止充电 todo cause
			tcu_set_charger_stopcause(1,port);
			pt_port->flow_state = E_TCU_STOPPING;
			pt_port->t_time_check.get_stop_finish_timeout = u32_time_get();
			XJ_LOG("tcu ---stop %d\n",port);
		}else if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.over){   //其他流程停止完成通知
			pt_port->t_mutual_data.t_mutual_data_state.bits.over = 0;
			pt_port->t_send_cnt.stop_finish = 20;
			pt_port->flow_state = E_TCU_STOP_FINISH;
		}
		if(0 != pt_port->t_mutual_data.t_mutual_data_state.bits.startfail){
			//日志记录 km启动失败
			pt_port->t_mutual_data.t_mutual_data_state.bits.startfail = 0;
			pt_port->flow_state = E_TCU_STOP_CLEAN;
		}
		break;
	}
	return SYS_OK;
}

static int32_t flow_control(uint8_t port,T_TCU_PORT* pt_port)
{
	switch(pt_port->flow_state){
	case E_TCU_IDLE:
		check_start(port,pt_port);
		break;
	case E_START_CHARGE_ACK:
		check_start_ack(port,pt_port);
		break;
	case E_TCU_WAIT_VIN_CODE:
		wait_vin_data(port,pt_port);
		break;
	case E_TCU_WAIT_VIN_VERIFY:
		wait_vin_verity(port,pt_port);
		break;
	case E_TCU_STARTING:
		wait_start(port,pt_port);
		break;
	case E_TCU_START_FINISH:
		wait_start_finish_ack(port,pt_port);
		break;
	case E_TCU_CHARGINE:
		break;
	case E_TCU_STOP_ACK:
//		wait_stop_ack_finish_ack(port,pt_port);
		break;
	case E_TCU_STOPPING:
		wait_stop_ok(port,pt_port);
		break;
	case E_TCU_STOP_FINISH:
		wait_stop_finish_ack(port,pt_port);
		break;
	case E_TCU_STOP_CLEAN:
		data_clean(port,pt_port);
		break;
	default:
		break;
	}
	return SYS_OK;
}

/************************************数据交互转换************************************/
/* 原有数据 电压10倍 电流100倍，需转换为真实值
 *
 *
 *
 * */
static int32_t data_sync(uint8_t port,T_TCU_PORT* pt_port)
{
	//YC
	pt_port->out_vol = sMutualData[port].sSysInfo.iSysVolt_10x/10.0;
	pt_port->out_cur = sMutualData[port].sSysInfo.iSysCurr_10x/100.0;
	pt_port->soc = sMutualData[port].sBmsInfo->bcsData.bcsCurSoc;
	pt_port->bat_min_temp = sMutualData[port].sBmsInfo->bsmData.bsmMaxTemperature;
	pt_port->bat_max_temp = sMutualData[port].sBmsInfo->bsmData.bsmMinTemperature;
	pt_port->cell_max_vol = sMutualData[port].sBmsInfo->bcsData.bcsSingleInfo.bit.bMaxSingleVol/100.0;
	//备注 充电中没有单体最低电压
	pt_port->cell_min_vol = sMutualData[port].sBmsInfo->bsdData.bsdMinSingleVol/100.0;
	pt_port->dev_temp = sMutualData[port].sSysInfo.iInletTemp_10x;
	pt_port->guide_vol = sMutualData[port].sSysInfo.iCc1Vol_10x/10.0;
	pt_port->bms_need_vol = sMutualData[port].sBmsInfo->bclData.bclVoltageDemand/10.0;
	pt_port->bms_need_cur = sMutualData[port].sBmsInfo->bclData.bclCurrentDemand/10.0;

	//start_finish
	//todo ccu ver
//	memcpy(pt_port->ccu_bms_ver);
	memcpy(pt_port->bms_ccu_ver,sMutualData[port].sBmsInfo->brmData.brmBmsVersion,sizeof(pt_port->bms_ccu_ver));
	//todo
	pt_port->handshake_result = 0;
	pt_port->battery_type =  sMutualData[port].sBmsInfo->brmData.brmBatteryType;
	pt_port->max_allow_temp = sMutualData[port].sBmsInfo->bcpData.bcpMaxChargerTemperature;
	pt_port->bms_max_allow_voltage = sMutualData[port].sBmsInfo->bcpData.bcpMaxChargerVoltage/100.0;
	pt_port->unit_max_allow_voltage = sMutualData[port].sBmsInfo->bcpData.bcpSingleMaxVol/10.0;
	pt_port->max_allow_electricity = sMutualData[port].sBmsInfo->bcpData.bcpMaxChargerCurrent/100.0;
	pt_port->battery_total_voltage = sMutualData[port].sBmsInfo->brmData.brmBatteryTotalVol/100.0;
	pt_port->battery_current_voltage = sMutualData[port].sBmsInfo->bcpData.bcpCurBatteryVoltage/100.0;
	pt_port->battery_rated_capacity = sMutualData[port].sBmsInfo->brmData.brmBatteryCapacity/100.0;
	pt_port->battery_nominal_energy = sMutualData[port].sBmsInfo->bcpData.bcpBatteryTotalEnergy/100.0;
	//todo
//	pt_port->ccu_max_out_voltage = sMutualData[port].sBmsInfo->bcpData.bcpStartSoc;
//	pt_port->ccu_min_out_voltage = sMutualData[port].sBmsInfo->bcpData.bcpStartSoc;
//	pt_port->ccu_max_out_electricity = sMutualData[port].sBmsInfo->bcpData.bcpStartSoc;
//	pt_port->ccu_min_out_electricity = sMutualData[port].sBmsInfo->bcpData.bcpStartSoc;
	memcpy(pt_port->vin,sMutualData[port].sBmsInfo->brmData.brmVin,sizeof(pt_port->vin));

	//stop finish
	//todo
//	pt_port->t_bst
	pt_port->stop_soc = sMutualData[port].sBmsInfo->bsdData.bsdSoc;
	pt_port->batt_min_vol = sMutualData[port].sBmsInfo->bsdData.bsdMinSingleVol;
	pt_port->batt_max_vol = sMutualData[port].sBmsInfo->bsdData.bsdMaxSingleVol;
	pt_port->batt_min_temp = sMutualData[port].sBmsInfo->bsmData.bsmMinTemperature;
	pt_port->batt_max_temp = sMutualData[port].sBmsInfo->bsmData.bsmMaxTemperature;
//  pt_port->t_bem

	return SYS_OK;
}

/*
 *
 *
 *
 */
static int32_t fault_sync(uint8_t port)
{
	if(0 != sMutualData[port].uAlarmA.bit.bEPO){
		set_all_fault(port,E_FAULT_COLLECT_EMG);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bBREAKER){
		set_all_fault(port,E_FAULT_COLLECT_AC_BREAKER);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bFUSE){
		set_all_fault(port,E_FAULT_DC_FUSE);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bFAN){
		set_all_fault(port,E_FAULT_FAN);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bSPD){
		set_all_fault(port,E_FAULT_COLLECT_SPD);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bOUTLETTEMPUP){
		set_all_fault(port,E_FAULT_TEMP_OVER);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bVGRIDUP){
		set_all_fault(port,E_FAULT_AC_VOL_OVER);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bVGRIDLOW){
		set_all_fault(port,E_FAULT_AC_VOL_BELOW);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bDOOR){
		set_all_fault(port,E_FAULT_COLLECT_DOOR);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bACCONT){
		set_all_fault(port,E_FAULT_COLLECT_AC_RELAY);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bAX){
		set_all_fault(port,E_FAULT_BOARD_POWER);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bMIDCONT){
		set_all_fault(port,E_FAULT_M_RELAY_LOST_CTRL);
	}
	//没作用
	if(0 != sMutualData[port].uAlarmA.bit.bTCUCOM){
		set_all_fault(port,E_FAULT_TCU_COMMUNICATION);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bMODCOMFAULT){
		set_all_fault(port,E_FAULT_RECT_COMM);
	}
	if(0 != sMutualData[port].uAlarmA.bit.bOUTLETTEMPUP_2){
		set_all_fault(port,E_ALARM_DEV_TEMP);
	}

	//模块故障
//	if(0 != sMutualData[port].uAlarmB.bit.bMOD){
//		set_all_fault(port,E_ALARM_DEV_TEMP);
//	}
//	if(0 != sMutualData[port].uAlarmB.bit.bMOD_AC){
//		set_all_fault(port,E_ALARM_DEV_TEMP);
//	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_ACUP){
		set_all_fault(port,E_ALARM_RECT_AC_VOL_OVER);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_ACLOW){
		set_all_fault(port,E_ALARM_RECT_AC_VOL_BELOW);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_ACLOSE){
		set_all_fault(port,E_ALARM_RECT_AC_PHASE_LOSS);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_SHORT){
		set_all_fault(port,E_ALARM_RECT_SHORT_CUT);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_DCOI){
		set_all_fault(port,E_ALARM_RECT_DC_CUR_OVER);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_DCUP){
		set_all_fault(port,E_ALARM_RECT_DC_VOL_OVER);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_DCLOW){
		set_all_fault(port,E_ALARM_RECT_DC_VOL_BELOW);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_OT){
		set_all_fault(port,E_FAULT_RECT_TEMP_OVER);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_COM){
		set_all_fault(port,E_FAULT_RECT_PFC_DC_COMM);
	}
	if(0 != sMutualData[port].uAlarmB.bit.bMOD_FAN){
		set_all_fault(port,E_FAULT_RECT_FAN);
	}

	if(0 != sMutualData[port].uAlarmC.bit.bDCCONT){
		set_all_fault(port,E_FAULT_DC_RELAY_P);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bGunLock){
		set_all_fault(port,E_FAULT_LOCK1_LOST_CTRL);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bINSDETFAIL){
		set_all_fault(port,E_FAULT_FLOW_INSLT);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bVDCINV){
		set_all_fault(port,E_FAULT_BAT_REVERSE);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bCC1){
		set_all_fault(port,E_FAULT_GUN_CC);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bGUNTEMPUP){
		set_all_fault(port,E_FAULT_GUN_TEMP_OVER);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bGUNNOTBACK){
		set_all_fault(port,E_ALARM_GUN_PARKED);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bBMS_COM){
		set_all_fault(port,E_FAULT_BMS_COMMUNICATION);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bVDCUP){
		set_all_fault(port,E_FAULT_DC_VOL_OVER);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bVDCLOW){
		set_all_fault(port,E_FAULT_DC_VOL_BELOW);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bIDCUP){
		set_all_fault(port,E_FAULT_DC_CUR_OVER);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bINSDETABS){
		set_all_fault(port,E_ALARM_FLOW_INSLT);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bGUNTEMPUP_2){
		set_all_fault(port,E_ALARM_GUN_TEMP);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bShort){
		set_all_fault(port,E_FAULT_DC_SHORT_CUT);
	}
	if(0 != sMutualData[port].uAlarmC.bit.bDISCHARGE){
		set_all_fault(port,E_FAULT_FLOW_BLEED);
	}
	return SYS_OK;
}

/*
 * 充电状态信息同步
 *
 */
static int32_t stage_sync(uint8_t port,T_TCU_PORT *pt_port)
{
	pt_port->t_mutual_data.t_mutual_data_state.bits.startok  = sMutualData[port].charge_stage.bit.startok;
	pt_port->t_mutual_data.t_mutual_data_state.bits.startfail  = sMutualData[port].charge_stage.bit.startfail;
	pt_port->t_mutual_data.t_mutual_data_state.bits.over  = sMutualData[port].charge_stage.bit.over;
	return SYS_OK;
}

void tcu_com_handle(void)
{
	t_config.dev_num = 2;

	for(uint8_t i = 0; i < 2; i++){
		tcu_send_handle(i,&t_ports[i],&t_sg_data,&t_config);
		check_fault(i,&t_ports[i]);
		stop_check(i,&t_ports[i]);
		flow_control(i,&t_ports[i]);
		//交互数据转换
	}
}



