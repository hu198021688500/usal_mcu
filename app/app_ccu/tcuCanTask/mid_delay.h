/*

 */

#ifndef MID_DELAY_H_
#define MID_DELAY_H_

#include "fun.h"

#define     TIME_1MS    (1)
#define     TIME_50MS   (50*TIME_1MS)
#define     TIME_100MS  (100*TIME_1MS)
#define     TIME_250MS  (250*TIME_1MS)
#define     TIME_500MS  (500*TIME_1MS)
#define     TIME_1S     (1000*TIME_1MS)



uint32_t u32_time_get(void);
uint32_t u32_time_past(uint32_t u32_start);
uint32_t u32_time_elapse(uint32_t u32_start,uint32_t u32_end);
int32_t s32_time_mdelay(uint32_t u32_ms);

#endif /* MID_DELAY_H_ */
