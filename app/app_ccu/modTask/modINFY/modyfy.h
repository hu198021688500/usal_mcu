/*
 * yfy.h
 *
 *  Created on: Nov 1, 2021
 *      Author: player
 */

#ifndef _MODYFY_H_
#define _MODYFY_H_
#include "fun.h"

#define 	MONITOR_ADDR 		0xF0
#define		BROADCAST_ADDR  	0x3F

#define		DEV_NUM_A		0x0A
#define		DEV_NUM_B		0x0B

typedef enum{
	E_READ_SYS_DATA = 1,
	E_READ_MOD_SUM,
	E_READ_MOD_N_FDATA,
	E_READ_MOD_N_STATE,
	E_PRE5,
	E_READ_MOD_N_INPUT,
	E_PRE7,
	E_READ_SYS_TOTAL_DATA,
	E_READ_MOD_N_IDATA,
	E_READ_MOD_N_INFO,
	E_READ_MOD_N_MAC,
	E_READ_MOD_N_EXT_DATA,

	E_SET_MOD_N_EN = 0x13,
	E_SET_MOD_N_LED,
	E_SET_MOD_N_GROUP = 0x16,
	E_SET_MOD_SLEEP = 0x19,
	E_SET_MOD_ON_OFF,
	E_SET_MOD_OUTPUT,
	E_SET_MOD_N_OUTPUT,
	E_SET_MOD_ADDR = 0x1F,

}T_YFY_MODULE_CMD;

typedef enum{
	E_BORADCAST,
	E_GROUPS,
	E_MODE,
}T_RUN_TYPE;

typedef enum{
	E_ADDR_AUTO = 0,    //地址自动分配
	E_ADDR_DIAL,        //地址拨码分配
}T_ADDR_TYPE;

typedef enum
{
	T_YFYMOD_CMD_ON = 0,      //开机
	T_YFYMOD_CMD_OFF,         //关机
}T_YFYMOD_CMD_ONOFF;

typedef union{
	struct{
		uint8_t	SA:8;
		uint8_t PS:8;
		uint8_t CMD:6;

		uint8_t DEV:4;
		uint8_t ERROR:3;
		uint8_t R1:3;
	}bits;
	uint32_t val;
}T_YFY_MODULE_ID;

typedef union{
	struct{
		uint8_t low:8;
		uint8_t middle:8;
		uint8_t high:7;
		uint8_t	exponent:8;
		uint8_t mark:1;
	}bits;
	uint32_t val;
}T_YFY_FLOAT;


typedef struct{
	T_YFY_MODULE_ID id;
	uint8_t data[8];
}T_CAN_MSG;

/*******************************************************************/

#define 	MAX_RECT_NUM 		32					//模块最大数量
#define 	MAX_GROUP_RECT_NUM 	8					//组内模块最大数量
#define 	MAX_GROUP_NUM 		8					//最大组数
#define 	MAX_BAR_LEN			18					//条码最大长度

/*typedef struct{

}T_GROUP_RECT;*/

typedef struct{
	uint8_t	mod_sum;
}T_MOD_CONFIG;

typedef union{
	struct{
		uint8_t 	pfc_down:1;						//模块PFC侧处于关机状态
		uint8_t 	input_out_vol_alarm:1;			//输入过压告警
		uint8_t 	input_under_vol_alarm:1;		//输入欠压告警
		uint8_t 	input_three_phase_alarm:1;		//三相输入不平衡告警
		
		uint8_t 	input_gap_alarm:1;				//三相输入缺相告警
		uint8_t		mod_uneven_cur:1;				//模块严重不均流
		uint8_t		mod_addr_repeat:1;				//模块 ID 重复
		uint8_t 	mod_limit_power:1;				//模块处于限功率状态
	}bits;
	uint8_t val;
}T_MOD_STATE2;

typedef union{
	struct{
		uint8_t 	mod_cumm_fault:1;				//模块通信中断告警
		uint8_t 	walk_in_enable:1;				//WALK-IN 使能
		uint8_t 	put_out_vol_alarm:1;			//输出过压告警
		uint8_t 	out_temp_alarm:1;				//过温告警
		
		uint8_t 	fan_alarm:1;					//风扇故障告警
		uint8_t		mod_protect_alarm:1;			//模块保护告警
		uint8_t		mod_fault:1;					//模块故障告警
		uint8_t 	mod_dc_down:1;					//模块 DC 侧处于关机状态
	}bits;
	uint8_t val;
}T_MOD_STATE1;

typedef union{
	struct{
		uint8_t 	pre7:1;							//预留
		uint8_t 	pre6:1;							//预留
		uint8_t 	mod_discharge:1;				//模块放电异常
		uint8_t 	mod_sleep:1;					//模块休眠
		
		uint8_t 	input_or_bus:1;					//输入或母线异常
		uint8_t		mod_in_cumm:1;					//模块内部通信故障
		uint8_t		pre1:1;							//预留
		uint8_t 	put_short_circuit:1;			//输出短路
	}bits;
	uint8_t val;
}T_MOD_STATE0;

typedef struct{
	int8_t	temp;
	uint16_t heartCnt;
	uint8_t onlineFlag;   
	T_MOD_STATE2 	state2;
	T_MOD_STATE1	state1;
	T_MOD_STATE0	state0;
}T_RECT_STATE;

typedef struct{
	float	mod_n_fvol;								//某个模块电压(浮点)
	float 	mod_n_fcur;								//某个模块电流(浮点)
	T_RECT_STATE	t_rect_state;					//某个模块状态
	float 			input_ab;						//某个模块A项输入电压
	float			input_bc;						//某个模块B项输入电压
	float			input_ca;						//某个模块C项输入电压
	uint32_t		mod_n_ivol;						//某个模块电压(整形mv)
	uint32_t 		mod_n_icur;						//某个模块电流(整形mA)
	float 			mod_max_vol;					//某个模块最大电压(浮点)
	float			mod_min_vol;					//某个模块最小电压(浮点)
	float 			mod_max_cur;					//某个模块最大电流(浮点)
	float 			mod_max_power;					//某个模块最大功率(浮点)
	uint8_t			mod_bar_code[MAX_BAR_LEN];		//某个模块条码信息
	float 			mod_ext_vol;					//某个模块外部电压
	float 			mod_max_allow_cur;				//最大允许充电电流
	uint8_t 		mod_walk_enable;				//某个模块walk使能结果
	uint8_t 		mod_green_led;					//某个模块绿灯闪烁结果
	uint8_t			mod_group_num;					//某个模块设置组号结果
	uint8_t 		mod_sleep;						//某个模块休眠结果
	uint8_t			mod_on_off;						//某个模块开关机结果
	uint32_t		mod_n_ivol_ack;					//组内某个模块电压(浮点)
	uint32_t		mod_n_icur_ack;					//组内某个模块电流(浮点)
	uint8_t			mod_n_addr_type;				//某个模块地址设置结果
}T_RECT;

typedef struct{
	float			group_n_fvol;						//组内某个模块电压(浮点)
	float 			group_n_fcur;						//组内某个模块电流(浮点)
	T_RECT_STATE	t_rect_state;						//组内某个模块状态
	float 			input_ab;							//组内某个模块A项输入电压
	float			input_bc;							//组内某个模块B项输入电压
	float			input_ca;							//组内某个模块C项输入电压
	uint32_t 		group_rect_n_ivol;					//组内某个模块电压(整形mv)
	uint32_t 		group_rect_n_icur;					//组内某个模块电流(整形mA)
	float 			group_rect_n_max_vol;				//组内某个模块最大电压(浮点)
	float			group_rect_n_min_vol;				//组内某个模块最小电压(浮点)
	float 			group_rect_n_max_cur;				//组内某个模块最大电流(浮点)
	float 			group_rect_n_max_power;				//组内某个模块最大功率(浮点)
	uint8_t 		group_walk_enable_ack;				//组内某个模块walk使能结果
	uint8_t 		group_green_led_ack;				//组内某个模块绿灯闪烁结果
	uint8_t 		group_num_ack;						//组内某个模块设置组号结果
	uint8_t 		group_rect_n_sleep_ack;				//组内某个模块休眠结果
	uint8_t 		group_rect_n_onoff_ack;				//组内某个模块开关机结果
	float 			group_rect_n_output_volack;			//组内某个模块电压(浮点)
	float 			group_rect_n_output_curack;			//组内某个模块电流(浮点)
	uint8_t			group_rect_addr_type_ack;			//组内某个模块地址设置结果
}T_RECT_GROUP;

typedef struct{
	float			sys_fvol;							//组系统总电压(浮点)
	float 			sys_fcur;							//组系统总电流(浮点)
	uint8_t 		sys_mod_sum;						//组系统模块数量
	T_RECT_GROUP	t_rect_group[MAX_GROUP_RECT_NUM];	//组内每个模块数据
	uint32_t 		group_sys_ivol;						//组系统总电压(整形mv)
	uint32_t 		group_sys_icur;						//组系统总电流(整形mA)
}T_GROUP;

typedef struct{
	uint8_t 	data_type;
	float		sys_fvol;							//系统总电压(浮点)
	float 		sys_fcur;							//系统总电流(浮点)
	uint8_t 	sys_mod_sum;						//系统模块数量
	uint32_t 	sys_ivol;							//系统总电压(整形mv)
	uint32_t 	sys_icur;							//系统总电流(整形mA)
	T_RECT		t_rects[MAX_RECT_NUM];				//每个模块数据
	T_GROUP 	t_group[MAX_GROUP_NUM];				//每组模块数据
}T_MOD_DATA;



/*********************************************************************/
//user data
typedef struct{
	uint16_t 	sys_vol;
	uint16_t	sys_cur;
	uint16_t	sys_group_vol[MAX_GROUP_NUM];
	uint16_t	sys_group_cur[MAX_GROUP_NUM];
	/*-----------------------------------------------*/
	uint8_t		sys_mod_sum;
	uint8_t 	sys_group_mod_sum[MAX_GROUP_NUM];
	/*-----------------------------------------------*/
	uint16_t 	n_mod_vol[MAX_RECT_NUM];
	uint16_t 	n_mod_cur[MAX_RECT_NUM];
	uint16_t 	n_group_mod_vol[MAX_GROUP_NUM];
	uint16_t 	n_group_mod_cur[MAX_GROUP_NUM];
	/*-----------------------------------------------*/
	uint32_t		n_mod_state[MAX_RECT_NUM];
	uint32_t		n_group_mod_state[MAX_GROUP_NUM];
	/*-----------------------------------------------*/
}T_MOD_USER_USER;

typedef struct 
{
	uint8_t uTxTimeCnt;
	uint8_t uTxMaxTime;
	uint8_t uTxSt;
	uint8_t uTxModNum;
    uint8_t uTxGroupNum;

	uint16_t uSetVoltVal_10x;		//50V-1000V，放大10倍，0为无效。
	uint16_t uSetCurrVal_100x;		//放大100倍

	uint16_t uTxMainTimeCnt;

}structModTxMegg;

void yfy_mod_Com_task(void);
uint8_t get_yfymod_online_state(uint8_t modAddr);
uint8_t get_yfymod_fault_state(uint8_t modAddr);
uint8_t get_yfymod_group_num(uint8_t modAddr);  
void get_yfymod_phase_data(uint16_t *phaseA,uint16_t *phaseB,uint16_t *phaseC);



#endif /* YFY_H_ */
