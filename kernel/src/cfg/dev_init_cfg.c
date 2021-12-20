#include "dev_pmu.h"
#include "dev_wdog.h"
#include "dev_power.h"
#include "dev_switch.h"
#include "dev_adc.h"
#include "dev_can.h"
#include "dev_flash.h"
#include "dev_log.h"
#include "dev_rs485.h"
#include "dev_pwm.h"

/*
 * 设备初始化函数
 */
void DevInitCfg(void)
{
    DevWdogInit();
    DevPowerInit();
    DevSwitchInit();
    DevAdcInit();
    DevFlashInit();
    DevLogInit();
    
    DevCanInit();
    DevRs485Init();
    DevPwmInit();
}
