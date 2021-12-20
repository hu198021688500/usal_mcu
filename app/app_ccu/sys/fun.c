/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件: fun
作者:   km
说明:   功能函数.
***************************************************************************************************/
#include "fun.h"
#include <stdio.h>
#include <string.h>

static uint8_t g_ucString[32];

/**
  * @brief  用于记录校验
  */
uint16_t check_sum(const uint8_t * buffer, uint16_t length)
{
    uint16_t initCrc = 0xffff;
    uint16_t crc = 0;
    int i = 0;
    int j = 0;

    if ((buffer == 0) || (length <= 0))
    {
        return 0;
    }

    for(i=0; i<length; i++)
    {
        initCrc ^=  buffer[i];
        for(j=0; j<8; j++)
        {
            crc = initCrc;
            initCrc >>= 1;
            if(crc&0x0001)
                initCrc ^= 0xa001;
        }
    }

    return initCrc;
}

/**
  * @brief  和校验，四字节
  */
uint32_t addchecksum(const uint8_t* buffer, uint32_t length)
{
    uint32_t sum=0;
    uint32_t i=0;

    if (!buffer)
    {
        return 0;
    }

    for(i=0;i<length;i++)
    {
        sum = sum + *buffer;
        buffer++;
    }

    return sum;
}

int fix_bcd_code(int code)
{
    if((code & 0x0f)>0x09)
    {
        return 0;
    }
    
    if(((code>>4)&0x0f)>0x09)
    {
        return 0;
    }
    
    return (((code)&0x0f) + ((code)>>4)*10);
}

int get_data_form_string(DATA_TYPE type,int index,int max,char *buffer)
{
    int idata = 0;
    
    if (!buffer)
    {
        return 0;
    }
    
    switch(type)
    {
        case DATA_TYPE_INT:
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_BIGINT32U://大端模式
             idata = 4;
        break;
        case DATA_TYPE_CHAR:
        case DATA_TYPE_UCHAR:
             idata = 1;
        break;
        case DATA_TYPE_INT16U:
        case DATA_TYPE_INT16S:
        case DATA_TYPE_BIGINT16U://大端模式
             idata = 2;
        break;
        default:
        break;
    }
    
    if ((index + idata) > max)
    {
        return 0;
    }
    
    idata = 0;
    
    switch(type)
    {
        case DATA_TYPE_INT:
        {
            idata = buffer[index]+(buffer[index+1]<<8)+(buffer[index+2]<<16)+(buffer[index+3]<<24);
        }
        break;
        case DATA_TYPE_CHAR:
        {
            idata = buffer[index];
        }
        break;
        case DATA_TYPE_UCHAR:
        {
            uint32_t data = (unsigned char) buffer[index];
            idata = data;
        }
        break;
        case DATA_TYPE_INT16U:
        {
            uint32_t data = buffer[index]+(buffer[index+1]<<8);
            idata = data;
        }
        break;
        case DATA_TYPE_INT16S:
        {
            int16_t data = buffer[index]+(buffer[index+1]<<8);
            idata = data;
        }
        break;
        case DATA_TYPE_BIGINT16U://大端模式
        {
            idata = buffer[index+1]+(buffer[index]<<8);
        }
        break;
        case DATA_TYPE_BIGINT32U://大端模式
        {
            idata = buffer[index+3]+(buffer[index+2]<<8)+(buffer[index+1]<<16)+(buffer[index]<<24);
        }
        break;
        default:
            idata = 0;
            break;
    }
    
    return idata;
}

void add_data_to_string(DATA_TYPE type,int index,int max,char *buffer,int sc)
{
    int idata = 0;
   
    switch(type)
    {
        case DATA_TYPE_INT:
        case DATA_TYPE_FLOAT:
        case DATA_TYPE_BIGINT32U://大端模式
             idata = 4;
        break;
        case DATA_TYPE_CHAR:
        case DATA_TYPE_UCHAR:
             idata = 1;
        break;
        case DATA_TYPE_INT16U:
        case DATA_TYPE_INT16S:
        case DATA_TYPE_BIGINT16U://大端模式
             idata = 2;
        break;
        default:
        break;
    }
    
    if ((index + idata) > max)
    {
        return;
    }
        
    switch(type)
    {
        case DATA_TYPE_INT:
        {
            buffer[index]=sc&0xff;
            buffer[index+1]=(sc>>8)&0xff;            
            buffer[index+2]=(sc>>16)&0xff;            
            buffer[index+3]=(sc>>24)&0xff;                        
        }
        break;
        case DATA_TYPE_CHAR:
        {
            buffer[index]=sc&0xff;;
        }
        break;
        case DATA_TYPE_UCHAR:
        {
            buffer[index]=sc&0xff;;
        }
        break;
        case DATA_TYPE_INT16U:
        {
            buffer[index]=sc&0xff;
            buffer[index+1]=(sc>>8)&0xff;   
        }
        break;
        case DATA_TYPE_INT16S:
        {
            buffer[index]=sc&0xff;
            buffer[index+1]=(sc>>8)&0xff;   
        }
        break;
        case DATA_TYPE_BIGINT16U://大端模式
        {
            buffer[index+1]=sc&0xff;
            buffer[index]=(sc>>8)&0xff;   
        }
        break;
        case DATA_TYPE_BIGINT32U://大端模式
        {
            buffer[index+3]=sc&0xff;
            buffer[index+2]=(sc>>8)&0xff;            
            buffer[index+1]=(sc>>16)&0xff;            
            buffer[index]=(sc>>24)&0xff;  
        }
        break;
        default:
            break;
    }
}

char *get_string_form_string(int index,int len,int max,char *buffer)
{
    if (!buffer)
    {
        return 0;
    }
    
    if ((index + len) > max)
    {
        return 0;
    }
    
    return &buffer[index];
}

uint8_t *hex_to_string(uint8_t ch)
{    
    uint8_t udata = (ch >>4);
    
    if(udata <=9)
    {
        g_ucString[0] = 0x30+udata;
    }
    else if((udata >= 10))
    {
        g_ucString[0] = 'A'+(udata-10);
    }
    else
    {
        g_ucString[0] = 0;
    }
    
    
    udata = ch&0x0f;
    
    if(udata <=9)
    {
        g_ucString[1] = 0x30+udata;
    }
    else if((udata >= 10))
    {
        g_ucString[1] = 'A'+(udata-10);
    }
    else
    {
        g_ucString[1] = 0;
    }
    
    g_ucString[2] = ' ';
    
    g_ucString[3] = 0;
    
    return g_ucString;
    
}

BOOL check_time_range(int8_t hour, int8_t min, int16_t hourStart, int16_t minStart, int16_t hourEnd, int16_t minEnd)
{
    if((0 == hourStart) && (0 == minStart) && (23 == hourEnd) && (59 == minEnd))
    {
        return BOOL_TRUE;
    }
    else if((0 == hourStart) && (0 == minStart) && (0 == hourEnd) && (0 == minEnd))
    {
        return BOOL_FALSE;
    }
    else if(hourStart <= hourEnd)
    {
        if((hour > hourStart) && (hour < hourEnd))
        {
            return BOOL_TRUE;
        }
        else if(hourStart == hourEnd)
        {                       
            if((hour == hourStart) && (min >= minStart) && (min < minEnd))
            {
                return BOOL_TRUE;
            }
            else
            {
                return BOOL_FALSE;
            }
        }
        else
        {
            if(((hour == hourStart) && (min >= minStart))
             ||((hour == hourEnd)   && (min < minEnd)))
            {
                return BOOL_TRUE;
            }
            else
            {
                return BOOL_FALSE;
            }   
        }
    }
    else
    {
        if((hour > hourStart) || (hour < hourEnd))
        {
            return BOOL_TRUE;
        }
        else
        {
            if(((hour ==hourStart) && (min >= minStart))
            || ((hour == hourEnd)  && (min < minEnd)))
            {
                return BOOL_TRUE;
            }
            else
            {
                return BOOL_FALSE;
            }   
        }
    }
}

BOOL checkVinValidStr(int8_t * pCarVin)
{    
    for(int32_t i = 0; i < 17; i++)
    {
        if(('0' <= pCarVin[i]) && ('9' >= pCarVin[i]))
        {
            continue;
        }
        else if(('a' <= pCarVin[i]) && ('z' >= pCarVin[i]))
        {
            continue;
        }
        else if(('A' <= pCarVin[i]) && ('Z' >= pCarVin[i]))
        {
            continue;
        }
        else
        {
            return BOOL_FALSE;
        }
    }
    return BOOL_TRUE;
}

/**
  * @brief  cnt根据flag增加（flag==1）或减少到阈值（flag==0）
  * @param  增加、减少的阶梯为add sub
  * @retval 达到上限high返回1，达到下限low返回0，否则返回2
  */
int8_t cnt_change_until_threshold(int8_t flag,int32_t *cnt,int32_t threshold,int32_t add,int32_t sub)
{
    if(flag)
    {
        *cnt = (*cnt) + add;
        if(*cnt >= threshold)
        {
            *cnt = threshold;
            return 1;
        }
    }
    else
    {
        *cnt = (*cnt) - sub;
        if(*cnt <= 0)
        {
            *cnt = 0;
            return 0;
        }        
    }

    return 2;         
}

/**
  * @brief  手动延时。。
  * @param  cnt每循环进一次自加，直到达到阈值
  * @retval 达到阈值返回1，否则0
  */
BOOL cnt_delay(int32_t *cnt,int32_t threshold)
{
    *cnt = (*cnt) + 1;
    if(*cnt >= threshold)
    {
        *cnt = threshold;
        return BOOL_TRUE;
    }

    return BOOL_FALSE;         
}

/**
  * @brief  循环
  * @param  cnt每循环进一次自加
  * @retval 每隔high返回1，然后归零重新循环
  */
BOOL cnt_cycle(int32_t *cnt,int32_t high)
{
    (*cnt) ++;
    if(*cnt >= high)
    {
        *cnt = 0;
        return BOOL_TRUE;
    }
    return BOOL_FALSE;   
}

int32_t _IQsat(int32_t i32Value, int32_t i32ValueMax,int32_t i32ValueMin)
{
	if (i32Value>i32ValueMax)
	{
		i32Value=i32ValueMax;
	}
	else if (i32Value<i32ValueMin)
	{
		i32Value=i32ValueMin;
	}
  
    return(i32Value);
}

void vIoFilterDeal(uint16_t uIoInFlag,int32_t* iFilterCnt ,uint16_t uTimeNum,uint16_t uTimePosNum,uint16_t uTimeNegNum,uint32_t *uReturnRe,uint16_t uBitNum)
{	
	if(uIoInFlag==1)
	{
		(*iFilterCnt)=(*iFilterCnt)+uTimePosNum;
		if((*iFilterCnt)>uTimeNum)
		{
			(*iFilterCnt)=uTimeNum;
			(*uReturnRe)=(*uReturnRe)|(1<<uBitNum);			
		}
		
	}
	else
	{
		(*iFilterCnt)=(*iFilterCnt)-uTimeNegNum;
		if((*iFilterCnt)<=0)
		{
			(*iFilterCnt)=0;
			(*uReturnRe)=(*uReturnRe)&(0xffffffff-(1<<uBitNum));
		}
	}
}


void sys_clock_handle(structSysClockCnt *sSysClockCnt,unionSysClockFlag *unSysClockFlag)
{
    unSysClockFlag->bit.b1ms = 1;
    
    unSysClockFlag->bit.b4ms=0;
    sSysClockCnt->iNumFor4ms++;
    if(sSysClockCnt->iNumFor4ms>=4)
    {
        sSysClockCnt->iNumFor4ms=0;
        unSysClockFlag->bit.b4ms=1;
    }

    unSysClockFlag->bit.b10ms=0;
    sSysClockCnt->iNumFor10ms++;
    if(sSysClockCnt->iNumFor10ms>=10)
    {
        sSysClockCnt->iNumFor10ms=0;
        unSysClockFlag->bit.b10ms=1;
    }
    
    unSysClockFlag->bit.b20ms=0;
    sSysClockCnt->iNumFor20ms++;
    if(sSysClockCnt->iNumFor20ms>=20)
    {
        sSysClockCnt->iNumFor20ms=0;
        unSysClockFlag->bit.b20ms=1;
    }
    
    unSysClockFlag->bit.b50ms=0;
    sSysClockCnt->iNumFor50ms++;
    if(sSysClockCnt->iNumFor50ms>=50)
    {
        sSysClockCnt->iNumFor50ms=0;
        unSysClockFlag->bit.b50ms=1;
    }

    unSysClockFlag->bit.b100ms=0;
    sSysClockCnt->iNumFor100ms++;
    if(sSysClockCnt->iNumFor100ms>=100)
    {
        sSysClockCnt->iNumFor100ms=0;
        unSysClockFlag->bit.b100ms=1;
    }

    unSysClockFlag->bit.b200ms=0;
    sSysClockCnt->iNumFor200ms++;
    if(sSysClockCnt->iNumFor200ms>=200)
    {
        sSysClockCnt->iNumFor200ms=0;
        unSysClockFlag->bit.b200ms=1;
    }

    unSysClockFlag->bit.b250ms=0;
    sSysClockCnt->iNumFor250ms++;
    if(sSysClockCnt->iNumFor250ms>=250)
    {
        sSysClockCnt->iNumFor250ms=0;
        unSysClockFlag->bit.b250ms=1;
    }
    
    unSysClockFlag->bit.b300ms=0;
    sSysClockCnt->iNumFor300ms++;
    if(sSysClockCnt->iNumFor300ms>=300)
    {
        sSysClockCnt->iNumFor300ms=0;
        unSysClockFlag->bit.b300ms=1;
    }
    
    unSysClockFlag->bit.b500ms=0;
    sSysClockCnt->iNumFor500ms++;
    if(sSysClockCnt->iNumFor500ms>=500)
    {
        sSysClockCnt->iNumFor500ms=0;
        unSysClockFlag->bit.b500ms=1;
    }

    unSysClockFlag->bit.b1s=0;
    sSysClockCnt->iNumFor1s++;
    if(sSysClockCnt->iNumFor1s>=1000)
    {
        sSysClockCnt->iNumFor1s=0;
        unSysClockFlag->bit.b1s=1;
        
        sSysClockCnt->secondCnt++;
    }

    unSysClockFlag->bit.b2s=0;
    sSysClockCnt->iNumFor2s++;
    if(sSysClockCnt->iNumFor2s>=2000)
    {
        sSysClockCnt->iNumFor2s=0;
        unSysClockFlag->bit.b2s=1;
    }

    unSysClockFlag->bit.b3s=0;
    sSysClockCnt->iNumFor3s++;
    if(sSysClockCnt->iNumFor3s>=3000)
    {
        sSysClockCnt->iNumFor3s=0;
        unSysClockFlag->bit.b3s=1;
    }

    unSysClockFlag->bit.b5s=0;
    sSysClockCnt->iNumFor5s++;
    if(sSysClockCnt->iNumFor5s>=5000)
    {
        sSysClockCnt->iNumFor5s=0;
        unSysClockFlag->bit.b5s=1;
    }

    unSysClockFlag->bit.b7s=0;
    sSysClockCnt->iNumFor7s++;
    if(sSysClockCnt->iNumFor7s>=7000)
    {
        sSysClockCnt->iNumFor7s=0;
        unSysClockFlag->bit.b7s=1;
    }

    unSysClockFlag->bit.b10s=0;
    sSysClockCnt->iNumFor10s++;
    if(sSysClockCnt->iNumFor10s>=10000)
    {
        sSysClockCnt->iNumFor10s=0;
        unSysClockFlag->bit.b10s=1;
    }

    unSysClockFlag->bit.b11s=0;
    sSysClockCnt->iNumFor11s++;
    if(sSysClockCnt->iNumFor11s>=11000)
    {
        sSysClockCnt->iNumFor11s=0;
        unSysClockFlag->bit.b11s=1;
    }
    
    unSysClockFlag->bit.b13s=0;
    sSysClockCnt->iNumFor13s++;
    if(sSysClockCnt->iNumFor13s>=13000)
    {
        sSysClockCnt->iNumFor13s=0;
        unSysClockFlag->bit.b13s=1;
    }

    unSysClockFlag->bit.b20s=0;
    sSysClockCnt->iNumFor20s++;
    if(sSysClockCnt->iNumFor20s>=20000)
    {
        sSysClockCnt->iNumFor20s=0;
        unSysClockFlag->bit.b20s=1;
    }

    unSysClockFlag->bit.b40s=0;
    sSysClockCnt->iNumFor40s++;
    if(sSysClockCnt->iNumFor40s>=40000)
    {
        sSysClockCnt->iNumFor40s=0;
        unSysClockFlag->bit.b40s=1;
    }
    
    unSysClockFlag->bit.b1min=0;
    sSysClockCnt->iNumFor1min++;
    if(sSysClockCnt->iNumFor1min>=60000)
    {
        sSysClockCnt->iNumFor1min=0;
        unSysClockFlag->bit.b1min=1;
    }
    
    unSysClockFlag->bit.b3min=0;
    sSysClockCnt->iNumFor3min++;
    if(sSysClockCnt->iNumFor3min>=180000)
    {
        sSysClockCnt->iNumFor3min=0;
        unSysClockFlag->bit.b3min=1;
    }
    
    unSysClockFlag->bit.b10min=0;
    sSysClockCnt->iNumFor10min++;
    if(sSysClockCnt->iNumFor10min>=600000)
    {
        sSysClockCnt->iNumFor10min=0;
        unSysClockFlag->bit.b10min=1;
    }
    
    unSysClockFlag->bit.b1hour=0;
    sSysClockCnt->iNumFor1hour++;
    if(sSysClockCnt->iNumFor1hour>=3600000)
    {
        sSysClockCnt->iNumFor1hour=0;
        unSysClockFlag->bit.b1hour=1;
    }
}

void bcd_time_to_timestring(char *bcdTime,char *stringTime)
{
    if(!bcdTime)
    {
        return ;
    }
    
    if(!stringTime)
    {
        return ;
    }
    
    sprintf(stringTime,"%02x%02x-%02x-%02x  %02x:%02x:%02x",bcdTime[0],bcdTime[1],bcdTime[2],bcdTime[3],bcdTime[4],bcdTime[5],bcdTime[6]);
}

void hexstring_to_assicstring(uint8_t* src,uint8_t* dec,int32_t len)
{
    int32_t i = 0;
    int32_t udata = 0;
    
    if ((!src)||(!dec))
    {
        return;
    }
    
    for (i=0;i<len;i++)
    {
        udata = src[i] >>4;
        
        if((udata >= 0)&&(udata <=9))
        {
            dec[i*2] = '0'+udata;
        }
        else if((udata >= 10))
        {
            dec[i*2] = 'A'+(udata-10);
        }
        else
        {
            dec[i*2+1] = 0;
        }
        
        udata = src[i]&0x0f;
        
        if((udata >= 0)&&(udata <=9))
        {
            dec[i*2+1] = '0'+udata;
        }
        else if((udata >= 10))
        {
            dec[i*2+1] = 'A'+(udata-10);
        }
        else
        {
            dec[i*2+1] = 0;
        }
        
    }
}


/***************************************************************************************************
文件结束.
***************************************************************************************************/

