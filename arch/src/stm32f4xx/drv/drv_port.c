#include "drv_port.h"
//#include "stm32f413xx.h"
#include "stm32f4xx.h"

TDrvPort g_drv_port[kDrvPortNumMax];

enum
{
    kGpioPortMaxCnt         = kDrvPortNumI + 1,    ///< 当前芯片最大PORT
    kGpioPinMaxCnt          = 16,       ///< 每组pin个数
};

typedef enum TDrvPortCtlModeTag TDrvPortCtlMode;
enum TDrvPortCtlModeTag
{
    kDrvPortCtlModeInput            = 0,        ///< 输入(默认)
    kDrvPortCtlModeOut10MHz         = 1,        ///< 输出(10MHz)
    kDrvPortCtlModeOut2MHz          = 2,        ///< 输出(2MHz)
    kDrvPortCtlModeOut50MHz         = 3,        ///< 输出(50MHz)
    
    kDrvPortModeMax,
};

typedef enum TDrvPortCtlCnfTag TDrvPortCtlCnf;
enum TDrvPortCtlCnfTag
{
    kDrvPortCtlCnfAin               = 0,        ///< 模拟输入
    kDrvPortCtlCnfInFloating        = 1,        ///< 浮空输入
    kDrvPortCtlCnfIpuIpd            = 2,        ///< 上下拉输入
    
    kDrvPortCtlCnfOutPp             = 0,        ///< 推挽输出
    kDrvPortCtlCnfOutOd             = 1,        ///< 开漏输出
    kDrvPortCtlCnfAfPp              = 2,        ///< 复用推挽输出
    kDrvPortCtlCnfAfOd              = 3,        ///< 复用开漏输出
};
/// GPIO时钟使能枚举
static UInt32 g_rcc_apb2_en_[] = {RCC_AHB1ENR_GPIOAEN, RCC_AHB1ENR_GPIOBEN, RCC_AHB1ENR_GPIOCEN, RCC_AHB1ENR_GPIODEN
                                , RCC_AHB1ENR_GPIOEEN, RCC_AHB1ENR_GPIOFEN, RCC_AHB1ENR_GPIOGEN, RCC_AHB1ENR_GPIOHEN
                                , RCC_AHB1ENR_GPIOIEN};
/// GPIO寄存器地址
static GPIO_TypeDef* g_gpio_base_[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI};

/**
 * 销毁时钟
 * @param self: 对象实例指针
 * @return
 */
static void TDrvPortDestroy(TOfObject *object)
{
    TDrvPort *self = (TDrvPort *)object;
    
    UInt32 gpio_x = self - &g_drv_port[0];
    
    if (gpio_x < kDrvPortNumMax && gpio_x < kGpioPortMaxCnt)
    {
        RCC->AHB1ENR &= ~g_rcc_apb2_en_[gpio_x];
        
        self->is_create_ = False;
    }
    self->Destroy_(object);
    
}

void TDrvPortCreate(TDrvPort *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvPortDestroy);
    
    UInt32 gpio_x = self - &g_drv_port[0];

    if (gpio_x < kDrvPortNumMax && gpio_x < kGpioPortMaxCnt)
    {
        self->gpio_type_ = g_gpio_base_[gpio_x];
        
        RCC->AHB1ENR |= g_rcc_apb2_en_[gpio_x];
        self->is_create_ = True;
    }
}

Bool TDrvPortPinOpen(TDrvPort *self, UInt8 pin, Bool is_out, TDrvPortPinConfig *config)
{
    Bool result = False;
    if (self->is_create_ && pin < kGpioPinMaxCnt)
    {
        uint8_t mode = 0;
        uint8_t cnf = 0;
        uint32_t reg_temp;
        GPIO_TypeDef *gpio_x = (GPIO_TypeDef *)(self->gpio_type_);
        if (is_out)
        {
            reg_temp = gpio_x->MODER;
            reg_temp &= ~(0x03 << (pin * 2));
            if (config != NULL && config->mux != kPortMuxDisabled)
            {
                reg_temp |= 0x02 << (pin * 2);
            }
            /// 通用输出
            else
                reg_temp |= 0x01 << (pin * 2);
            gpio_x->MODER = reg_temp;
            /// 速度配置
            reg_temp = gpio_x->OSPEEDR;
            reg_temp &= ~(0x03 << (pin * 2));
            /// 设置50MHZ
            reg_temp |= 0x02 << (pin * 2);
            gpio_x->OSPEEDR = reg_temp;
            /// 输出配置
            reg_temp = gpio_x->OTYPER;
            reg_temp &= ~(1 << pin);
            if (config != NULL && config->strength == kPortStrengthHigh)
            {
                reg_temp |= (1 << pin);
            }
            gpio_x->OTYPER = reg_temp;
        }
        else
        {
            reg_temp = gpio_x->MODER;
            reg_temp &= ~(0x03 << (pin * 2));
            if (config != NULL)
            {
                if (config->mux == kPortMuxAltAdc)
                    reg_temp |= 0x03 << (pin * 2);
                else if (config->mux != kPortMuxDisabled)
                    reg_temp |= 0x02 << (pin * 2);
            }
            gpio_x->MODER = reg_temp;

        }
        /// 上下拉寄存器
        reg_temp = gpio_x->PUPDR;
        reg_temp &= ~(0x03 << (pin * 2));
        if (config != NULL)
        {
            if (config->pull == kPortPullUp)
            {
                reg_temp |= 0x01 << (pin * 2);
            }
            else if (config->pull == kPortPullDown)
            {
                reg_temp |= 0x02 << (pin * 2);
            }
        }
        gpio_x->PUPDR = reg_temp;
        /// 引脚复用
        if (pin < 8)
        {
            reg_temp = gpio_x->AFR[0];
            reg_temp &= ~(0x0F << (pin * 4));
            if (config != NULL && config->mux != kPortMuxDisabled && config->mux < kPortMuxAltAdc)
            {
                reg_temp |= (config->mux << (pin * 4));
            }
            gpio_x->AFR[0] = reg_temp;
        }
        else
        {
            reg_temp = gpio_x->AFR[1];
            reg_temp &= ~(0x0F << ((pin - 8) * 4));
            if (config != NULL && config->mux != kPortMuxDisabled && config->mux < kPortMuxAltAdc)
            {
                reg_temp |= (config->mux << ((pin - 8) * 4));
            }
            gpio_x->AFR[1] = reg_temp;
        }
        result = True;
    }

//    GPIO_InitTypeDef GPIO_Initure;
//    __HAL_RCC_GPIOC_CLK_ENABLE();           //开启GPIOB时钟
//    	
//        
//    GPIO_TypeDef *gpio_x = (GPIO_TypeDef *)(self->gpio_type_);
//    
//    GPIO_Initure.Pin=1 << pin; //PB1,0
//    if (is_out)
//    {
//        GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
//        GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
//        GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
//    }
//    else
//    {
//        GPIO_Initure.Mode = GPIO_MODE_INPUT;
//    }
//    HAL_GPIO_Init(gpio_x,&GPIO_Initure);
//    
    return result;
}

Bool TDrvPortPinClose(TDrvPort *self, UInt8 pin)
{
    Bool result = False;
    if (self->is_create_ && pin < kGpioPinMaxCnt)
    {
        GPIO_TypeDef *gpio_x = (GPIO_TypeDef *)(self->gpio_type_);
        UInt32 reg_temp;

        reg_temp = gpio_x->MODER;
        reg_temp &= ~(0x03 << (pin * 2));
        gpio_x->MODER = reg_temp;

        /// 引脚复用
        if (pin < 8)
        {
            reg_temp = gpio_x->AFR[0];
            reg_temp &= ~(0x0F << (pin * 4));
            gpio_x->AFR[0] = reg_temp;
        }
        else
        {
            reg_temp = gpio_x->AFR[1];
            reg_temp &= ~(0x0F << ((pin - 8) * 4));
            gpio_x->AFR[1] = reg_temp;
        }
        result = True;
    }
    return result;
}


Bool TDrvPortPinSet(TDrvPort *self, UInt8 pin, Bool status)
{
    Bool result = False;
    if (self->is_create_ && pin < kGpioPinMaxCnt)
    {
        GPIO_TypeDef *gpio_x = (GPIO_TypeDef *)(self->gpio_type_);
        
        if (status)
        {
            gpio_x->ODR |= (1 << pin);
        }
        else
        {
            gpio_x->ODR &= ~(1 << pin);
        }
        //HAL_GPIO_WritePin(gpio_x, 1 << pin, status);
        result = True;;
    }
    return result;
}


Bool TDrvPortPinGet(TDrvPort *self, UInt8 pin, Bool *status)
{
    Bool result = False;
    if (self->is_create_ && pin < kGpioPinMaxCnt && status != NULL)
    {
        GPIO_TypeDef *gpio_x = (GPIO_TypeDef *)(self->gpio_type_);
        if (gpio_x->IDR & (1 << pin))
            *status = True;
        else
            *status = False;
        result = True;
    }
    return result;
}





