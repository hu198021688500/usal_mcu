//#include "drv_usb_master.h"
//#include "usbh_core.h"
//#include "usbh_msc_core.h"
//#include "usb_hcd_int.h"
//#include "usbh_core.h"
//#include "drv_port.h"
//#include "drv_int.h"
//#include "drv_irq_cfg.h"

//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"

//TDrvUsbMaster g_drv_usb_master;

//USB_OTG_CORE_HANDLE      USB_OTG_Core;
//USBH_HOST                USB_Host;

//void OTG_FS_IRQHandler(void)
//{
//    USBH_OTG_ISR_Handler(&USB_OTG_Core);
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

//void USBH_USR_Init(void)
//{
//}
//void USBH_USR_DeInit(void)
//{
//}
//void USBH_USR_DeviceAttached(void)
//{
//}
//void USBH_USR_ResetDevice(void)
//{
//}
//void USBH_USR_DeviceDisconnected(void)
//{
//    g_drv_usb_master.is_connected_ = False;
//    if (g_drv_usb_master.MethodIsConnected != NULL)
//    {
//        g_drv_usb_master.MethodIsConnected(g_drv_usb_master.recevier, False);
//    }
//}

//void USBH_USR_OverCurrentDetected(void)
//{
//}
//void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
//{
//}
//void USBH_USR_Device_DescAvailable(void *DeviceDesc)
//{ 
//    
//}
//void USBH_USR_DeviceAddressAssigned(void)
//{
//}
//void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
//                                          USBH_InterfaceDesc_TypeDef *itfDesc,
//                                          USBH_EpDesc_TypeDef *epDesc)
//{
//}
//void USBH_USR_Manufacturer_String(void *ManufacturerString)
//{
//}
//void USBH_USR_Product_String(void *ProductString)
//{
//}
//void USBH_USR_SerialNum_String(void *SerialNumString)
//{
//}
//void USBH_USR_EnumerationDone(void)
//{
//}
//USBH_USR_Status USBH_USR_UserInput(void)
//{
//    /// 用户运行的时候通知连接
//    if (!g_drv_usb_master.is_connected_)
//    {
//        g_drv_usb_master.is_connected_ = True;
//        if (g_drv_usb_master.MethodIsConnected != NULL)
//        {
//            g_drv_usb_master.MethodIsConnected(g_drv_usb_master.recevier, True);
//        }
//    }
//  return USBH_USR_RESP_OK;
//}  
//int USBH_USR_MSC_Application(void)
//{
//    return 0;
//}

//void USBH_USR_DeviceNotSupported(void)
//{
//}
//void USBH_USR_UnrecoveredError(void)
//{
//}

//USBH_Usr_cb_TypeDef USR_cb =
//{
//  USBH_USR_Init,
//  USBH_USR_DeInit,
//  USBH_USR_DeviceAttached,
//  USBH_USR_ResetDevice,
//  USBH_USR_DeviceDisconnected,
//  USBH_USR_OverCurrentDetected,
//  USBH_USR_DeviceSpeedDetected,
//  USBH_USR_Device_DescAvailable,
//  USBH_USR_DeviceAddressAssigned,
//  USBH_USR_Configuration_DescAvailable,
//  USBH_USR_Manufacturer_String,
//  USBH_USR_Product_String,
//  USBH_USR_SerialNum_String,
//  USBH_USR_EnumerationDone,
//  USBH_USR_UserInput,
//  USBH_USR_MSC_Application,
//  USBH_USR_DeviceNotSupported,
//  USBH_USR_UnrecoveredError
//};


///**
// * 驱动销毁
// * @param object: 对象实例
// * @return
// */
//static void TDrvUsbMasterDestroy(TOfObject *object)
//{
//    TDrvUsbMaster *self = (TDrvUsbMaster *)object;
//    self->Destroy_(object);
//}

//void TDrvUsbMasterCreate(TDrvUsbMaster *self, UInt8 usb_index)
//{
//    TOfObjectCreate((TOfObject *)self);
//    OVERRIDE_METHOD(TOfObject, Destroy, TDrvUsbMasterDestroy);
//    self->usb_index_ = usb_index;
//    self->is_connected_ = False;
//}

//Bool TDrvUsbMasterOpen(TDrvUsbMaster *self)
//{
//    USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_cb);    
//}

//void TDrvUsbMasterClose(TDrvUsbMaster *self)
//{
//    USBH_DeInit(&USB_OTG_Core, &USB_Host);
//}


//UInt32 TDrvUsbMasterRead(TDrvUsbMaster *self, UInt32 addr, UInt8 *buffer, UInt32 read_len)
//{
//    UInt32 result = 0;
//    UInt8 status;
//    UInt32 do_try = 9000;
//    if (self->is_connected_)
//    {
//        do
//        {
//            status = USBH_MSC_Read10(&USB_OTG_Core, buffer, addr, read_len);
//            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
//        }
//        while(self->is_connected_ && status == USBH_MSC_BUSY && do_try-- > 0);
//        ///< 异常时候会无休止等待
////        do
////        {
////            status = USBH_MSC_Read10(&USB_OTG_Core, buffer, addr, read_len);
////            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
////        }
////        while(self->is_connected_ && status == USBH_MSC_BUSY);
//        
//        if (status == USBH_MSC_OK)
//        {
//            result = read_len;
//        }
//    }
//    return result;
//}

//UInt32 TDrvUsbMasterWrite(TDrvUsbMaster *self, UInt32 addr, const UInt8 *data, UInt32 write_len)
//{
//    UInt32 result = 0;
//    UInt8 status;
//    UInt32 do_try = 9000;
//    if (self->is_connected_)
//    {
//        do
//        {
//            status = USBH_MSC_Write10(&USB_OTG_Core, (UInt8 *)data, addr, write_len);
//            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
//        }
//        while(self->is_connected_ && status == USBH_MSC_BUSY && do_try-- > 0);
//        ///< 异常时候会无休止等待
////        do
////        {
////            status = USBH_MSC_Write10(&USB_OTG_Core, (UInt8 *)data, addr, write_len);
////            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
////        }
////        while(self->is_connected_ && status == USBH_MSC_BUSY);
//        if (status == USBH_MSC_OK)
//        {
//            result = write_len;
//        }
//    }
//    return result;
//}

//void TDrvUsbMasterRunOnce(TDrvUsbMaster *self)
//{
//    USBH_Process(&USB_OTG_Core, &USB_Host);
//}

//UInt32 TDrvUsbMasterGetCapacity(TDrvUsbMaster *self)
//{
//    if (self->is_connected_)
//    {
//        return USBH_MSC_Param.MSCapacity;
//    }
//    else
//        return 0;
//}