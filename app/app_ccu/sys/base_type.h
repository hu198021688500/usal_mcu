/***************************************************************************************************   
Copyright @ 2021-20xx小桔科技

文件:   base_type
作者:   km
说明:   自定义的基本类型.
***************************************************************************************************/
#ifndef __BASETYPE_H__                   //防重包含.
#define __BASETYPE_H__

#include <stdint.h>
#include"dcharger_version.h"
#include <string.h>

typedef enum
{ 
    BOOL_FALSE = 0,
    BOOL_TRUE = 1,
}BOOL;

typedef enum    //数据类型.
{
    DATA_TYPE_INT = 0,
    DATA_TYPE_STRING,
    DATA_TYPE_STRING_ADDR,
    DATA_TYPE_FLOAT,
    DATA_TYPE_CHAR,
    DATA_TYPE_UCHAR,
    DATA_TYPE_INT16U,
    DATA_TYPE_INT16S,
    DATA_TYPE_BIGINT16U,//16位大端
    DATA_TYPE_BIGINT32U,//32位大端
    DATA_TYPE_ENUM
}DATA_TYPE;

typedef enum
{
    SYS_LANGUAGE_CH      = 0,
    SYS_LANGUAGE_EN,
    SYS_LANGUAGE_TOTAL
}ESysLanguage;

typedef  enum{
     SYS_OK=0,
	 SYS_FAIL=-1,
     SYS_ERR_PARAM=-2,
     SYS_NOT_INIT=-3,
     SYS_RESOURCE_BUSY=-4,
     SYS_RESOURCE_AVAILABLE=-5,
}E_ERROR_TABLE;

typedef union
{
	int32_t dword;
	struct
	{
		uint16_t lword;
		int16_t  hword;
	}half;
}UNLONG;	

#define CH_BIT(j) (1 << j)
#define CH_BIT_GET(i,j) (i & CH_BIT(j))
#define CH_BIT_SET(i,j) (i |= CH_BIT(j))
#define CH_BIT_RST(i,j) (i &= ~ CH_BIT(j))

#ifndef offset_of
//获取结构体成员的偏移量。
//#   define offset_of(type, member)  ((size_t)&((typeof(type) *)0)->member)  //keil编译器不支持typeof
#   define offset_of(type, member)  ((size_t)&((type *)0)->member)
#endif


#endif  //#ifndef __BASETYPE_H__
/***************************************************************************************************
文件结束.
***************************************************************************************************/


