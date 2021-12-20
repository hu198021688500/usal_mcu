// 定时服务器使用FTM2

#include "drv_pwm.h"

#include "drv_clock.h"
#include "drv_int.h"
#include <pfc_sync_objs.h>
#include "drv_pwm_cfg.h"
#include "stm32f4xx_tim.h"

#include "drv_pwm_cfg.c"

static TIM_TypeDef* g_pwm_base_[] = {TIM1, TIM2, TIM3, TIM4, TIM5, TIM6, TIM7, TIM8};

static void TDrvPwmClockOpen(TDrvPwm *self, Byte index)
{
    switch(index)
    {
        case 0: RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;break;
        case 1: RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;break;
        case 2: RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;break;
        case 3: RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;break;
        case 4: RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;break;
        case 5: RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;break;
        case 6: RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;break;
        case 7: RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;break;
        case 8: RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;break;
        case 9: RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;break;
        case 10: RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;break;
        case 11: RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;break;
        case 12: RCC->APB1ENR |= RCC_APB1ENR_TIM12EN;break;
        case 13: RCC->APB1ENR |= RCC_APB1ENR_TIM13EN;break;
        case 14: RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;break;
        default:
            break;
    }
}
static UInt32 TDrvPwmClockGet(TDrvPwm *self, Byte index)
{
    UInt32 result = 0;
    TDrvClockFrequency clock_freq;
    TDrvClockGetFrequency(&g_drv_clock, &clock_freq);
    
    if (index == 0 || (index >= 7 && index <= 11))
        result = clock_freq.pclk2_frequency;
    else
        result = clock_freq.pclk1_frequency;
    return result;
}
/**
 * 定时器打开
 * @param self: 对象实例指针
 * @return 
 */
static void TDrvPwmTimOpen(TDrvPwm *self, UInt8 pwm_tim_index, UInt32 pwm_freq)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TDrvPwmTimerCfg *time_cfg = &g_drv_pwm_timer_cfgs[self->pwm_timer_index_];
    
    TIM_TypeDef *base = g_pwm_base_[time_cfg->tim_index];
    
    UInt32 pwm_clock, prescaler, period;
    /// 打开时钟
    TDrvPwmClockOpen(self, time_cfg->tim_index);
    
    pwm_clock = TDrvPwmClockGet(self, time_cfg->tim_index);
    
    pwm_clock *= 2; ///< 实际频率为2倍，原因不明
    /// 计算自动重载值1000左右应该的分频值
    prescaler = pwm_clock / (pwm_freq * 1000);
    /// 根据分频值再计算具体自动重载值
    period = pwm_clock / (pwm_freq * prescaler);
    /// 保存自动重载值
    time_cfg->pwm_period = period;
    
    TIM_TimeBaseStructure.TIM_Period = period - 1;                   ///< 设置自动重载计数=999+1=1000
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;             ///< 定时器分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 1;                     ///< 输入捕获时滤波用的参数,必须设一下
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      ///< 向上计数
    TIM_TimeBaseInit(base, &TIM_TimeBaseStructure);                  ///< 根据指定的参数初始

    TIM_CtrlPWMOutputs(base,ENABLE);
    TIM_Cmd(base, ENABLE);
}
/**
 * 销毁Dma通道
 * @param self: 对象实例指针
 * @return 
 */
static void TDrvPwmDestroy(TOfObject *self)
{
    TDrvPwm *this = (TDrvPwm *)self;
    this->Destroy_(self);
}

Bool TDrvPwmCreate(TDrvPwm *self, UInt8 index)
{
    Bool result = False;
    // 创建父类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvPwmDestroy);

    if (index < sizeof(g_drv_pwm_cfgs) / sizeof(g_drv_pwm_cfgs[0]))
    {
        self->cfg_index_ = index;
        self->pwm_timer_index_ = g_drv_pwm_cfgs[index].pwm_tim_index_;
        
        if (self->pwm_timer_index_ < sizeof(g_drv_pwm_timer_cfgs) / sizeof(g_drv_pwm_timer_cfgs[0]))
        {
            result = True;
        }
    }
    return result;
}

Bool TDrvPwmOpen(TDrvPwm *self)
{
    TDrvPortPinConfig pin_config;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    if (self->pwm_timer_index_ < sizeof(g_drv_pwm_timer_cfgs) / sizeof(g_drv_pwm_timer_cfgs[0]))
    {
        const TDrvPwmCfg *config = &g_drv_pwm_cfgs[self->cfg_index_];
        TDrvPwmTimerCfg *time_cfg = &g_drv_pwm_timer_cfgs[self->pwm_timer_index_];
        
        TIM_TypeDef *base = g_pwm_base_[time_cfg->tim_index];
        
        if (time_cfg->time_is_open == False)
        {
            TDrvPwmTimOpen(self, self->pwm_timer_index_, time_cfg->pwm_frequence);
            time_cfg->time_is_open = True;
        }
        
        /// 设置GPIO设置
        pin_config.mux = config->gpio_mux;
        pin_config.pull = kPortPullDisabled;
        pin_config.strength = kPortStrengthLow;
        TDrvPortPinOpen(config->pwm_port, config->pwm_pin, True, &pin_config);
        
        /// 设置定时器
        TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
        TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
        TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
        TIM_OCInitStructure.TIM_Pulse=0;
        if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 0)
        {
            TIM_OC1Init(base, &TIM_OCInitStructure);
            TIM_OC1PreloadConfig(base, TIM_OCPreload_Enable);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 1)
        {
            TIM_OC2Init(base, &TIM_OCInitStructure);
            TIM_OC2PreloadConfig(base, TIM_OCPreload_Enable);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 2)
        {
            TIM_OC3Init(base, &TIM_OCInitStructure);
            TIM_OC3PreloadConfig(base, TIM_OCPreload_Enable);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 3)
        {
            TIM_OC4Init(base, &TIM_OCInitStructure);
            TIM_OC4PreloadConfig(base, TIM_OCPreload_Enable);
        }        
    }
    return True;
}

Bool TDrvPwmClose(TDrvPwm *self)
{
    TDrvPwmTimerCfg *time_cfg = &g_drv_pwm_timer_cfgs[self->pwm_timer_index_];
    
    TIM_TypeDef *base = g_pwm_base_[time_cfg->tim_index];

    if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 0)
    {
        TIM_OC1PreloadConfig(base, TIM_OCPreload_Disable);
    }
    else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 1)
    {
        TIM_OC2PreloadConfig(base, TIM_OCPreload_Disable);
    }
    else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 2)
    {
        TIM_OC3PreloadConfig(base, TIM_OCPreload_Disable);
    }
    else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 3)
    {
        TIM_OC4PreloadConfig(base, TIM_OCPreload_Disable);
    }

    return True;
}

Bool TDrvPwmSetVaule(TDrvPwm *self, UInt32 value)
{
    UInt32 set_value;
    if (value <= 100)
    {
        TDrvPwmTimerCfg *time_cfg = &g_drv_pwm_timer_cfgs[self->pwm_timer_index_];
        
        TIM_TypeDef *base = g_pwm_base_[time_cfg->tim_index];
        
        set_value = (time_cfg->pwm_period / 100) * value;
        if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 0)
        {
            TIM_SetCompare1(base, set_value);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 1)
        {
            TIM_SetCompare2(base, set_value);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 2)
        {
            TIM_SetCompare3(base, set_value);
        }
        else if (g_drv_pwm_cfgs[self->cfg_index_].tim_chn_ == 3)
        {
            TIM_SetCompare4(base, set_value);
        }
    }
    return True;
}


