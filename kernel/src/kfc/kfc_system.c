#include "kfc_system.h"
#include "drv_clock.h"
#include "rtos_api.h"

void KfcSleep(UInt32 ms)
{
    RtosSleep(ms);
}

UInt32 KfcGetTickCount(void)
{
    return RtosGetTickCount();
}

void KfcDelayUs(UInt32 us)
{
    TDrvClockDelayUs(&g_drv_clock, us);
}

void KfcDelayMs(UInt32 ms)
{
    TDrvClockDelayMs(&g_drv_clock, ms);
}