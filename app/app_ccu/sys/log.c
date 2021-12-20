/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技
文件:   log
作者:   km
说明:   串口打印
***************************************************************************************************/
#include "log.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "log.h"
#include "kfc_dev.h"

#define     UART_BUFFER_SIZE        512
static char txbuf[UART_BUFFER_SIZE] = {0};
static uint8_t hexbuf[UART_BUFFER_SIZE] = {0};

void log_d(const char *format, ...)
{
    va_list args;
    unsigned int  length;

	memset(txbuf, 0, UART_BUFFER_SIZE);
    va_start(args, format);
    length = vsnprintf(txbuf, sizeof(txbuf), format, args);
    va_end(args);
    
    if(length<UART_BUFFER_SIZE-1)
    {
        txbuf[length] = '\r';
        txbuf[length+1] = '\n';
        length += 2;
    }
    else
    {
        txbuf[UART_BUFFER_SIZE-2] = '\r';
        txbuf[UART_BUFFER_SIZE-1] = '\n';
        length=UART_BUFFER_SIZE;        
    }    

	KfcDevWrite(kSfDevNumLog, txbuf, length);
    //usart3_out_data(txbuf, length);
}

/*
  * @brief 打印16进制字符串
  * @param head 字符串说明
           data 16进制数据串
           len  数据总字节数
           cmd  报文命令符
*/

void log_hex(char *head, uint8_t *data, uint8_t len, uint32_t cmd)
{
    char tmp[5] = {0};
    uint16_t i=0;
    uint8_t maxi = len / (UART_BUFFER_SIZE/2-1);
    uint8_t maxj = len % (UART_BUFFER_SIZE/2-1);
	memset(hexbuf, 0, UART_BUFFER_SIZE);
    
    DLOG("%s[0x%x,%d]:",head,cmd,len);
    if(maxi == 0)
    {
        for(i=0;i<maxj;i++)
        {
            snprintf(tmp,sizeof(tmp),"%02x ",data[i]);
            hexbuf[2*i] = tmp[0];
            hexbuf[2*i+1] = tmp[1];            
        }
        DLOG("%s",hexbuf);
    }
    
}

void log_show(const char *format, ...)
{
}

void log_flash(const char *format, ...)
{
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/


