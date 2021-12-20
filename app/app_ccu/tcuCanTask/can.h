/*
 * can.h
 *
 *  Created on: Oct 10, 2021
 *      Author: player
 */

#ifndef CAN_H_
#define CAN_H_

#include "fun.h"

#define PF_CAN                          29
#define AF_CAN                          PF_CAN


#define CAN_TX_MSG_LIST_MAX 64
#define CAN_RX_MSG_LIST_MAX 128

#define CAN_TX_INTERVAL     10


typedef enum
{
	CAN_BUAD_10K = 0,
	CAN_BUAD_20K,
	CAN_BUAD_50K,
	CAN_BUAD_100K,
	CAN_BUAD_125K,
	CAN_BUAD_250K,
} CAN_BAUD_E;


typedef struct
{
	uint32_t id;
	uint8_t data[8];
	uint8_t len;
	uint8_t ch;
	uint8_t format;
	uint8_t type;
} CAN_MSG_T;

int32_t 	s32_can_init(uint8_t	can_index,uint32_t	baud);

#endif /* CAN_H_ */
