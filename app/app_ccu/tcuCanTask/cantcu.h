/*
 * cantcu.h
 *
 *  Created on: Sep 18, 2021
 *      Author: Zhx
 */

#ifndef CANTCU_H_
#define CANTCU_H_

#include "fun.h"
#include "sgcan.h"


typedef struct{
	uint8_t	dev_num;
//	uint8_t

}T_SYS_CONFIG;


/******************************PGN****************************************/
#define	TCU_PGN_START                	0x000100	  	//充电启动帧
#define TCU_PGN_START_ACK            	0x000200		//充电启动应答帧
#define TCU_PGN_STOP                 	0x000300		//充电停止帧
#define TCU_PGN_STOP_ACK             	0x000400		//充电停止应答帧
#define TCU_PGN_TIME_SYNC            	0x000500		//下发对时帧
#define TCU_PGN_TIME_SYNC_ACK        	0x000600		//下发对时应答帧
#define TCU_PGN_CHECK_VERSION        	0x000700		//版本校验帧
#define TCU_PGN_VERSION_ACK          	0x000800		//版本校验应答帧
#define TCU_PGN_PARAM		           	0x000900		//下发充电参数信息帧

#define TCU_PGN_PARAM_ACK        		0x000A00		//下发充电参数信息应答帧
#define TCU_PGN_CHARGING_SERVICE     	0x000B00		//充电服务启停控制帧

#define TCU_PGN_CHARGING_SERVICE_ACK 	0x000C00		//充电服务启停控制应答帧

#define TCU_PGN_LOCK_CTRL            	0x000D00		//电子锁控制帧
#define TCU_PGN_LOCK_CTRL_ACK        	0x000E00		//电子锁控制应答帧
#define TCU_PGN_ADJUST_POWER         	0x000F00		//功率调节控制帧
#define TCU_PGN_ADJUST_POWER_ACK     	0x001000		//功率调节控制应答帧


#define TCU_PGN_START_FINISH        	0x001100		//充电启动完成帧
#define TCU_PGN_START_FINISH_ACK     	0x001200		//充电启动完成应答帧
#define TCU_PGN_STOP_FINISH          	0x001300		//充电停止完成帧
#define TCU_PGN_STOP_FINISH_ACK      	0x001400		//充电停止完成应答帧

#define TCU_PGN_DC_STATE             	0x001500		//直流充电桩状态帧
#define TCU_PGN_AC_STATE             	0x001600		//交流充电桩状态帧

#define TCU_PGN_YC                   	0x002000		//DC遥测帧
#define TCU_PGN_YC_ACK               	0x002100		//DC遥测帧
#define TCU_PGN_YX                   	0x002200		//DC遥信帧
#define TCU_PGN_YX1                  	0x002300		//DC遥信帧1

#define TCU_PGN_VIN                  	0x002D00		//车辆验证数据帧
#define TCU_PGN_VIN_ACK              	0x002E00		//车辆验证确认帧

#define TCU_PGN_AC_YC					0x003000		//AC遥测帧
#define TCU_PGN_AC_YC1					0x003100		//AC遥测帧
#define TCU_PGN_AC_YX					0x003200		//AC遥信帧
#define TCU_PGN_AC_YX1					0x003300		//AC遥信帧

#define TCU_PGN_TICK                 	0x004000		//计费控制单元心跳帧
#define TCU_PGN_TICK_ACK             	0x004100		//充电控制器心跳帧

#define TCU_PGN_TIMOUT              	0x005100		//计费控制单元错误状态帧
#define TCU_PGN_TIMEOUT              	0x005200		//充电控制器错误状态帧

#define TCU_PGN_CONFIG_QUERY         	0x006000		//充电桩配置信息查询帧
#define TCU_PGN_CONFIG_ACK           	0x006100		//充电桩配置信息查询应答帧

#define TCU_PNG_UPGRADE_TICK		 	0x007000		//升级心跳帧
#define TCU_PGN_UPGRADE_TICK_ACK	 	0x007100		//升级心跳应答帧

#define TCU_PGN_START_UPLOAD		 	0x007200		//启动下载命令帧
#define TCU_PGN_START_UPLOAD_ACK	 	0x007300		//启动下载应答帧
#define TCU_PGN_REQUEST_INTERVAL	 	0x007400		//索要区间命令帧
#define TCU_PGN_REQUEST_INTERVAL_ACK1 	0x007500		//索要区间应答帧1
#define TCU_PGN_REQUEST_INTERVAL_ACK2 	0x007600		//索要区间应答帧2
#define TCU_PGN_START_GROUP_PACK 	 	0x007700		//启动组包命令帧
#define TCU_PGN_START_GROUP_PACK_ACK 	0x007800		//启动组包应答帧
#define TCU_PGN_SEND_DATA				0x007900		//数据发送帧
#define TCU_PGN_FULFILL_GROUP_PACK		0x007A00		//完成组包命令帧
#define TCU_PGN_FULFILL_GROUP_PACK_ACK	0x007B00		//完成组包应答帧
#define TCU_PGN_PROGRAM_CHECK			0x007C00		//程序校验数据帧
#define TCU_PGN_PROGRAM_CHECK_ACK		0x007D00		//程序校验应答帧
#define TCU_PGN_RESET_IMMEDIATELY		0x007E00		//立即复位命令帧
#define TCU_PGN_RESET_INMEDIATELY_ACK	0x007F00		//立即复位应答


#define TCU_PGN_SETTING              	0x008000		//定值设置命令帧
#define TCU_PGN_SETTING_ACK          	0x008100		//定值设置命令应答帧

#define TCU_PGN_SETTING_QUERY           0x008200		//定值查询命令帧
#define TCU_PGN_SETTING_QUERY_ACK       0x008300		//定值查询命令帧应答帧

#define TCU_PGN_MODULE_VERIFY			0x008400		//模块验证数据帧
#define TCU_PGN_MODULE_VERIFY_ACK		0x008500		//模块验证数据确认帧

#define TCU_PGN_PARAM_SET				0x008600		//辅助参数设置帧
#define TCU_PGN_PARAM_SET_ACK			0x008700		//辅助参数确认帧
#define TCU_PGN_CONTROL_CMD				0x008800		//终端控制帧
#define TCU_PGN_CONTROL_CMD_ACK			0x008900		//终端控制应答帧

#define CAN_CCU_VERSION		0x0230		//充电控制器通信协议版本号

#define SRC_ADDR	0xE0				//源地址
#define DES_ADDR	0x8A				//目的地址


typedef union{
	struct{
		uint8_t	SA:8;
		uint8_t PS:8;
		uint8_t PF:8;

		uint8_t R3:1;
		uint8_t R2:1;
		uint8_t P:3;
		uint8_t R1:3;
	}bits;
	uint32_t val;
}T_CANTCU_ID;

typedef enum{
	 E_DILE = 0,
	 E_START_CHARGE_ACK,			 //启动充电应答
	 E_STOP_CHARGE_ACK,				 //充电停止应答
	 E_TIME_CHECK_ACK,				 //下发对时应答
	 E_VERSION_CHECK_ACK,			 //版本校验应答
	 E_CHARGE_PARAM_ACK,			 //充电参数应答
	 E_CHARGE_SERVICE_CONTROL_ACK,	 //充电服务启停控制应答
	 E_LOCAK_CONTROL_ACK,			 //电子锁控制应答
	 E_POWER_CONTROL_ACK,			 //功率调节控制应答
	 E_START_CHARGE_COMPLETE,		 //充电启动完成帧
	 E_STOP_CHARGE_COMPLETE,		 //充电停止完成帧
	 E_CHARGE_STATE1,				 //充电桩状态帧 --预留
	 E_CHARGE_STATE2,				 //充电桩状态帧 --预留
	 E_DC_YC,						 //遥测帧
	 E_DC_YX1,						 //遥信帧1
	 E_DC_YX2,						 //遥信帧2
	 E_VIN_VERITY,					 //车辆验证数据帧
	 E_AC_YC,						 //
	 E_AC_YX1,						 //交流遥信帧
	 E_AC_YX2,						 //交流遥信帧
	 E_HEART,						 //充电控制器心跳帧
	 E_ERROR,						 //充电控制器错误状态帧
	 E_CHARGE_CFG_INQUIRE_ACK,		 //充电桩配置信息查询应答帧
	 E_UPLOAD_HEART_ACK,			 //升级心跳应答帧
	 E_START_UPLOAD_ACK,			 //启动下载应答帧
	 E_ASK_FOR_BLOAK_ACK1,			 //索要区间应答帧1
	 E_ASK_FOR_BLOAK_ACK2,			 //索要区间应答帧2
	 E_START_GROUP_PACK_ACK,		 //启动组包应答帧
	 E_FULFILL_GROUP_PACK_ACK,	     //完成组包应答帧
	 E_PROGRAM_CHECK_ACK,			 //程序校验应答帧
	 E_IMMEDIATELY_RESET_ACK,		 //立即复位应答
	 E_CONSTANT_VALUE_SET_ACK,		 //定值设置命令应答帧
	 E_CONSTANT_VALUE_INQUIRE_ACK,	 //定值查询命令应答帧
	 E_MODULE_VERITY,				 //模块验证数据帧
	 E_ASSIST_PARAM_ACK,			 //辅助参数确认帧
	 E_CMD_CONTROL_ACK,				 //终端控制应答帧
	 E_TOTAL,
}T_E_STATE;

typedef enum{
	E_TCU_IDLE = 0,
	E_TCU_START_ACK,
	E_TCU_WAIT_VIN_CODE,
	E_TCU_WAIT_VIN_VERIFY,
	E_TCU_STARTING,
	E_TCU_START_FINISH,
	E_TCU_CHARGINE,
	E_TCU_STOP_ACK,
	E_TCU_STOPPING,
	E_TCU_STOP_FINISH,
	E_TCU_SUSPEND,
	E_TCU_STOP_CLEAN,
}T_CHARGE_STATE;

typedef struct{
	uint8_t		state;
	uint32_t	pgn;
	uint8_t	 	pri;
	uint16_t	time;
	uint16_t	out_time;
}T_CANTCU_TX_TABLE;



/***********************************数据结构**************************************/
typedef struct{
	uint8_t	port;
	uint8_t	load_switch;
	uint8_t	vin_start;
	uint8_t	apu_type;
	uint8_t	co_charge_flag;
}T_START_DATA;

//充电启动应答帧
typedef struct{
	uint8_t	port;
	uint8_t	load_switch;
	uint8_t	success_sign;
	uint8_t	fail_cause;
}T_START_ACK_DATA;

typedef struct{
	uint8_t	port;
	uint8_t	cause;
	uint8_t	fault_code;
}T_STOP_DATA;
//充电停止应答帧
typedef struct{
	uint8_t	port;
	uint8_t	success_sign;
}T_STOP_ACK_DATA;

typedef struct{
	uint8_t	port;
	uint8_t	load_switch;
	uint8_t	success_sign;
}T_START_FINISH_ACK;

typedef struct{
	uint8_t	port;
	uint8_t	cause;
	uint8_t	sign;
}T_STOP_FINISH_ACK;

typedef struct{
	uint8_t	port;
	uint16_t	kWh;
	uint16_t	time;
}T_YC_ACK;

typedef struct{
	uint8_t 	vin[17];
	uint8_t		charge_cont[3];
	uint16_t 	soc;
	uint16_t 	bat_vol;
}T_VIN_VERITY;

typedef struct{
	uint8_t	port;
	uint8_t	success_sign;
	uint8_t cause;
}T_VIN_VERITY_ACK;


typedef struct{
	uint8_t	port;
	uint8_t	vendor_code[4];
	uint16_t dev_type;
	uint8_t	bms_ver[4];
	uint8_t	ccu_serial[12];
	uint8_t hard_ver[2];
	uint8_t soft_ver[3];
	uint8_t ccu_soft_date[4];
	uint16_t	max_output_vol;
	uint16_t	min_output_vol;
	uint16_t	max_output_cur;
	uint16_t	min_output_cur;
}T_PARAM_INFO;
#pragma pack(1)	//按char对齐
typedef struct
{
    uint8_t working_status:2;//工作状态
    uint8_t total_failure:1;//总故障
    uint8_t total_waring:1;//总警告
    uint8_t exigency_failure:1;//急停按钮动作故障
    uint8_t smoke_failure:1;//烟感故障
    uint8_t ac_input_breaker_failure:1;//交流输入断路器故障
    uint8_t failure_28:1;//直流母线输出接触器拒动/误动故障

    uint8_t _ccu_di_fuse:1;//直流母线输出熔断器故障
    uint8_t _ccu_di_gunlock:1;//充电接口电子锁故障
    uint8_t _ccu_di_fan:1;//充电桩风扇故障
    uint8_t _ccu_di_spd:1;//避雷器故障
    uint8_t _ccu_di_iso:1;//绝缘监测故障
    uint8_t _ccu_di_battery:1;//电池极性反接故障
    uint8_t _ccu_di_daoyin:1;//充电中车辆控制导引故障
    uint8_t _ccu_di_piletemperr:1;//充电桩过温故障

	uint8_t _ccu_di_guntemperr:1;//充电接口过温故障
	uint8_t _ccu_di_gunposition:1;//充电枪未归位告警
	uint8_t _ccu_di_bmscom:1;//bms 通信故障
	uint8_t _ccu_di_voltoverin:1;//输入电压过压故障
	uint8_t _ccu_di_voltlessin:1;//输入电压欠压故障
	uint8_t _ccu_di_busvoltover:1;//直流母线输出过压故障
	uint8_t _ccu_di_busvoltless:1;//直流母线输出欠压故障
	uint8_t _ccu_di_buscurrover:1;//直流母线输出过流故障

	uint8_t _ccu_di_rmerr:1;//充电模块故障
	uint8_t _ccu_di_rmacerr:1;//充电模块交流输入故障
	uint8_t _ccu_di_rmacvoltover:1;//充电模块交流输入过压故障
	uint8_t _ccu_di_rmacvoltless:1;//充电模块交流输入欠压故障
	uint8_t _ccu_di_rmacphaseless:1;//充电模块交流输入缺相故障
	uint8_t _ccu_di_rmdcshort:1;//充电模块直流输出短路故障
	uint8_t _ccu_di_rmdccurrover:1;//充电模块直流输出过流故障
	uint8_t _ccu_di_rmdcvoltover:1;//充电模块直流输出过压故障

	uint8_t _ccu_di_rmdcvoltless:1;//充电模块直流输出欠压故障
	uint8_t _ccu_di_rmtemp:1;//充电模块过温故障
	uint8_t _ccu_di_rmcomm:1;//充电模块通信故障
	uint8_t _ccu_di_rmfan:1;//充电模块风扇故障

	uint8_t _ccu_di_carconnect:1;//车辆连接状态
	uint8_t _ccu_di_gunseat:1;//充电桩充电枪座状态
	uint8_t _ccu_di_interfacelock:1;//充电接口电子锁状态
	uint8_t _ccu_di_dcswstatus:1;//直流输出接触器状态

	uint8_t _ccu_other_failure[2];//其它类型故障
}T_YX1;//st_telecommand_dc_1;//遥信帧1内容 CCU->TCU 直流桩，

typedef struct
{
    uint8_t _ccu_di_door:1;//门禁故障
    uint8_t _ccu_di_dcpaste:1;//直流输出接触器粘连故障
    uint8_t _ccu_di_isoalarm:1;//绝缘监测告警
    uint8_t _ccu_di_reserr:1;//泄放回路故障
    uint8_t _ccu_di_piletempalarm:1;//充电桩过温告警
    uint8_t _ccu_di_guntempalarm:1;//充电枪过温告警
    uint8_t _ccu_di_ackmnotdo:1;//交流输入接触器据动/误动故障
    uint8_t _ccu_di_ackmpaste:1;//交流输入接触器粘连故障

    uint8_t _ccu_di_bmspower:1;//辅助电源故障
    uint8_t _ccu_di_buskmnotdo:1;//并联接触器据动/误动故障
    uint8_t _ccu_di_buskmpaste:1;//并联接触器粘连故障

    uint8_t _ccu_other_start1:1;//并联接触器状态
    uint8_t _ccu_other_fault1:1;//模块紧急停机故障
    uint8_t _ccu_other_fault2:1;//充电桩水浸故障
    uint8_t _ccu_other_fault3:1;//充电机柜门禁故障
    uint8_t _ccu_other_fault4:1;//充电机柜水浸故障

    uint8_t _ccu_other_fault5:1;//充电机柜烟感故障
    uint8_t _ccu_other_fault6:1;//功率控制模块通信故障
    uint8_t _ccu_other_fault7:1;//功率控制模块通信告警
    uint8_t _ccu_other_fault8:1;//功率控制模块故障
    uint8_t _ccu_other_fault9:1;//功率控制模块告警
    uint8_t _ccu_other_fault10:1;//开关模块通信故障
    uint8_t _ccu_other_fault11:1;//开关模块通信告警
    uint8_t _ccu_other_fault12:1;//开关模块故障

    uint8_t _ccu_other_fault13:1;//开关模块告警
    uint8_t _ccu_other_fault14:1;//直流充电主控模块判断BMS故障
    uint8_t _ccu_other_fault15:1;//BMS发送故障信息
    uint8_t _ccu_other_fault16:1;//充电模块泄放故障
    uint8_t _ccu_other_fault17:1;//绝缘监测仪通信故障
    uint8_t _ccu_other_fault18:1;//环境监控板通信故障
    uint8_t _ccu_other_fault19:1;//灯板通信故障
    uint8_t _ccu_other_fault20:1;//电能表通信故障

}T_YX2;//st_telecommand_dc_2;//遥信帧2内容 CCU->TCU       直流桩

typedef struct{
	uint8_t			port;
	uint16_t		out_vol;				//充电输出电压
	uint16_t		out_cur;				//充电输出电流
	uint8_t			soc;					//soc
	uint8_t			bat_min_temp;			//电池组最低温度
	uint8_t			bat_max_temp;			//电池组最高温度
	uint16_t		cell_max_vol;			//单体电池最高电压
	uint16_t		cell_min_vol;			//单体电池最低电压
	uint8_t			dev_temp;				//充电机环境温度
	uint16_t		guide_vol;				//充电导引电压
	uint16_t		bms_need_vol;			//BMS需求电压
	uint16_t		bms_need_cur;			//BMS需求电流
	uint8_t			charge_type;			//充电模式
	uint16_t		bms_meas_vol;			//BMS充电电压测量值
	uint16_t		bms_meas_cur;			//BMS充电电流测量值
	uint16_t		remain_charge_time;		//估算剩余充电时间
	uint8_t			dc1_temp;				//充电接口温度探头1温度
	uint8_t			dc2_temp;				//充电接口温度探头2温度
	uint8_t			dc3_temp;				//充电接口温度探头3温度
	uint8_t			dc4_temp;				//充电接口温度探头4温度
	uint8_t			meter_kWh[4];			//电能表当前读数
}T_YC;

typedef struct{
	uint8_t 	port;
	uint8_t 	load_switch;				//负荷开关
	uint8_t 	start_result;				//成功标识
	uint8_t 	cause_failure;				//失败原因
	uint8_t		ccu_bms_ver[3];				//充电桩与bms通讯协议版本
	uint8_t 	bms_ccu_ver[3];				//bms与充电桩通讯协议版本
	uint8_t 	handshake_result;			//充电桩与BMS握手结果
	uint8_t 	battery_type;				//电池类型
	uint8_t 	max_allow_temp;				//最高允许温度  -50度偏移
	uint16_t 	bms_max_allow_voltage;		//BMS最高允许电压
	uint16_t 	unit_max_allow_voltage;		//单体最高允许电压
	int16_t 	max_allow_electricity;		//最高允许充电电流  -400A偏移 ，单位0.1A
	uint16_t 	battery_total_voltage;		//整车动力蓄电池总电压
	uint16_t 	battery_current_voltage;	//整车动力蓄电池当前电压
	uint16_t 	battery_rated_capacity;		//整车动力蓄电池额定容量
	uint16_t 	battery_nominal_energy;		//整车动力蓄电池标称能量
	uint16_t 	battery_capacity;			//整车动力蓄电池荷电状态
	uint16_t 	ccu_max_out_voltage;		//充电机最高输出电压
	uint16_t 	ccu_min_out_voltage;		//充电机最低输出电压
	uint16_t 	ccu_max_out_electricity;	//充电机最高输出电流  -400A偏移 ，单位0.1A
	uint16_t 	ccu_min_out_electricity;	//充电机最低输出电流  -400A偏移 ，单位0.1A
	uint8_t  	vin[17];                	//车辆识别码
}T_START_FINISH;

typedef union{
    struct
    {
        uint32_t soc_reached:2;						//SOC 目标值
        uint32_t total_vol_reached:2;				//总电压设定值
        uint32_t cell_vol_reached:2;				//单体电压的设定值
        uint32_t cst_stop:2;						//充电机主动中止

        uint32_t inslt_fault:2;						//绝缘故障
        uint32_t out_connector_temp_over:2;			//输出连接器过温故障
        uint32_t component_temp_over:2;				//BMS 元件、输出连接器过温
        uint32_t charge_connector_fault:2;			//充电连接器故障

        uint32_t bat_temp_over:2;					//电池组温度过高故障
        uint32_t relay_fault:2;						//高压继电器故障
        uint32_t point_2_vol_detect_fault:2;		//检测点 2 电压检测故障
        uint32_t other_fault:2;						//其他故障

        uint32_t current_over_fault:2;				//电流过大
        uint32_t voltage_fault:2;					//电压异常
    }bits;
    uint32_t val;
}T_BST;

typedef union{
    struct
    {
    	uint32_t b2_crm_00_timeout:2;				//接 收 SPN2560=0x00的充电机辨识报文超时
    	uint32_t b2_crm_aa_timeout:2;				//接 收 SPN2560=0xAA的充电机辨识报文超时
        uint32_t :4;								//预留保证字节对齐
        uint32_t b2_cts_cml_timeout:2;				// 接收充电机的时间同步和充电机最大输出能力报文超时
        uint32_t b2_cro_timeout:2;					//接收充电机完成充电准备报文超时
        uint32_t :4;								//预留保证字节对齐
        uint32_t b2_ccs_timeout:2;					//接收充电机充电状态报文超时
        uint32_t b2_cst_timeout:2;					//接收充电机中止充电报文超时
        uint32_t :4;								//预留保证字节对齐
        uint32_t b2_csd_timeout:2;					//接收充电机充电统计报文超时
        uint32_t b2_others:6;						//bms其他故障
    }bits;
    uint32_t val;
}T_BEM;

typedef struct{
	uint8_t 	port;
	uint8_t 	stop_reason;						//停止原因
	uint8_t 	stop_result;						//成功标识
	uint8_t 	bms_stop_reason;					//bms中止充电原因
	T_BST 		t_bst;
	uint8_t 	stop_soc_status;					//中止荷电状态SOC%
	uint16_t 	battery_monomer_min_voltage;		//动力蓄电池单体最低电压
	uint16_t 	battery_monomer_max_voltage;		//动力蓄电池单体最高电压
	uint8_t 	battery_min_temp;					//动力蓄电池最低温度 -50度偏移
	uint8_t 	battery_max_temp;					//动力蓄电池最高温度 -50度偏移
	T_BEM		t_bem;

	uint8_t 	bms_brm_timeout:2;					//接收 BMS 和车辆的辨识报文超时
	uint8_t 	reserved4:6;						//预留保证字节对齐

	uint8_t 	bms_bcp_timeout:2;					//接收电池充电参数报文超时
	uint8_t 	bms_broaa_timeout:2;				//接收 BMS 完成充 电准备报文超时
	uint8_t 	reserved5:4;						//预留保证字节对齐

	uint8_t 	bms_bcs_timeout:2;					//接收电池充电总状 态报文超时
	uint8_t 	bms_bcl_timeout:2;					//接收电池充电要求报文超时
	uint8_t 	bms_bst_timeout:2;					//接收 BMS 中止充电报文超时
	uint8_t 	reserved6:2;						//预留保证字节对齐

	uint8_t 	bms_bsd_timeout:2;					//接收 BMS 充电统计报文超时
	uint8_t 	timeout_13:6;						//充电机其他错误
}T_STOP_FINISH;

typedef struct{
	uint8_t		port;
	uint8_t		total_dev_type;
	uint8_t		dev_addr;
	uint8_t		set_num;
	uint8_t		pre;
	uint8_t		dev_name[32];
	uint16_t	charge_dev_type;
	uint8_t		dev_type;
	uint8_t		dev_manu[4];
	uint8_t		dev_hard_ver[2];
	uint8_t		dev_soft_ver[2];
	uint8_t		dev_soft_date[4];
	uint8_t		dev_soft_verity[16];
	uint8_t		dev_run_model;
	uint8_t		ccu_addr;
	uint8_t		ccu_tcu_timeout;
	uint8_t		ccu_tcu_pro_ver[2];
	uint8_t		model_addr;
	uint8_t		ccu_model_timeout;
	uint8_t		ccu_model_pro_ver[2];
	uint16_t	rated_out_vol;
	uint16_t 	rated_out_cur;
	uint16_t	max_out_vol;
	uint16_t	max_out_cur;
	uint16_t	min_out_cur;
	uint8_t		bms_pro_ver;
	uint8_t		yx_cycle;
	uint8_t		yc_cycle;
}T_SETTING_DATA;

typedef struct{
	uint8_t		port;
	uint8_t		dev_type;
	uint8_t		dev_addr;
	uint16_t	set_num;
}T_SETTING_QUERY;

typedef struct
{
	uint16_t ms;
	uint8_t min;
	uint8_t hour;
	uint8_t mday:5;
	uint8_t wday:3;
	uint8_t month;
	uint8_t year;
}T_CP56_TIME2A; //用于对时阶段

typedef struct{
	uint32_t	time_250ms;
	uint32_t 	time_500ms;
	uint32_t	time_1s;
	uint32_t 	time_2s;
	uint32_t 	time_5s;
}T_TIME_CYCLE;

typedef struct{
	uint32_t	tcu_cumm_fault_time;			//TCU通讯超时时间
	uint32_t	get_vin_timeout;				//VIN码获取超时时间
	uint32_t 	verity_vin_timeout;				//验证VIN超时
	uint32_t 	get_start_finish_timeout;		//启动完成帧超时
	uint32_t 	get_start_finish_ack_timeout;	//启动完成应答帧超时
	uint32_t 	get_stop_finish_timeout;		//停止完成帧超时
	uint32_t 	get_stop_finish_ack_timeout;	//停止完成帧应答超时
	//ceshi
	uint32_t 	soc_time;
}T_TIME_CHECK;

typedef union{
	struct{
		uint8_t	start_cmd:1;
		uint8_t	vin_ret:1;
		uint8_t	vin_verity:1;
		uint8_t start_finish_ack:1;

		uint8_t	stop_cmd:1;
		uint8_t	stop_finish_ack:1;

	}bits;
	uint8_t	val;
}T_FLOW_VAL;

typedef struct{
	int8_t	start_ack;
	int8_t	stop_ack;
	int8_t 	time_ack;
	int8_t 	version_ack;
	int8_t 	charge_param_ack;
	int8_t 	start_finish;
	int8_t 	stop_finish;
	int8_t 	service_control;
	int8_t	lock_ctrl;
	int8_t 	power_ctrl;
	int8_t 	error;
	int8_t	param_ack;
	int8_t	vin_ctrl;
//	int8_t
}T_SEND_CNT;
#pragma pack()	//取消
typedef	union{
	struct{
		uint32_t	get_param:1;
		uint32_t	get_version:1;
		uint32_t	get_time:1;
		uint32_t	get_service_control:1;
		uint32_t 	cumm_fault:1;
		uint32_t	get_set_val:1;
		uint32_t	get_set_quire:1;
		uint32_t	get_model_verity:1;
		uint32_t	get_model_verity_ack:1;
		uint32_t 	get_verity_vin:1;

	}bits;
	uint32_t val;
}T_TCU_FLAG;

#if 0
struct{
		uint32_t	yc_cnt:1;
		uint32_t	vin_verity_cnt:1;
		uint32_t	time_ack_cnt:1;
		uint32_t	version_ack_cnt:1;
		uint32_t 	charge_param_ack:1;
		uint32_t 	dc_yx1:1;
		uint32_t 	dc_yx2:1;
		uint32_t	charge_cfg_inquire_ack_ant:1;

		uint32_t	stop_cmd:1;


	}bits;
	uint32_t	val;
#endif
/*****************************交互数据结构*************************************/
typedef union{
	struct{
		uint32_t    vin_state:1;			//是否有VIN信息
		uint32_t 	idle:1;					//空闲
		uint32_t 	starting:1;				//启动中
		uint32_t 	startok:1;				//启动成功
		uint32_t 	startfail:1;			//启动失败
		uint32_t	charging:1;				//充电中
		uint32_t 	over:1;					//充电结束,充电状态机结束5s后清零
		uint32_t 	fault:1;				//故障
		uint32_t 	insdet:1;				//绝缘检测中
	    }bits;
	    uint32_t val;
}T_MUTUAL_DATA_STATE;

typedef struct{
	T_MUTUAL_DATA_STATE t_mutual_data_state;
}T_MUTUAL_DATA;

typedef struct{
	uint8_t		flow_state;
	T_FLOW_VAL	t_flow_ctrl;
	T_SEND_CNT		t_send_cnt;
	T_TIME_CYCLE	t_time_cycle;
	T_TIME_CHECK	t_time_check;
	T_CP56_TIME2A	t_cp56_time2a;

	uint8_t 		cumm_ver[2];		//
	uint8_t			start_stop_control;
	uint8_t			lock_num;
	uint8_t			lock_operate;
	uint8_t			power_cmd_type;
	uint16_t		power_param;

	T_START_DATA	t_start_data;
	T_STOP_DATA		t_stop_data;
	T_START_ACK_DATA	t_start_ack_data;
	T_STOP_ACK_DATA		t_stop_ack_data;
	T_SETTING_DATA		t_setting_data;
	T_SETTING_QUERY		t_setting_query;
	T_START_FINISH_ACK	t_start_finish_ack;
	T_STOP_FINISH_ACK 	t_stop_finish_ack;

	T_YX1		t_yx1;
	T_YX2		t_yx2;

	//遥测数据
	float			out_vol;				//充电输出电压
	float			out_cur;				//充电输出电流
	uint8_t			soc;					//soc
	int8_t			bat_min_temp;			//电池组最低温度
	int8_t			bat_max_temp;			//电池组最高温度
	float			cell_max_vol;			//单体电池最高电压
	float			cell_min_vol;			//单体电池最低电压
	int8_t			dev_temp;				//充电机环境温度
	float			guide_vol;				//充电导引电压
	float			bms_need_vol;			//BMS需求电压
	float			bms_need_cur;			//BMS需求电流
	uint8_t			charge_type;			//充电模式
	float			bms_meas_vol;			//BMS充电电压测量值
	float			bms_meas_cur;			//BMS充电电流测量值
	uint16_t		remain_charge_time;		//估算剩余充电时间
	int8_t			dc1_temp;				//充电接口温度探头1温度
	int8_t			dc2_temp;				//充电接口温度探头2温度
	int8_t			dc3_temp;				//充电接口温度探头3温度
	int8_t			dc4_temp;				//充电接口温度探头4温度
	uint8_t			meter_kWh[4];			//电能表当前读数


	T_YC_ACK	t_yc_ack;

	T_VIN_VERITY		t_vin_verity;
	T_VIN_VERITY_ACK	t_vin_verity_ack;
	uint8_t	tcu_tick;

	T_TCU_FLAG	t_tcu_flag;
	T_PARAM_INFO	t_param_info;

	T_MUTUAL_DATA	t_mutual_data;
	//启动完成帧数据
//	uint8_t 	start_result;				//启动结果
	uint8_t 	load_switch;				//负荷开关
	uint8_t 	start_result;				//成功标识
	uint8_t 	start_fause;				//失败原因
	uint8_t		ccu_bms_ver[3];				//充电桩与bms通讯协议版本
	uint8_t 	bms_ccu_ver[3];				//bms与充电桩通讯协议版本
	uint8_t 	handshake_result;			//充电桩与BMS握手结果
	uint8_t 	battery_type;				//电池类型
	int8_t 		max_allow_temp;				//最高允许温度  -50度偏移
	float 		bms_max_allow_voltage;		//BMS最高允许电压
	float 		unit_max_allow_voltage;		//单体最高允许电压
	float 		max_allow_electricity;		//最高允许充电电流  -400A偏移 ，单位0.1A
	float 		battery_total_voltage;		//整车动力蓄电池总电压
	float 		battery_current_voltage;	//整车动力蓄电池当前电压
	float 		battery_rated_capacity;		//整车动力蓄电池额定容量
	float 		battery_nominal_energy;		//整车动力蓄电池标称能量
	float 		battery_capacity;			//整车动力蓄电池荷电状态
	float 		ccu_max_out_voltage;		//充电机最高输出电压
	float 		ccu_min_out_voltage;		//充电机最低输出电压
	float 		ccu_max_out_electricity;	//充电机最高输出电流  -400A偏移 ，单位0.1A
	float 		ccu_min_out_electricity;	//充电机最低输出电流  -400A偏移 ，单位0.1A
	uint8_t  	vin[17];                	//车辆识别码
	//停止完成数据
	uint8_t 	stop_fause;					//stop原因
	uint8_t 	stop_result;				//stop结果
	T_BST 		t_bst;
	uint8_t 	stop_soc;					//stop soc
	float 		batt_min_vol;				//动力蓄电池单体最低电压
	float 		batt_max_vol;				//动力蓄电池单体最高电压
	int8_t 		batt_min_temp;				//动力蓄电池最低温度
	int8_t 		batt_max_temp;				//动力蓄电池最高温度
	T_BEM		t_bem;
}T_TCU_PORT;


typedef enum{
	 E_FAULT_RECT_AUTH,                          ///< 密钥验证
	 E_FAULT_RECT_AC_VOL_OVER,                   ///< 输入过压
	 E_FAULT_RECT_AC_VOL_BELOW,                  ///< 输入欠压
	 E_FAULT_RECT_AC_PHASE_LOSS,                 ///< 输入缺相
	 E_FAULT_RECT_AC_FREQ,                       ///< 输入频率异常
	 E_FAULT_RECT_TEMP_OVER,                     ///< 模块进风口过温
	 E_FAULT_RECT_AC_UNBALANCE,                  ///< 三相电压不平衡
	 E_FAULT_RECT_ADDR_CONFLICT,                 ///< 地址冲突
	 E_FAULT_RECT_DIODE,                         ///< 防反二极管
	 E_FAULT_RECT_BLEED,                         ///< 泄放回路故障
	 E_FAULT_RECT_BLEED_OT,                      ///< 泄放过温，100度
	 E_FAULT_RECT_SOFT_START,                    ///< 软启动故障
	 E_FAULT_RECT_FAN,                           ///< 风扇故障
	 E_FAULT_RECT_PFC_DC_COMM,                   ///< 模块内部通信故障
	 E_FAULT_RECT_SHORT_CUT,                     ///< 输出短路
	 E_FAULT_RECT_CURR_UNBALANCE,                ///< 输出不均流
	 E_FAULT_RECT_DC_VOL_OVER,                   ///< 输出过压
	 E_FAULT_RECT_DC_CUR_OVER,                   ///< 输出过流，88A，软件在100A
	 E_FAULT_RECT_DC_VOL_BELOW,                  ///< 输出欠压
	 E_FAULT_RECT_DC_CUR_BELOW,                  ///< 输出欠流
	 E_FAULT_RECT_PFC,                           ///< pfc总故障
	 E_FAULT_RECT_PFC_OV,                        ///< pfc过压
	 E_FAULT_RECT_PFC_BIAS_VOLT,                 ///< pfc偏压
	 E_FAULT_RECT_PFC_OT,                        ///< pfc过温
	 E_FAULT_RECT_PFC_OVERLOAD,                  ///< pfc过载
	 E_FAULT_RECT_PFC_UNBALANCE,                 ///< pfc串联，内部不平衡
	 E_FAULT_RECT_DCDC,                          ///< DCDC总故障
	 E_FAULT_RECT_MASTER_OT,                     ///< 主变压器，主变过温
	 E_FAULT_RECT_RADIATOR_OT,                   ///< DCDC电路过(温散热器过温?)
	 E_FAULT_RECT_HIGH_RELAY,                    ///< 高低压切换继电器
	 E_FAULT_RECT_OUT_UNBALANCE,                 ///< 输出不平衡，两个500V串联，大概50V马上
	 E_FAULT_RECT_AC_SPD,                        ///< 输入防雷故障

	 E_FAULT_RECT_AC,                            ///< 模块交流故障
	 E_FAULT_RECT_OFFLINE,                       ///< 模块离线
	 E_FAULT_RECT_NO,                            ///< 无模块
	 E_FAULT_RECT_REL_GROUP_TO,                  ///< 模块释放分组超时
	 E_FAULT_RECT_START_TO,                      ///< 模块启动超时
	 E_FAULT_FLOW_RECT_STOP_TO,                  ///< 模块停机超时
	 E_FAULT_NO_RECT_AVAILABLE,                  ///< 无可用整流模块
	 E_FAULT_FLOW_SOFT_START_TO,                 ///< 模块软起动超时
	 E_FAULT_FLOW_RECT_QUICK_START_TO,           ///< 模块快速启动超时

	 //collect
	 E_FAULT_COLLECT_SPD,                        ///< 避雷器故障
	 E_FAULT_COLLECT_AC_RELAY,                   ///< 交流输入接触器故障
	 E_FAULT_COLLECT_AC_BREAKER,                 ///< 交流断路器故障
	 E_FAULT_COLLECT_EMG,                        ///< 急停故障
	 E_FAULT_COLLECT_DOOR,                       ///< 门禁故障
	 E_FAULT_COLLECT_DOOR_B,                     ///< 后端机柜门故障
	 E_FAULT_SMOG,                               ///< 烟感故障
	 E_FAULT_SMOG_B,							 ///  后端机柜烟感故障
	 E_FAULT_DC_FUSE,                            ///< 直流熔断器故障
	 E_FAULT_FAN,                                ///< 风扇故障
	 E_FAULT_GUN_CC,                             ///< 控制导引故障
	 E_FAULT_TEMP_OVER,                          ///< 充电桩过温故障
	 E_FAULT_GUN_TEMP_OVER,                      ///< 充电枪过温故障
	 E_FAULT_COLLECT_BLEED,                      ///< 泄放接触器故障
	 E_FAULT_M_RELAY_LOST_CTRL,                  ///< 母联接触器故障
	 E_FAULT_LOCK1_LOST_CTRL,                    ///< 电子锁故障
	 E_FAULT_RELAY1_STICKY,                      ///< 输出接触器粘连故障（TCU）
	 E_FAULT_BLEED1_STICKY,                      ///< 泄放接触器粘连故障（TCU）
	 E_FAULT_LOCK1_STICKY,                       ///< 电子锁粘连故障（TCU）
	 E_FAULT_AC_RELAY_STICKY,                    ///< 交流接触器粘连故障（TCU）
	 E_FAULT_M_RELAY_STICKY,                     ///< 母联接触器粘连故障（TCU）
	 E_FAULT_COLLECT_POWER_DOWN,                 ///< 掉电故障
	 E_FAULT_DC_RELAY_P,                         ///< 直流接触器正故障
	 E_FAULT_DC_RELAY_N,                         ///< 直流接触器负故障
	 E_FAULT_BOARD_POWER,                        ///< 12V辅助电源故障
	 E_FAULT_BOARD_POWER24,                      ///< 24V辅助电源故障
	 E_FAULT_WATER,                              ///< 水浸故障
	 E_FAULT_WATER_BACK,                         ///< 后端机柜水浸故障
	 //
	 E_FAULT_SELF_CHECKING,						 ///< 自检超时
	 E_FAULT_POWER_BUDGET,						 ///< 功率分配超时

	 E_FAULT_NO_CUR,							 ///< 无有效电流停止
	 E_FAULT_FLOW_BLEED,                         ///< 泄放回路故障
	 E_FAULT_FLOW_BLEED_TO,                      ///< 泄放超时
	 E_FAULT_FLOW_5PERCENT,                      ///< 测量电压与BMS报文中电池电压差值超过5%
	 E_FAULT_FLOW_OVER10,                        ///< 启动充电前电池电压大于10V
	 E_FAULT_PRECHARGE_BAT_VOL_BLELOW_RECT_MIN,  ///< 预充电压小于模块电压最小值
	 E_FAULT_PRECHARGE_BAT_VOL_OVER_RECT_MAX,    ///< 预充电压大于模块最大值
	 E_FAULT_PRECHARGE_BAT_VOL_TIMEOUT,          ///< 预充时未检测到电池电压
	 E_FAULT_FLOW_PERCHARGING_VOLT_TO,           ///< 预充电压未达到要求值
	 E_FAULT_FLOW_PRECHARGE_TO,                  ///< 30S内未完成预充
	 E_FAULT_FLOW_PRECHARGE_ACK_TO,              ///< 65S内未收到BROAA的应答
	 E_FAULT_FLOW_INSLT,                         ///< 绝缘检测故障
	 E_FAULT_FLOW_INSLT_VOLT_TO,                 ///< 绝缘检测电压超时
	 E_FAULT_FLOW_INSLT_TO,                      ///< 绝缘检测超时
	 E_FAULT_BAT_REVERSE,                        ///< 电池反接
	 E_FAULT_AC_VOL_OVER,                        ///< 交流输入过压
	 E_FAULT_AC_VOL_BELOW,                       ///< 交流输入欠压
	 E_FAULT_AC_PHASE_LOSS,                      ///< 交流输入缺相
	 E_FAULT_DC_SHORT_CUT,                       ///< 输出短路
	 E_FAULT_BAT_VOL_MAX_BELOW_RECT_MIN,         ///< 电池最高电压小于模块最低电压
	 E_FAULT_VOL_OVER_BAT_VOL_MAX,               ///< 电池端电压大于电池最高允许充电电压
	 E_FAULT_DEV_OTHER_FAULT,                    ///< 设备其他故障（TCU）
	 E_FAULT_DC_VOL_OVER,                        ///< 输出过压
	 E_FAULT_DC_VOL_BELOW,                       ///< 输出欠压
	 E_FAULT_DC_CUR_OVER,                        ///< 输出过流
	 E_FAULT_FLOW_STOP_TO,                       ///< 停机超时
	 E_FAULT_FLOW_SHAKE_ACK_TO,                  ///< 握手应答超时
	 E_FAULT_FLOW_DC_VOL_OVER_BMS,               ///< 输出电压超过BMS最高允许充电电压
	 E_FAULT_FLOW_DC_VOL_BELOW_BMS,              ///< 输出电压小于BMS电池电压
	 //bms
	 E_FAULT_BMS_VER,							 ///< BMS通信协议版本不匹配
	 E_FAULT_BMS_INSLT,                          ///< BSTBMS绝缘故障
	 E_FAULT_BMS_CONNECTER_OT,                   ///< BST输出连接器过温故障
	 E_FAULT_BMS_COMPONENT_OT,                   ///< BST BMS元件、输出连接器过温
	 E_FAULT_BMS_CONNECTER,                      ///< BST 充电连接器故障
	 E_FAULT_BMS_BAT_OT,                         ///< BST电池组温度过高故障
	 E_FAULT_BMS_HIGH_RELAY,                     ///< BST高压继电器故障
	 E_FAULT_BMS_CHECK_P2,                       ///< BST检测点2电压检测故障
	 E_FAULT_BMS_OTHER,                          ///< BST其他故障
	 E_FAULT_BMS_OUT_OC,                         ///< BST电流过大
	 E_FAULT_BMS_VOLT_ERROR,                     ///< BST电压异常
	 E_FAULT_BMS_BRM_TO,                         ///< CEM接收BMS和车辆的辨识报文超时
	 E_FAULT_BMS_BSM_TO,						 ///< 动力蓄电池状态信息报文超时
	 E_FAULT_BMS_BCP_TO,                         ///< CEM接收电池充电参数报文超时
	 E_FAULT_BMS_BRO_00_TO,                      ///< CEM接收BRO报文(0x00)接收超时
	 E_FAULT_BMS_BRO_AA_TO,                      ///< CEM接收BRO报文(0xAA)接收超时
	 E_FAULT_BMS_BCS_TO,                         ///< CEM接收电池充电总状态报文超时
	 E_FAULT_BMS_BCL_TO,                         ///< CEM接收电池充电要求报文超时
	 E_FAULT_BMS_BST_TO,                         ///< CEM接收BMS中止充电报文超时
	 E_FAULT_BMS_BSD_TO,                         ///< CEM接收BMS充电统计报文超时
	 E_FAULT_BMS_BSM_CELL_OV,                    ///< BSM单体动力蓄电池电压过高
	 E_FAULT_BMS_BSM_CELL_LV,                    ///< BSM单体动力蓄电池电压过低
	 E_FAULT_BMS_BSM_SOC_OVER,                   ///< BSM整车动力蓄电池荷电状态SOC过高
	 E_FAULT_BMS_BSM_SOC_LOWER,                  ///< BSM整车动力蓄电池荷电状态SOC过低
	 E_FAULT_BMS_BSM_BAT_OC,                     ///< BSM动力蓄电池充电过电流
	 E_FAULT_BMS_BSM_BAT_OT,                     ///< BSM动力蓄电池温度过高
	 E_FAULT_BMS_BSM_INSLT,                      ///< BSM动力蓄电池绝缘故障
	 E_FAULT_BMS_BSM_CONNECTOR,                  ///< BSM动力蓄电池组输出连接器状态不正常
	 E_FAULT_BMS_BEM_CRM00_TO,                   ///< BEM接收crm00的充电机辨识报文超时
	 E_FAULT_BMS_BEM_CRMAA_TO,                   ///< BEM接收crmaa的充电机辨识报文超时
	 E_FAULT_BMS_BEM_CTS_CML_TO,                 ///< BEM接收CTS和CML报文超时
	 E_FAULT_BMS_BEM_CRO_TO,                     ///< BEM接收CRO充电机完成充电准备报文超时
	 E_FAULT_BMS_BEM_CCS_TO,                     ///< BEM接收CCS充电机充电状态报文超时
	 E_FAULT_BMS_BEM_CST_TO,                     ///< BEM接收CST充电机中止充电报文超时
	 E_FAULT_BMS_BEM_CSD_TO,                     ///< BEM接收CSD充电机充电统计报文超时
	 E_FAULT_BMS_WAIT_PRECHARGE_CMD_TO,          ///< 自绝缘检测开始，30S内未收到预充指令
	 E_FAULT_BMS_PAUSE_OT,                       ///< 暂停充电超过10分钟
	 E_FAULT_BMS_BRO00_AFTER_BROAA,              ///< 收到BROAA后又收到BRO00
	 E_FAULT_BMS_WAIT_CHARGE_CMD_TO,             ///< 等待充电指令超时
	 E_FAULT_BSM_TIMOUT,                         ///< BSM报文超时
	 E_FAULT_BMS_BCS_CELL_OV,                    ///< BCS中单体最高电压大于BCP中最高允许单体电压
	 E_FAULT_BMS_NEED_VOLT_OVER_MAX,             ///< BCL中需求电压大于BCP中最高允许充电电压
	 E_FAULT_BMS_NEED_CURR_OVER_MAX,             ///< BCL中需求电流大于BCP中最大充电电流
	 E_FAULT_BRM_DATA_ERROR,                     ///< BRM数据粗错误（TCU）
	 E_FAULT_BCP_DATA_ERROR,                     ///< BCP数据错误（TCU）

	 E_FAULT_TCU_START_TIMEOUT,                  ///< TCU启动超时
	 E_FAULT_TCU_STOP_TIMEOUT,                   ///< TCU停机超时
	 E_FAULT_START_CMD_ERROR,                    ///< 启动指令错误
	 E_FAULT_START_FINISH_ACK_TIMEOUT,           ///< 启动完成应答超时
	 E_FAULT_TCU_VIN_CODE_TO,                    ///< 等待VIN码超时
	 E_FAULT_VIN_VERIFY_TIMEOUT,                 ///< VIN验证超时
	 E_FAULT_TCU_VIN_VERIFY_FAIL,                ///< VIN验证失败

	 E_FAULT_LED_COMM,							 // 灯板通信故障
	 E_FAULT_SWITCH,							 // 开关模块故障
	 E_FAULT_SWITCH_COMM,                        ///< 开关模块通信故障
	 E_FAULT_METER_COMM,                         ///< 电表通讯故障
	 E_FAULT_TCU_COMMUNICATION,                  ///< TCU通讯故障
	 E_FAULT_BMS_COMMUNICATION,                  ///< BMS通讯故障
	 E_FAULT_READER_COMM,                        ///< 读卡器通讯故障
	 E_FAULT_RECT_COMM,                          ///< 模块通信故障
	 E_FAULT_MAS_POWER_CTRL_CUMM,				 // 直流充电主控模块与功率控制模块通信超时

	 E_ALARM_RECT_AUTH,                          ///< 密钥验证  此处开始共32个模块遥信报文告警
	 E_ALARM_RECT_AC_VOL_OVER,                   ///< 输入过压
	 E_ALARM_RECT_AC_VOL_BELOW,                  ///< 输入欠压
	 E_ALARM_RECT_AC_PHASE_LOSS,                 ///< 输入缺相
	 E_ALARM_RECT_AC_FREQ,                       ///< 输入频率异常
	 E_ALARM_RECT_TEMP_OVER,                     ///< 模块进风口过温
	 E_ALARM_RECT_AC_UNBALANCE,                  ///< 三相电压不平衡
	 E_ALARM_RECT_ADDR_CONFLICT,                 ///< 地址冲突
	 E_ALARM_RECT_DIODE,                         ///< 防反二极管
	 E_ALARM_RECT_BLEED,                         ///< 泄放回路告警
	 E_ALARM_RECT_BLEED_OT,                      ///< 泄放过温，100度
	 E_ALARM_RECT_SOFT_START,                    ///< 软启动告警
	 E_ALARM_RECT_FAN,                           ///< 风扇告警
	 E_ALARM_RECT_PFC_DC_COMM,                   ///< 模块内部通信告警
	 E_ALARM_RECT_SHORT_CUT,                     ///< 输出短路
	 E_ALARM_RECT_CURR_UNBALANCE,                ///< 输出不均流
	 E_ALARM_RECT_DC_VOL_OVER,                   ///< 输出过压
	 E_ALARM_RECT_DC_CUR_OVER,                   ///< 输出过流，88A，软件在100A
	 E_ALARM_RECT_DC_VOL_BELOW,                  ///< 输出欠压
	 E_ALARM_RECT_DC_CUR_BELOW,                  ///< 输出欠流
	 E_ALARM_RECT_PFC,                           ///< pfc总告警
	 E_ALARM_RECT_PFC_OV,                        ///< pfc过压
	 E_ALARM_RECT_PFC_BIAS_VOLT,                 ///< pfc偏压
	 E_ALARM_RECT_PFC_OT,                        ///< pfc过温
	 E_ALARM_RECT_PFC_OVERLOAD,                  ///< pfc过载
	 E_ALARM_RECT_PFC_UNBALANCE,                 ///< pfc串联，内部不平衡
	 E_ALARM_RECT_DCDC,                          ///< DCDC总告警
	 E_ALARM_RECT_MASTER_OT,                     ///< 主变压器，主变过温
	 E_ALARM_RECT_RADIATOR_OT,                   ///< DCDC电路过(温散热器过温?)
	 E_ALARM_RECT_HIGH_RELAY,                    ///< 高低压切换继电器
	 E_ALARM_RECT_OUT_UNBALANCE,                 ///< 输出不平衡，两个500V串联，大概50V马上
	 E_ALARM_RECT_AC_SPD,                        ///< 输入防雷告警

	 E_ALARM_FAN,                                ///< 风扇告警
	 E_ALARM_GUN_TEMP,                           ///< 枪温告警
	 E_ALARM_DEV_TEMP,                           ///< 桩温告警

	 E_ALARM_FLOW_INSLT,                         ///< 绝缘告警
	 E_ALARM_SWITCH_COMM,                        ///< 开关模块通信告警
	 E_ALARM_GUN_PARKED,                         ///< 枪未归位告警
	 E_ALARM_RECT_COMM,                          ///< 模块通信告警
	 E_ALARM_RECT_AC,                            ///< 模块交流告警

	 E_STOP_BMS_REACH_SOC,                       ///< BST达到所需求的SOC目标值
	 E_STOP_BMS_REACH_TOTAL_VOL,                 ///< BST达到总电压的设定值
	 E_STOP_BMS_REACH_CELL_VOL,                  ///< BST达到单体电压的设定值
	 E_STOP_BMS_CHARGER_STOP,                    ///< BST充电机主动中止
	 E_STOP_BMS_NORMAL,                          ///< BMS正常停止充电（TCU）
	 E_STOP_BMS_FAULT,                           ///< BMS故障停止充电（TCU）
	 E_STOP_BMS_OTHER_FAULT,                     ///< BMS其他故障停机充电（TCU）
	 E_STOP_BMS_CHARGE_FAULT,					 ///< 充电桩判断BMS其他异常终止
	 E_FAULT_MAX_CNT,
}E_FAULT_ID;

/////////***************************************************************//////////////////////////

////////****************************交互数据*****************************/////////////////

void tcu_com_handle(void);
int32_t recv_handle(T_TCU_PORT* pt_ports,T_SG_DATA *pt_sg_data,T_SYS_CONFIG *pt_config);


extern T_TCU_PORT	t_ports[2];
extern 	T_SG_DATA		t_sg_data;
extern 	T_SYS_CONFIG	t_config;

#endif /* CANTCU_H_ */
