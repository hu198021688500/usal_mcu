/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件: fun
作者:   km
说明:   功能函数.
***************************************************************************************************/


#ifndef __CFUN_H__                     
#define __CFUN_H__

#include "base_type.h"

typedef union 
{
    uint32_t all;
    struct 
    {
        uint32_t    b1ms                    :1;// BIT0
        uint32_t    b4ms                    :1; 
        uint32_t    b10ms                   :1;  
        uint32_t    b20ms                   :1;         
        uint32_t    b50ms                   :1; 
        uint32_t    b100ms                  :1; 
        uint32_t    b200ms                  :1; 
        uint32_t    b250ms                  :1;
        uint32_t    b300ms                  :1;// BIT8
        uint32_t    b500ms                  :1; 
        uint32_t    b1s                     :1;
        uint32_t    b2s                     :1;            
        uint32_t    b3s                     :1;                
        uint32_t    b5s                     :1;              
        uint32_t    b10s                    :1;
        uint32_t    b20s                    :1;
        
        uint32_t    b40s                    :1;//BIT10
        uint32_t    b1min                   :1;
        uint32_t    b3min                   :1;
        uint32_t    b10min                  :1;
        uint32_t    b1hour                  :1;
        uint32_t    b7s                     :1;
        uint32_t    b11s                    :1;
        uint32_t    b13s                    :1;
    }bit;
}unionSysClockFlag;

typedef struct 
{
    uint16_t iNumFor1ms;
    uint16_t iNumFor4ms;
    uint16_t iNumFor10ms;
    uint16_t iNumFor20ms;
    uint16_t iNumFor50ms;
    uint16_t iNumFor100ms;
    uint16_t iNumFor200ms;
    uint16_t iNumFor250ms;
    uint16_t iNumFor300ms;
    uint16_t iNumFor500ms;
    uint16_t iNumFor1s;
    uint16_t iNumFor2s;
    uint16_t iNumFor3s;
    uint16_t iNumFor5s;
    uint16_t iNumFor10s;
    uint16_t iNumFor20s;
    uint16_t iNumFor40s;
    uint16_t iNumFor1min;
    uint32_t iNumFor3min;
    uint32_t iNumFor10min;
    uint32_t iNumFor1hour;
    uint16_t iNumFor7s;
    uint16_t iNumFor11s;
    uint16_t iNumFor13s;
    
    uint32_t secondCnt;
}structSysClockCnt;

#define SYS_SET_BIT(value,bit)  (value = (value|(1 << bit)))
#define SYS_CLR_BIT(value,bit)  (value = (value &= ~(1 << bit)))
#define SYS_CHK_BIT(value,bit)  ((value & (1 << bit)))

#define FROM_BCD(val)   (((val)>>4)*10 + ((val)&0x0F))

#define BCDTOHEX(val) ((val)=(((val)>>4)*10) + ((val)&0x0F))
#define HEXTOBCD(val) ((val)=((((val)/10)<<4) + (((val)%10)&0x0F)))
#define DECTOHEX(val) ((val)=(((val)/1000)<<12) + ((((val)%1000)/100)<<8) + ((((val)%100)/10)<<4) + ((val)%10))//16bit,170801 groupch
#define BIGGER(a,b)   ((a<b)?b:a)
#define SMALLER(a,b)   ((a>b)?b:a)


uint16_t check_sum(const uint8_t * buffer, uint16_t length);
uint32_t addchecksum(const uint8_t* buffer, uint32_t length);
uint8_t *hex_to_string(uint8_t ch);
int32_t fix_bcd_code(int32_t code);
BOOL check_time_range(int8_t hour, int8_t min, int16_t hourStart, int16_t minStart, int16_t hourEnd, int16_t minEnd);
BOOL checkVinValidStr(int8_t * pCarVin);
void sys_clock_handle(structSysClockCnt *sSysClockCnt,unionSysClockFlag *unSysClockFlag);
int8_t cnt_change_until_threshold(int8_t flag,int32_t *cnt,int32_t threshold,int32_t add,int32_t sub);
BOOL cnt_delay(int32_t *cnt,int32_t threshold);
BOOL cnt_cycle(int32_t *cnt,int32_t high);
int32_t _IQsat(int32_t i32Value, int32_t i32ValueMax,int32_t i32ValueMin);
void vIoFilterDeal(uint16_t uIoInFlag,int32_t* iFilterCnt ,uint16_t uTimeNum,uint16_t uTimePosNum,uint16_t uTimeNegNum,uint32_t *uReturnRe,uint16_t uBitNum);
char *get_string_form_string(int index,int len,int max,char *buffer);
void add_data_to_string(DATA_TYPE type,int index,int max,char *buffer,int sc);
int get_data_form_string(DATA_TYPE type,int index,int max,char *buffer);
void bcd_time_to_timestring(char *bcdTime,char *stringTime);
void hexstring_to_assicstring(uint8_t* src,uint8_t* dec,int32_t len);

#endif  //__CFUN_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/


