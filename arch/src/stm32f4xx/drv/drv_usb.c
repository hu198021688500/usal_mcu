#include "drv_usb.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"
#include "usb_hcd_int.h"

#include "usbd_core.h"
#include "usb_dcd_int.h"
#include "usbd_desc.h"
#include "usbd_msc_core.h"
#include "usbd_msc_mem.h"

#include "drv_port.h"
#include "drv_int.h"
#include "drv_irq_cfg.h"

#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


TDrvUsb g_drv_usb;

static USB_OTG_CORE_HANDLE      USB_OTG_Core;
static USBH_HOST                USB_Host;

static USB_OTG_CORE_HANDLE     USB_OTG_dev;

#define STORAGE_LUN_NBR                  1

/// 中断函数
void OTG_FS_IRQHandler(void)
{
    if (g_drv_usb.is_host_)
        USBH_OTG_ISR_Handler(&USB_OTG_Core);
    else
        USBD_OTG_ISR_Handler(&USB_OTG_dev);
}

///------------------驱动实现--------------///
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
    TDrvPortPinConfig pin_config;
    pin_config.mux = kPortMuxAlt10;
    pin_config.pull = kPortPullDisabled;
    pin_config.strength = kPortStrengthLow;
    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 11, True, &pin_config);
    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 12, True, &pin_config);
    
    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 9, False, &pin_config);
    TDrvPortPinOpen(&g_drv_port[kDrvPortNumA], 10, False, &pin_config);

    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    RCC->AHB2ENR |= RCC_AHB2LPENR_OTGFSLPEN;
}

void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
    SysEnableIRQ((IRQn_Type)OTG_FS_IRQn, kDrvIrqCfgPreeUsb, kDrvIrqCfgSubUsb);
}

void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{
}
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{
    
}

void USB_OTG_BSP_uDelay (const uint32_t usec)
{
    UInt32 i;
    for (i = 0; i < usec * 9; i++);
}

void USB_OTG_BSP_mDelay (uint32_t msec)
{
    while(msec > 0)
    {
        USB_OTG_BSP_uDelay(1000);
        msec--;
    }
}
///----------------------主机接口--------------------------------------//
static void USBH_USR_Init(void)
{
}
static void USBH_USR_DeInit(void)
{
}
static void USBH_USR_DeviceAttached(void)
{
}
static void USBH_USR_ResetDevice(void)
{
}
static void USBH_USR_DeviceDisconnected(void)
{
    g_drv_usb.is_connected_ = False;
    if (g_drv_usb.MethodIsConnected != NULL)
    {
        g_drv_usb.MethodIsConnected(g_drv_usb.recevier, False);
    }
}
static void USBH_USR_OverCurrentDetected(void)
{
}
static void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
}
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
    
}
static void USBH_USR_DeviceAddressAssigned(void)
{
}
static void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
}
static void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
}
static void USBH_USR_Product_String(void *ProductString)
{
}
static void USBH_USR_SerialNum_String(void *SerialNumString)
{
}
static void USBH_USR_EnumerationDone(void)
{
}
static USBH_USR_Status USBH_USR_UserInput(void)
{
    /// 用户运行的时候通知连接
    if (!g_drv_usb.is_connected_)
    {
        g_drv_usb.is_connected_ = True;
        if (g_drv_usb.MethodIsConnected != NULL)
        {
            g_drv_usb.MethodIsConnected(g_drv_usb.recevier, True);
        }
    }
  return USBH_USR_RESP_OK;
}  
int USBH_USR_MSC_Application(void)
{
    return 0;
}

void USBH_USR_DeviceNotSupported(void)
{
}
void USBH_USR_UnrecoveredError(void)
{
}

USBH_Usr_cb_TypeDef USRH_cb =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};


///-----------------------从机接口-------------------------------------//
void USBD_USR_Init(void)
{   
}
void USBD_USR_DeviceReset (uint8_t speed)
{
}
void USBD_USR_DeviceConfigured (void)
{
}
void USBD_USR_DeviceSuspended (void)
{
}
void USBD_USR_DeviceResumed (void)
{
}
void USBD_USR_DeviceConnected (void)
{
    if (!g_drv_usb.is_connected_)
    {
        g_drv_usb.is_connected_ = True;
        if (g_drv_usb.MethodIsConnected != NULL)
        {
            g_drv_usb.MethodIsConnected(g_drv_usb.recevier, True);
        }
    }
}
void USBD_USR_DeviceDisconnected (void)
{
    g_drv_usb.is_connected_ = False;
    if (g_drv_usb.MethodIsConnected != NULL)
    {
        g_drv_usb.MethodIsConnected(g_drv_usb.recevier, False);
    }
}

USBD_Usr_cb_TypeDef USRD_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,  
};

int8_t STORAGE_Init (uint8_t lun)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->Init(lun);
    }
    else
        return -1;
//  if( SD_Init() != 0)
//  {
//    return (-1); 
//  } 
//  return (0);
}
int8_t STORAGE_GetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->GetCapacity(lun, block_num, block_size);
    }
    else
        return -1;
//  if(SD_GetStatus() != 0 )
//  {
//    return (-1); 
//  }
//  
//  *block_size =  512;  
//  *block_num =  SDCardInfo.CardCapacity / 512;  
//  
//  return (0); 
}
int8_t  STORAGE_IsReady (uint8_t lun)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->IsReady(lun);
    }
    else
        return -1;
//  if( SD_Init() != 0)
//  {
//    return (-1);
//  }
//  return (0);
}
int8_t  STORAGE_IsWriteProtected (uint8_t lun)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->IsWriteProtected(lun);
    }
    else
        return -1;
//  return  0;
}
int8_t STORAGE_Read (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->Read(lun, buf, blk_addr, blk_len);
    }
    else
        return -1;
//  if( SD_ReadMultiBlocks (buf, 
//                          blk_addr * 512, 
//                          512,
//                          blk_len) != 0)
//  {
//    return -1;
//  } 
//  return 0;
}
int8_t STORAGE_Write (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->Write(lun, buf, blk_addr, blk_len);
    }
    else
        return -1;
//  if( SD_WriteMultiBlocks (buf, 
//                           blk_addr * 512, 
//                           512,
//                           blk_len) != 0)
//  {
//    return -1;
//  }
//  return (0);
}
int8_t STORAGE_GetMaxLun (void)
{
    if (!g_drv_usb.is_host_ && g_drv_usb.MethodDeviceStorage != NULL)
    {
        return g_drv_usb.MethodDeviceStorage->GetMaxLun();
    }
    else
        return -1;
//  return (STORAGE_LUN_NBR - 1);
}
/** @defgroup STORAGE_Private_Variables
  * @{
  */ 
/* USB Mass storage Standard Inquiry Data */
//const int8_t  STORAGE_Inquirydata[] = {//36
//  
//  /* LUN 0 */
//  0x00,		
//  0x80,		
//  0x02,		
//  0x02,
//  (USBD_STD_INQUIRY_LENGTH - 5),
//  0x00,
//  0x00,	
//  0x00,
//  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
//  'm', 'i', 'c', 'r', 'o', 'S', 'D', ' ', /* Product      : 16 Bytes */
//  'F', 'l', 'a', 's', 'h', ' ', ' ', ' ',
//  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
//}; 

USBD_STORAGE_cb_TypeDef USBD_MICRO_SDIO_fops =
{
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  NULL,//  (int8_t *)STORAGE_Inquirydata,
};

USBD_STORAGE_cb_TypeDef  *USBD_STORAGE_fops = &USBD_MICRO_SDIO_fops;

/**
 * 驱动销毁
 * @param object: 对象实例
 * @return
 */
static void TDrvUsbDestroy(TOfObject *object)
{
    TDrvUsb *self = (TDrvUsb *)object;
    self->Destroy_(object);
}

void TDrvUsbCreate(TDrvUsb *self, UInt8 usb_index)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TDrvUsbDestroy);
    self->usb_index_ = usb_index;
    self->is_connected_ = False;
    self->is_host_ = False;
}


Bool TDrvUsbOpen(TDrvUsb *self, Bool is_host, TDrvUsbDeviceStorageMethod *method_device_storage)
{
    if (is_host)
    {
        self->is_host_ = True;
        USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USRH_cb);
    }
    else
    {
        self->MethodDeviceStorage = method_device_storage;
        USBD_MICRO_SDIO_fops.pInquiry = method_device_storage->pInquiry;
        
        self->is_host_ = False;
        USBD_Init(&USB_OTG_dev, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_MSC_cb, &USRD_cb);
    }
    
    return True;
}

void TDrvUsbClose(TDrvUsb *self)
{
    if (self->is_host_)
        USBH_DeInit(&USB_OTG_Core, &USB_Host);
    else
        USBD_DeInit(&USB_OTG_Core);
}

UInt32 TDrvUsbMasterRead(TDrvUsb *self, UInt32 addr, UInt8 *buffer, UInt32 read_len)
{
    UInt32 result = 0;
    UInt8 status;
    UInt32 do_try = 9000;
    if (self->is_connected_)
    {
        do
        {
            status = USBH_MSC_Read10(&USB_OTG_Core, buffer, addr, read_len);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
        }
        while(self->is_connected_ && status == USBH_MSC_BUSY && do_try-- > 0);
        ///< 异常时候会无休止等待
//        do
//        {
//            status = USBH_MSC_Read10(&USB_OTG_Core, buffer, addr, read_len);
//            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
//        }
//        while(self->is_connected_ && status == USBH_MSC_BUSY);
        
        if (status == USBH_MSC_OK)
        {
            result = read_len;
        }
    }
    return result;
}

UInt32 TDrvUsbMasterWrite(TDrvUsb *self, UInt32 addr, const UInt8 *data, UInt32 write_len)
{
    UInt32 result = 0;
    UInt8 status;
    UInt32 do_try = 9000;
    if (self->is_connected_)
    {
        do
        {
            status = USBH_MSC_Write10(&USB_OTG_Core, (UInt8 *)data, addr, write_len);
            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
        }
        while(self->is_connected_ && status == USBH_MSC_BUSY && do_try-- > 0);
        ///< 异常时候会无休止等待
//        do
//        {
//            status = USBH_MSC_Write10(&USB_OTG_Core, (UInt8 *)data, addr, write_len);
//            USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);
//        }
//        while(self->is_connected_ && status == USBH_MSC_BUSY);
        if (status == USBH_MSC_OK)
        {
            result = write_len;
        }
    }
    return result;
}

void TDrvUsbMasterRunOnce(TDrvUsb *self)
{
    if (self->is_host_)
        USBH_Process(&USB_OTG_Core, &USB_Host);
}

UInt32 TDrvUsbMasterGetCapacity(TDrvUsb *self)
{
    if (self->is_connected_)
    {
        return USBH_MSC_Param.MSCapacity;
    }
    else
        return 0;
}
