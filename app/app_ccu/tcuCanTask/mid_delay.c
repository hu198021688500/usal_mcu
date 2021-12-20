/******************************
    > File Name: mid_delay.c
    > Author:Zhx
    > Mail: 862145496@qq.com
    > Created Time: Sat 18 Sep 2021 02:15:35 AM PDT
 *****************************/

#include "mid_delay.h"
//#include "delay.h"
#include <string.h>


uint32_t u32_time_get(void)
{
    return 0;
}

//uint32_t uint32_time_get(void)
//{
////	return u32_delay_get_time();
//}

uint32_t u32_time_past(uint32_t u32_start)
{
//	return u32_delay_past_time(u32_start);
	uint32_t	ret=0;
	uint32_t  tick=u32_time_get();
	ret=0xFFFFFFFF- u32_start;
	ret+= 1 +tick;
	return (ret % 0xFFFFFFFF);
}

uint32_t u32_time_elapse(uint32_t u32_start,uint32_t u32_end)
{
//	return u32_delay_elapse(u32_start,u32_end);
	return 0;
}

int32_t s32_time_mdelay(uint32_t u32_ms)
{
//    return s32_delay_mdelay(u32_ms);
	return 0;
}
