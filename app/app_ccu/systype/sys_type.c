/***************************************************************************************************   
Copyright @ 2020-20xx小桔科技

文件:   pro_base
作者:   km
说明:   基本机型.
***************************************************************************************************/

#include <string.h>

#include "sys_type.h"
#include "sys_config.h"
#include "log.h"
#include "sys_type_contactor_group.h"

/**
  * @brief  开机前留的时间，用来切换模块、接触器
  */
int32_t Project_Type_Delay(void)
{
    int32_t delay = 0;
    
    switch(sSysConfig.sInt.uPorjectType)
    {
        case SYSTYPE_FASTSLOW:
            delay = DELAY_PROJECT_FASTSLOW;
            break;
		case SYSTYPE_CONTACTOR_GROUP:
			
		    delay = DELAY_PROJECT_FASTSLOW;
            break;
        default:
            break;

    
    }
    return delay;
}

void Project_Type_Deal(void)
{
    switch(sSysConfig.sInt.uPorjectType)
    {
        case SYSTYPE_FASTSLOW:
            break;

        case SYSTYPE_CONTACTOR_GROUP:
			//vGh2_contactor_group();
            break;

        default:
            break;

    
    }
}

/***************************************************************************************************
文件结束.
***************************************************************************************************/
