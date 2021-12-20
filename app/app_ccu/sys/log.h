/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   log
作者:   km
说明:   串口打印
***************************************************************************************************/

#ifndef __LOG_H__                     
#define __LOG_H__

#include "base_type.h"

/*
  * @brief 打印分成三个等级，log_d∈log_show∈log_flash
  * log_d 串口打印(DEBUG)
  * log_hex 串口打印HEX
  * log_show 串口打印+屏幕显示
  * log_flash 串口打印+屏幕显示+FLASH日志
  */
void log_d(const char *format, ...);
void log_hex(char *head, uint8_t *data, uint8_t len, uint32_t cmd);
void log_show(const char *format, ...);
void log_flash(const char *format, ...);

#ifdef CHECK_VERSION
#define DLOG(format, args...)
//#elif(DCHARGER_VERSION_BRANCH % 2 == 1)
//#define DLOG log_d
#else
#define DLOG log_d
#define XJ_LOG log_d
//#define DLOG(format, args...)
#endif





#endif  //__CFUN_H__

/***************************************************************************************************
文件结束.
***************************************************************************************************/


