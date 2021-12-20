/*
 * yfy.c
 *
 *  Created on: Nov 1, 2021
 *      Author: player
 */
#include <stdio.h>
#include <string.h>
#include "modyfy.h"
#include "io_megg.h"
#include "kfc_dev.h"
#include "srt_config.h"
#include "can_def.h"
#include "charge_task.h"
#include "modSysManage.h"
#include "sys_type_contactor_group.h"


static T_CAN_MSG can_send_msg,can_recv_msg;

static T_MOD_DATA t_mod_data;

static structModTxMegg sModTxMegg;

static void array_reverse(uint8_t *src_data,uint8_t len)
{
	uint8_t min = 0,max = 0,temp = 0;
	for(min = 0,max = len-1;min < max;min++,max--){
		temp = src_data[min];
		src_data[min] = src_data[max];
		src_data[max] = temp;
	}
}

static int8_t* my_litoa(uint64_t value,int8_t *str,int32_t radix)
{
	int32_t sign = 0;
	int8_t ps[32];
    memset(ps,0,32);
    int32_t i = 0;
    do{
        if(value % radix > 9)
            ps[i] = value % radix +'0'+7;
        else
            ps[i] = value % radix +'0';
        i++;
    }while((value /= radix) > 0);
    if(sign < 0)
        ps[i] = '-';
    else
        i--;
    for(int j = i;j >=0; j--)
    {
        str[i-j] = ps[j];
    }
    return str;
}


/*
 *  func:获取系统电压、电流。组电压、电流
 *  param:
 *	type 0-(获取系统电压、电流) 1-(获取组电压、电流)
 *  serial :组号
 */
static int8_t send_get_sys_data(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_SYS_DATA;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}


#if 0
/*
* 	func:获取系统模块数量。
* 	param:
*	type 0-(获取系统模块数量) 1-(获取组模块数量)
*  	serial :组号
*/
static int8_t send_get_mod_sum(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_SUM;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}
/*
*  func:获取模块电压、电流。组电压、电流。
*  param:
*	type 0-(获取n号模块电压电流) 1-(获取组内模块电压电流)
*  serial :模块号或组号
*/
static int8_t send_get_n_mod_fdata(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_N_FDATA;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));
	
	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}
#endif

/*
*  func:获取模块状态。
*  param:
*	type 0-(获取模块状态) 1-(获取组内模块状态)
*  serial :模块号或组号
*/
static int8_t send_get_n_mod_state(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_N_STATE;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}

/*
*  func:获取模块ABC三项输入电压。
*  param:
*  type 0-(几号模块) 1-(组内模块)
*  serial :模块号或组号
*/
static int8_t send_get_n_mod_input(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_N_INPUT;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);

	return SYS_OK;
}


#if 0
/*
 * func:获取系统电压、电流(mV、mA)。组电压、电流(mV、mA)
 * param:
 *	type 0-(获取系统电压、电流) 1-(获取组电压、电流)
 *  serial :组号
 */
static int8_t send_get_mod_sys_data(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_SYS_TOTAL_DATA;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;

	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);

	return SYS_OK;
}

/*
 *  func:获取n号模块电压、电流。
 *  param:
 *	type 0-(获取n模块电压电流) 1-(获取组内每个模块电压电流)
 *  serial :模块号或组号
 */
static int8_t send_get_n_mod_idata(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_N_IDATA;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}

/*
 *  func:获取n号模块限流、限压、功率值。
 *  param:
 *	type 0-(获取n模块限流、限压、功率值) 1-(获取组内每个模块限流、限压、功率值)
 *  serial :模块号或组号
 */
static int8_t send_get_n_mod_info(uint8_t type,uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(0 == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_READ_MOD_N_INFO;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	
	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}

 /*
 *  func:获取n号模块条码信息。
 *  param:
 *  serial :模块号
 */
static int8_t send_get_n_mod_mac(uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;

	t_msg->id.bits.DEV = DEV_NUM_A;
	t_msg->id.bits.PS = serial;

	t_msg->id.bits.CMD = E_READ_MOD_N_MAC;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}

/*
*  func:获取n号模块外部电压电流。
*  param:
*  serial :模块号
*/
static int8_t send_get_n_mod_extdata(uint8_t serial)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;

	t_msg->id.bits.DEV = DEV_NUM_A;
	t_msg->id.bits.PS = serial;

	t_msg->id.bits.CMD = E_READ_MOD_N_EXT_DATA;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}

/*
*  func:设置n号模块WALK使能or禁止。
*  param:
*  type 0-(设置n号模块walk使能) 1-(设置n组模块walk使能)
*  serial :模块号or组号
*	enbale :0-禁止 1-使能
*/
static int8_t send_set_n_mod_en(uint8_t type,uint8_t serial,uint8_t enable)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_N_EN;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));

	send_mod_can_frame(t_msg);
	
	t_msg->data[0] = enable;

	return SYS_OK;
}

/*
*  func:设置n号模块绿灯闪烁。
*  param:
*  type 0-广播 1-(设置n号模块使能) 2-(设置n组模块使能)
*  serial :模块号or组号
*	enbale :0-正常 1-闪烁
*/
static int8_t send_set_n_mod_led(uint8_t type,uint8_t serial,uint8_t enable)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_N_LED;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	t_msg->data[0] = enable;
	
	send_mod_can_frame(t_msg);

	return SYS_OK;
}
#endif

/*
*  func:设置n号模块组号。
*  param:
*  type 0-广播   1-(设置n组模块组号) else-(设置n号模块组号)
*  serial :模块号
*	group  :组号
*/
static int8_t send_set_n_mod_group(uint8_t type,uint8_t serial,uint8_t group)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_N_GROUP;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	t_msg->data[0] = group;

	send_mod_can_frame(t_msg);

	return SYS_OK;
}


#if 0
/*
 *  func:设置n号模块休眠。
 *  param:
 *  type 0-广播 1-(设置n号模块) 2-(设置n组模块)
 *  serial :模块号
 *	val	   : 0-模块不休眠 1-模块休眠
 */
static int8_t send_set_n_mod_sleep(uint8_t type,uint8_t serial,uint8_t val)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_SLEEP;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	t_msg->data[0] = val;
	
	send_mod_can_frame(t_msg);
	
	return SYS_OK;
}
#endif

/*
 *  func:设置模块开关机。
 *  param:
 *  type 0-广播   1-(设置n组模块) else-(设置n号模块)
 *  serial :模块号or组号
 *	val	   : 0-关机 1-开机
 */
static int8_t send_set_mod_on_off(uint8_t type,uint8_t serial,uint8_t val)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_ON_OFF;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	t_msg->data[0] = val;

	send_mod_can_frame(t_msg);

	return SYS_OK;
}

 /*
 *  func:设置模块输出。
 *  param:
 *  type 0-广播 1-(设置n组模块)
 *  serial :模块号or组号
 *	vol	   : 电压   x10
 *	cur	   : 电流   x100
 */
static int8_t send_setall_mod_output(uint8_t type,uint8_t serial,uint16_t vol,uint16_t cur)
{

	#if 0
	uint8_t len = 0;
	int8_t	farray[4] = {0};
	T_YFY_FLOAT t_yfy_float;
	memset(&t_yfy_float,0x0,sizeof(T_YFY_FLOAT));
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
//		t_msg->id.bits.DEV = DEV_NUM_A;
//		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_OUTPUT;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memcpy(farray,&vol,sizeof(float));
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	memcpy(&t_msg->data[len],farray,sizeof(float));
	len += sizeof(float);
	memcpy(farray,&cur,sizeof(float));
	memcpy(&t_msg->data[len],farray,sizeof(float));

	send_mod_can_frame(t_msg);

	#endif

	return SYS_OK;
}

#if 0
/*
*  func:设置模块输出。
*  param:
*  type 0-广播 1-(设置n号模块) 2-(设置n组模块)
*  serial :模块号or组号
*	vol	   : 电压
*	cur	   : 电流
*/
static int8_t send_set_n_mod_output(uint8_t type,uint8_t serial,float vol,float cur)
{
	uint8_t len = 0;
	int8_t	farray[4] = {0};
	T_YFY_FLOAT t_yfy_float;
	memset(&t_yfy_float,0x0,sizeof(T_YFY_FLOAT));
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_N_OUTPUT;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memcpy(farray,&vol,sizeof(float));
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	memcpy(&t_msg->data[len],farray,sizeof(float));
	len += sizeof(float);
	memcpy(farray,&cur,sizeof(float));
	memcpy(&t_msg->data[len],farray,sizeof(float));

	send_mod_can_frame(t_msg);

	return SYS_OK;
}
#endif

/*
*  func:设置模块地址分配方式。
*  param:
*  type 0-广播 1-(设置n号模块) 2-(设置n组模块)
*  serial :模块号or组号
*	val	   : 0-自动分配 1-拨码方式
*/
static int8_t send_set_mod_addr_pattern(uint8_t type,uint8_t serial,uint8_t val)
{
	T_CAN_MSG *t_msg = &can_send_msg;
	t_msg->id.bits.SA = MONITOR_ADDR;
	if(E_BORADCAST == type){
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = BROADCAST_ADDR;
	}else if(E_GROUPS == type){
		t_msg->id.bits.DEV = 0x0B;
		t_msg->id.bits.PS = serial;
	}else{
		t_msg->id.bits.DEV = DEV_NUM_A;
		t_msg->id.bits.PS = serial;
	}
	t_msg->id.bits.CMD = E_SET_MOD_ADDR;
	t_msg->id.bits.ERROR = 0;
	t_msg->id.bits.R1 = 0;
	memset(t_msg->data,0x0,sizeof(t_msg->data));
	t_msg->data[0] = val;
	
	send_mod_can_frame(t_msg);

	return SYS_OK;
}

/*
 *  func:模块数据接收解析。
 */
static int8_t recv_handle(void)
{
    TCanData mod_can_data = {0};

	uint8_t cmd = 0,group_or_mod = 0,mod_addr = 0,len = 0,group = 0;
	uint8_t	temp_array[4] = {0x0};
	uint8_t temp_bar_array[12] = {'0'};
	uint32_t	temp = 0;
	uint64_t	temp_bar = 0;

	while (KfcDevRead(kSfDevNumCan2, &mod_can_data, sizeof(mod_can_data)) > 0)
	{
        can_recv_msg.id.val = mod_can_data.id;
		for(uint8_t i=0;i<8;i++)
		{
			can_recv_msg.data[i] = mod_can_data.data[i];
		}

		T_CAN_MSG 	*pt_msg=&can_recv_msg;
		if(0x02 != ((pt_msg->id.val >> 24) & 0xff)){
			return SYS_FAIL;
		}
		cmd = pt_msg->id.bits.CMD;
		if(BROADCAST_ADDR == pt_msg->id.bits.SA){
			switch(cmd){
			case E_READ_SYS_DATA:
				memcpy(temp_array,&pt_msg->data,4);
				array_reverse(temp_array,4);
				memcpy(&t_mod_data.sys_fvol,temp_array,4);
				memcpy(temp_array,&pt_msg->data[4],4);
				array_reverse(temp_array,4);
				memcpy(&t_mod_data.sys_fcur,temp_array,4);
				break;
			case E_READ_MOD_SUM:
				t_mod_data.sys_mod_sum = pt_msg->data[2];
				break;
			case E_READ_SYS_TOTAL_DATA:
				t_mod_data.sys_ivol = pt_msg->data[0];
				t_mod_data.sys_ivol = t_mod_data.sys_ivol << 8 | pt_msg->data[1];
				t_mod_data.sys_ivol = t_mod_data.sys_ivol << 8 | pt_msg->data[2];
				t_mod_data.sys_ivol = t_mod_data.sys_ivol << 8 | pt_msg->data[3];
				t_mod_data.sys_icur = pt_msg->data[4];
				t_mod_data.sys_icur = t_mod_data.sys_icur << 8 | pt_msg->data[5];
				t_mod_data.sys_icur = t_mod_data.sys_icur << 8 | pt_msg->data[6];
				t_mod_data.sys_icur = t_mod_data.sys_icur << 8 | pt_msg->data[7];
				break;
			case E_SET_MOD_N_EN:
			case E_SET_MOD_N_LED:
			case E_SET_MOD_N_GROUP:
			case E_SET_MOD_SLEEP:
			case E_SET_MOD_ON_OFF:
			case E_SET_MOD_OUTPUT:
			case E_SET_MOD_N_OUTPUT:
			case E_SET_MOD_ADDR:
				// 广播地址 无回复
				break;
			default:
				// 记录错误日志
				break;
			}
			return SYS_OK;
		}
		if(DEV_NUM_A == pt_msg->id.bits.DEV){
			mod_addr = pt_msg->id.bits.SA;
			if(group >= MAX_RECT_NUM){
				//记录错误日志
				return SYS_FAIL;
			}
			switch(cmd){
				case E_READ_SYS_DATA:
					//no
					break;
				case E_READ_MOD_SUM:
					//no
					break;
				case E_READ_MOD_N_FDATA:
					memcpy(temp_array,&pt_msg->data,4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_rects[mod_addr].mod_n_fvol,temp_array,4);
					memcpy(temp_array,&pt_msg->data[4],4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_rects[mod_addr].mod_n_fcur,temp_array,4);
					break;
				case E_READ_MOD_N_STATE:
					t_mod_data.t_rects[mod_addr].t_rect_state.state2.val = pt_msg->data[5];
					t_mod_data.t_rects[mod_addr].t_rect_state.state1.val = pt_msg->data[6];
					t_mod_data.t_rects[mod_addr].t_rect_state.state0.val = pt_msg->data[7];

					t_mod_data.t_rects[mod_addr].t_rect_state.heartCnt = 0;
					t_mod_data.t_rects[mod_addr].t_rect_state.onlineFlag = 1;
					break;
				case E_READ_MOD_N_INPUT:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					t_mod_data.t_rects[mod_addr].input_ab = temp/10.0;
					temp = pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_rects[mod_addr].input_bc = temp/10.0;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					t_mod_data.t_rects[mod_addr].input_ca = temp/10.0;
					break;
				case E_READ_SYS_TOTAL_DATA:
					//no
					break;
				case E_READ_MOD_N_IDATA:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					temp = temp << 8 | pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_rects[mod_addr].mod_n_ivol = temp;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					temp = temp << 8 | pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_rects[mod_addr].mod_n_icur = temp;
					break;
				case E_READ_MOD_N_INFO:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					t_mod_data.t_rects[mod_addr].mod_max_vol = temp/1.0;
					temp = pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_rects[mod_addr].mod_min_vol = temp/1.0;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					t_mod_data.t_rects[mod_addr].mod_max_cur = temp/10.0;
					temp = pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_rects[mod_addr].mod_max_power = temp/100.0;
					break;
				case E_READ_MOD_N_MAC:
					memset(t_mod_data.t_rects[mod_addr].mod_bar_code,0x30,sizeof(t_mod_data.t_rects[mod_addr].mod_bar_code));
					temp_bar = pt_msg->data[1];
					temp_bar = temp_bar << 8 | pt_msg->data[2];
					temp_bar = temp_bar << 8 | pt_msg->data[3];
					temp_bar = temp_bar << 8 | pt_msg->data[4];
					temp_bar = temp_bar << 8 | pt_msg->data[5];
					my_litoa(temp_bar,(int8_t*)temp_bar_array,10);
					len = strlen((char*)temp_bar_array);
					if(len > 12){
						// 记录错误日志
						return SYS_FAIL;
					}
					memcpy(&t_mod_data.t_rects[mod_addr].mod_bar_code[12-len],temp_bar_array,len);
					memcpy(&t_mod_data.t_rects[mod_addr].mod_bar_code[13],&pt_msg->data[0],1);
					temp = pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					my_litoa(temp_bar,(int8_t*)temp_bar_array,10);
					memcpy(&t_mod_data.t_rects[mod_addr].mod_bar_code[16-len],temp_bar_array,len);
					break;
				case E_READ_MOD_N_EXT_DATA:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					t_mod_data.t_rects[mod_addr].mod_ext_vol = temp/10.0;
					temp = pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_rects[mod_addr].mod_max_allow_cur = temp/10.0;
					break;
				case E_SET_MOD_N_EN:
					t_mod_data.t_rects[mod_addr].mod_walk_enable = pt_msg->data[0];
					break;
				case E_SET_MOD_N_LED:
					t_mod_data.t_rects[mod_addr].mod_green_led = pt_msg->data[0];
					break;
				case E_SET_MOD_N_GROUP:
					t_mod_data.t_rects[mod_addr].mod_group_num = pt_msg->data[0];
					break;
				case E_SET_MOD_SLEEP:
					t_mod_data.t_rects[mod_addr].mod_sleep = pt_msg->data[0];
					break;
				case E_SET_MOD_ON_OFF:
					t_mod_data.t_rects[mod_addr].mod_on_off = pt_msg->data[0];
					break;
				case E_SET_MOD_OUTPUT:
					//no
					break;
				case E_SET_MOD_N_OUTPUT:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					temp = temp << 8 | pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_rects[mod_addr].mod_n_ivol_ack = temp;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					temp = temp << 8 | pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_rects[mod_addr].mod_n_icur_ack = temp;
					break;
				case E_SET_MOD_ADDR:
					t_mod_data.t_rects[mod_addr].mod_n_addr_type = pt_msg->data[0];
					break;
				default:
					break;
				}
		}else if(DEV_NUM_B == pt_msg->id.bits.DEV){
			group_or_mod = pt_msg->id.bits.SA;
			if(group_or_mod >= MAX_GROUP_NUM){
				//记录错误日志
				return SYS_FAIL;
			}
			switch(cmd){
				case E_READ_SYS_DATA:
					memcpy(temp_array,&pt_msg->data,4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].sys_fvol,temp_array,4);
					memcpy(temp_array,&pt_msg->data[4],4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].sys_fcur,temp_array,4);
					break;
				case E_READ_MOD_SUM:
					t_mod_data.t_group[group_or_mod].sys_mod_sum = pt_msg->data[2];
					break;
				case E_READ_MOD_N_FDATA:
					memcpy(temp_array,&pt_msg->data,4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group].t_rect_group[group_or_mod].group_n_fvol,temp_array,4);
					memcpy(temp_array,&pt_msg->data[4],4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group].t_rect_group[group_or_mod].group_n_fcur,temp_array,4);
					break;
				case E_READ_MOD_N_STATE:
					group = pt_msg->data[2];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].t_rect_state.temp = pt_msg->data[4];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].t_rect_state.state2.val = pt_msg->data[5];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].t_rect_state.state1.val = pt_msg->data[6];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].t_rect_state.state0.val = pt_msg->data[7];
					break;
				case E_READ_MOD_N_INPUT:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].input_ab =  temp/10.0;
					temp = pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].input_bc = temp/10.0;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					t_mod_data.t_group[group].t_rect_group[group_or_mod].input_ca = temp/10.0;
					break;
				case E_READ_SYS_TOTAL_DATA:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					temp = temp << 8 | pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_group[group_or_mod].group_sys_ivol = temp;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					temp = temp << 8 | pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_group[group_or_mod].group_sys_icur = temp;
					break;
				case E_READ_MOD_N_IDATA:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					temp = temp << 8 | pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_ivol = temp;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					temp = temp << 8 | pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_icur = temp;
					break;
				case E_READ_MOD_N_INFO:
					temp = pt_msg->data[0];
					temp = temp << 8 | pt_msg->data[1];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_max_vol = temp/1.0;
					temp = pt_msg->data[2];
					temp = temp << 8 | pt_msg->data[3];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_min_vol = temp/1.0;
					temp = pt_msg->data[4];
					temp = temp << 8 | pt_msg->data[5];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_max_cur = temp/10.0;
					temp = pt_msg->data[6];
					temp = temp << 8 | pt_msg->data[7];
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_max_power = temp/100.0;
					break;
				case E_READ_MOD_N_MAC:
					//no
					break;
				case E_READ_MOD_N_EXT_DATA:
					//no
					break;
				case E_SET_MOD_N_EN:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_walk_enable_ack = pt_msg->data[0];
					break;
				case E_SET_MOD_N_LED:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_green_led_ack = pt_msg->data[0];
					break;
				case E_SET_MOD_N_GROUP:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_num_ack = pt_msg->data[0];
					break;
				case E_SET_MOD_SLEEP:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_sleep_ack = pt_msg->data[0];
					break;
				case E_SET_MOD_ON_OFF:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_onoff_ack = pt_msg->data[0];
					break;
				case E_SET_MOD_OUTPUT:
					memcpy(temp_array,&pt_msg->data,4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_output_volack,temp_array,4);
					memcpy(temp_array,&pt_msg->data[4],4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_output_curack,temp_array,4);
					break;
				case E_SET_MOD_N_OUTPUT:
					memcpy(temp_array,&pt_msg->data,4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_output_volack,temp_array,4);
					memcpy(temp_array,&pt_msg->data[4],4);
					array_reverse(temp_array,4);
					memcpy(&t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_n_output_curack,temp_array,4);
					break;
				case E_SET_MOD_ADDR:
					t_mod_data.t_group[group_or_mod].t_rect_group[group_or_mod].group_rect_addr_type_ack = pt_msg->data[0];
					break;
				default:
					break;
				}
		}else{
			//记录错误日志
			return SYS_FAIL;
		}

	}

	return SYS_OK;

}

static void yfy_mod_TxSingleModMegg(void)
{	
	uint8_t group = 0;
	sModTxMegg.uTxTimeCnt++;
	if(sModTxMegg.uTxTimeCnt>=sModTxMegg.uTxMaxTime)
	{
		sModTxMegg.uTxTimeCnt=0;

		switch(sModTxMegg.uTxSt)
		{
			case 0:                              //设置地址分配方式,获取#1号模块三相电流
			{
				sModTxMegg.uTxMaxTime=5;         //txmaxtime加起来不能超过25，不然被100ms一次返回了

				send_set_mod_addr_pattern(E_BORADCAST,BROADCAST_ADDR,E_ADDR_DIAL);

				send_get_n_mod_input(0,1);     

				//send_set_mod_on_off(E_GROUPS,0,T_YFYMOD_CMD_OFF);		// 0组模块关
				
				sModTxMegg.uTxSt++;
				break;
			}
			case 1:                                     //设置电压、电流
			{
			    sModTxMegg.uTxGroupNum = (sModTxMegg.uTxGroupNum == 0)?1:sModTxMegg.uTxGroupNum;
				send_setall_mod_output(E_GROUPS,sModTxMegg.uTxGroupNum,sModTxMegg.uSetVoltVal_10x,sModTxMegg.uSetCurrVal_100x);
				sModTxMegg.uTxMaxTime = 1;
				
        		if(++sModTxMegg.uTxGroupNum > sSysConfig.sInt.uGunNum)
        		{
				    sModTxMegg.uTxSt++;
					sModTxMegg.uTxGroupNum = 1;
					sModTxMegg.uTxMaxTime = 2;
        		}
				break;
			}
			case 2:                                     //模块开关机
			{
			    sModTxMegg.uTxGroupNum = (sModTxMegg.uTxGroupNum == 0)?1:sModTxMegg.uTxGroupNum;
				if(chargerMegg[sModTxMegg.uTxGroupNum-1].flag.bit.bStartMod)
				{
			        send_set_mod_on_off(E_GROUPS,sModTxMegg.uTxGroupNum,T_YFYMOD_CMD_ON);
				}
				else
				{
			        send_set_mod_on_off(E_GROUPS,sModTxMegg.uTxGroupNum,T_YFYMOD_CMD_OFF);
				}
				
				sModTxMegg.uTxMaxTime=2;

				if(++sModTxMegg.uTxGroupNum > sSysConfig.sInt.uGunNum)
        		{
				    sModTxMegg.uTxSt++;
					sModTxMegg.uTxGroupNum = 1;
					sModTxMegg.uTxMaxTime = 2;
        		}
				break;
			}

			case 3:                                     //获取组内模块电压总电流
			{
				send_get_sys_data(1,sModTxMegg.uTxGroupNum);
				sModTxMegg.uTxMaxTime=1;
			    if(++sModTxMegg.uTxGroupNum > sSysConfig.sInt.uGunNum)
        		{
				    sModTxMegg.uTxSt++;
					sModTxMegg.uTxGroupNum = 1;
					sModTxMegg.uTxMaxTime = 2;
        		}
				break;
			}

			case 4:                                    //获取模块温度等信息
			{
			    sModTxMegg.uTxModNum = (sModTxMegg.uTxModNum == 0)?1:sModTxMegg.uTxModNum;
				send_get_n_mod_state(0,sModTxMegg.uTxModNum);
				sModTxMegg.uTxMaxTime=1;

			    if(++sModTxMegg.uTxModNum > sSysConfig.sInt.uModNum)
        		{
				    sModTxMegg.uTxSt++;
					sModTxMegg.uTxModNum = 1;
					sModTxMegg.uTxMaxTime = 2;
        		}
				break;
			}
			case 5:                                     //设置模块组号			
			{

			    //group = vGetModGroup_Gh2(sModTxMegg.uTxModNum);     //待打开
				send_set_n_mod_group(2,sModTxMegg.uTxModNum,group);        //模块地址为1~n(不含0)
				sModTxMegg.uTxMaxTime=5;

				
			    if(++sModTxMegg.uTxModNum > sSysConfig.sInt.uModNum)
        		{
				    sModTxMegg.uTxSt++;
					sModTxMegg.uTxModNum = 1;
					sModTxMegg.uTxMaxTime = 2;
        		}
				break;
			}
				
			default:
			{
			    sModTxMegg.uTxGroupNum = 1;
				sModTxMegg.uTxModNum = 1;
				sModTxMegg.uTxSt=0;
				break;
			}
			
		}
	}


}

static void mod_heart_cnt_timeout(void)
{
	uint8_t AC_contactorSt = 0;
	
	AC_contactorSt = get_In_IO(ENUM_IN_ACCONT,0);
	if(0 == AC_contactorSt)
    {
    
	    for(uint8_t j=1;j<=sSysConfig.sInt.uModNum;j++)
        {
            t_mod_data.t_rects[j].t_rect_state.heartCnt = 0;
			t_mod_data.t_rects[j].t_rect_state.onlineFlag = 0;
        }
        return;
    }

	for(uint8_t i=1;i<=sSysConfig.sInt.uModNum;i++)
    {
        t_mod_data.t_rects[i].t_rect_state.heartCnt++;
		if(t_mod_data.t_rects[i].t_rect_state.heartCnt > 6000)
		{
			t_mod_data.t_rects[i].t_rect_state.heartCnt = 6000;
			t_mod_data.t_rects[i].t_rect_state.onlineFlag = 0;
		}
    }
}

/*
*  func:获取n模块组号。
*  param:
*  modAddr-（1-n）
*/
uint8_t get_yfymod_online_state(uint8_t modAddr)
{
	return(t_mod_data.t_rects[modAddr].t_rect_state.onlineFlag);
}

/*
*  func:获取n模块组号。
*  param:
*  modAddr-（1-n）
*/
uint8_t get_yfymod_fault_state(uint8_t modAddr)
{
    if(((t_mod_data.t_rects[modAddr].t_rect_state.state2.val&0x7e)!=0)
		||((t_mod_data.t_rects[modAddr].t_rect_state.state1.val&0xbe)!=0)
		||((t_mod_data.t_rects[modAddr].t_rect_state.state0.val&0x3d!=0)))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/*
*  func:获取n模块组号。
*  param:
*  modAddr-（1-n）
*/
uint8_t get_yfymod_group_num(uint8_t modAddr)  
{
	return(t_mod_data.t_rects[modAddr].mod_group_num);
}

void get_yfymod_phase_data(uint16_t *phaseA,uint16_t *phaseB,uint16_t *phaseC)
{
    *phaseA = (uint16_t)(t_mod_data.t_rects[1].input_ab*10);
    *phaseB = (uint16_t)(t_mod_data.t_rects[1].input_bc*10);
    *phaseC = (uint16_t)(t_mod_data.t_rects[1].input_ca*10);
}

void yfy_mod_Com_task(void)
{
    mod_manage_main();
	yfy_mod_TxSingleModMegg();
    recv_handle();
	mod_heart_cnt_timeout();
}


