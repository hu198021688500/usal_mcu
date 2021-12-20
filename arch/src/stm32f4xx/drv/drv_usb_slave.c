//#include "drv_usb_slave.h"
//#include "usbd_msc_core.h"
//#include "usbd_usr.h"
//#include "usbd_desc.h"
//#include "usb_conf.h"
//#include "usb_dcd_int.h"

//#include "drv_port.h"
//#include "drv_int.h"
//#include "drv_irq_cfg.h"

//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"

//TDrvUsbSlave g_drv_usb_master;

//USB_OTG_CORE_HANDLE     USB_OTG_dev;

//void OTG_FS_IRQHandler(void)
//{
//    USBD_OTG_ISR_Handler (&USB_OTG_dev);
//}

/////------------------驱动实现--------------///
//void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
//{
//    TDrvPortPinConfig pin_config;
//    pin_config.mux = kPortMuxAlt10;
//    pin_config.pull = kPortPullDisabled;
//    pin_config.strength = kPortStrengthLow;
//    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 11, True, &pin_config);
//    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 12, True, &pin_config);
//    
//    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 9, False, &pin_config);
//    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 10, False, &pin_config);

//    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
//    RCC->AHB2ENR |= RCC_AHB2LPENR_OTGFSLPEN;
//}

//void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
//{
//    SysEnableIRQ((IRQn_Type)OTG_FS_IRQn, kDrvIrqCfgPreeUsb, kDrvIrqCfgSubUsb);
//}

//void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
//{
//}
//void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
//{
//    
//}

//void USB_OTG_BSP_uDelay (const uint32_t usec)
//{
//    UInt32 i, j;
//    for (i = 0; i < usec * 9; i++);
//}

//void USB_OTG_BSP_mDelay (uint32_t msec)
//{ 
//    UInt32 i, j;
//    while(msec > 0)
//    {
//        USB_OTG_BSP_uDelay(1000);
//        msec--;
//    }
//}
/////------------------------------------------------------------//
//void USBD_USR_Init(void)
//{   
//}
//void USBD_USR_DeviceReset (uint8_t speed)
//{
//}
//void USBD_USR_DeviceConfigured (void)
//{
//}
//void USBD_USR_DeviceSuspended (void)
//{
//}
//void USBD_USR_DeviceResumed (void)
//{
//}
//void USBD_USR_DeviceConnected (void)
//{
//    if (!g_drv_usb_master.is_connected_)
//    {
//        g_drv_usb_master.is_connected_ = True;
//        if (g_drv_usb_master.MethodIsConnected != NULL)
//        {
//            g_drv_usb_master.MethodIsConnected(g_drv_usb_master.recevier, True);
//        }
//    }
//}
//void USBD_USR_DeviceDisconnected (void)
//{
//    g_drv_usb_master.is_connected_ = False;
//    if (g_drv_usb_master.MethodIsConnected != NULL)
//    {
//        g_drv_usb_master.MethodIsConnected(g_drv_usb_master.recevier, False);
//    }
//}
//USBD_Usr_cb_TypeDef USR_cb =
//{
//  USBD_USR_Init,
//  USBD_USR_DeviceReset,
//  USBD_USR_DeviceConfigured,
//  USBD_USR_DeviceSuspended,
//  USBD_USR_DeviceResumed,
//  
//  USBD_USR_DeviceConnected,
//  USBD_USR_DeviceDisconnected,  
//};
///**
// * 驱动销毁
// * @param object: 对象实例
// * @return
// */
//static void TDrvUsbSlaveDestroy(TOfObject *object)
//{
//    TDrvUsbSlave *self = (TDrvUsbSlave *)object;
//    self->Destroy_(object);
//}

//void TDrvUsbSlaveCreate(TDrvUsbSlave *self, UInt8 usb_index)
//{
//    TOfObjectCreate((TOfObject *)self);
//    OVERRIDE_METHOD(TOfObject, Destroy, TDrvUsbSlaveDestroy);
//    self->usb_index_ = usb_index;
//    self->is_connected_ = False;
//}

//Bool TDrvUsbSlaveOpen(TDrvUsbSlave *self)
//{
//    USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USR_cb);    
//}

//void TDrvUsbSlaveClose(TDrvUsbSlave *self)
//{
//    USBD_DeInit(&USB_OTG_dev);
//}
