/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "per_ext_flash.h"
#include "drv_usb.h"

#define USBD_STD_INQUIRY_LENGTH		36

Bool g_disk_ready[kDiskTypeMax] = {1, 0};

static uint32_t g_flash_sector_size = 512;          ///< 扇区大小
static uint32_t g_flash_sector_count = 2048 * 6;    ///< 块扇区个数
static uint32_t g_flash_block_count = 8;            ///< 块大小

Int8 DeviceStoreInit(UInt8 lun)
{
    if (g_disk_ready[kDiskTypeExtFlash])
        return 0;
    else 
        return -1;
}
Int8 DeviceStoreGetCapacity (uint8_t lun, uint32_t *block_num, uint32_t *block_size)
{
    if (g_disk_ready[kDiskTypeExtFlash])
    {
        *block_size = g_flash_sector_size;
        *block_num = g_flash_sector_count;
        return 0;
    }
    else
        return -1;
}
int8_t  DeviceStoreIsReady (uint8_t lun)
{
    if (g_disk_ready[kDiskTypeExtFlash])
        return 0;
    else 
        return -1;
}
int8_t  DeviceStoreIsWriteProtected (uint8_t lun)
{
  return  0;
}
int8_t DeviceStoreRead (uint8_t lun, 
                 uint8_t *buf, 
                 uint32_t blk_addr,                       
                 uint16_t blk_len)
{
    for( ; blk_len > 0; blk_len--)
    {
        if (TPerExtFlashRead(&g_per_ext_flash, blk_addr * g_flash_sector_size, buf, g_flash_sector_size) > 0)
        {
            blk_addr++;
            buf += g_flash_sector_size;
        }
        else
            break;
    }
    if (blk_len == 0)
        return 0;
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
int8_t DeviceStoreWrite (uint8_t lun, 
                  uint8_t *buf, 
                  uint32_t blk_addr,
                  uint16_t blk_len)
{
    for(;blk_len>0;blk_len--)
    {										    
        if (TPerExtFlashWrite(&g_per_ext_flash, blk_addr * g_flash_sector_size, buf, g_flash_sector_size) > 0)
        {
            blk_addr++;
            buf += g_flash_sector_size;
        }
        else
            break;
    }
    if (blk_len == 0)
        return 0;
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
int8_t DeviceStoreGetMaxLun (void)
{
  return 0;
}
const int8_t  DeviceStoreInquirydata[] = {//36
  
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (USBD_STD_INQUIRY_LENGTH - 5),
  0x00,
  0x00,	
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'm', 'i', 'c', 'r', 'o', 'S', 'D', ' ', /* Product      : 16 Bytes */
  'F', 'l', 'a', 's', 'h', ' ', ' ', ' ',
  '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
};

TDrvUsbDeviceStorageMethod g_usb_device_method =
{
  DeviceStoreInit,
  DeviceStoreGetCapacity,
  DeviceStoreIsReady,
  DeviceStoreIsWriteProtected,
  DeviceStoreRead,
  DeviceStoreWrite,
  DeviceStoreGetMaxLun,
  (int8_t *)DeviceStoreInquirydata,
};



/// u盘是否连接
void UsbMasterIsConnected(void *sender, Bool is_connected)
{
    g_disk_ready[kDiskTypeUsb] = is_connected;
}
/// 硬盘驱动初始化
DSTATUS disk_create(BYTE pdrv)
{
    if (pdrv == kDiskTypeExtFlash)
    {
        TPerExtFlashCreate(&g_per_ext_flash);
    }
    else if (pdrv == kDiskTypeUsb)
    {
        TDrvUsbCreate(&g_drv_usb, 0);
        g_drv_usb.MethodIsConnected = UsbMasterIsConnected;
        /// 初始化读u盘主机
        TDrvUsbOpen(&g_drv_usb, True, NULL);
        /// 初始化usb从机
        //TDrvUsbOpen(&g_drv_usb, False, &g_usb_device_method);
    }
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    if (g_disk_ready[pdrv])
        return RES_OK;
    else
        return RES_NOTRDY;
}
/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    DSTATUS result = STA_NOINIT;
	switch (pdrv) 
    {
        case kDiskTypeExtFlash :
            {
                if (TPerExtFlashOpen(&g_per_ext_flash))
                {
                    result = RES_OK;
                }
            }
            break;
        case kDiskTypeUsb:
            {
                result = RES_OK;
            }
            break;
        default:
            break;
    }
	return result;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
    DRESULT result = RES_PARERR;
	switch (pdrv)
	{
		case kDiskTypeExtFlash :
             {
				for( ; count > 0; count--)
				{
                    if (TPerExtFlashRead(&g_per_ext_flash, sector * g_flash_sector_size, buff, g_flash_sector_size) > 0)
                    {
                        sector++;
                        buff += g_flash_sector_size;
                    }
                    else
                        break;
				}
                if (count == 0)
                    result = RES_OK;
                else
                    result = RES_ERROR;
             }
             break;

		case kDiskTypeUsb :
            {
                  if (TDrvUsbMasterRead(&g_drv_usb, sector, buff, 512 * count) > 0)
                      result = RES_OK;
                  else
                      result = RES_ERROR;
            }
			 break;
        default:
             break;
	}
	return result;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
    DRESULT result = RES_PARERR;

	switch (pdrv) 
    {

	case kDiskTypeExtFlash :
         {
			for(;count>0;count--)
			{										    
                if (TPerExtFlashWrite(&g_per_ext_flash, sector * g_flash_sector_size, buff, g_flash_sector_size) > 0)
                {
                    sector++;
                    buff += g_flash_sector_size;
                }
                else
                    break;
			}
            if (count == 0)
                result = RES_OK;
            else
                result = RES_ERROR;
         }
         break;
	case kDiskTypeUsb :
            {
                if (TDrvUsbMasterWrite(&g_drv_usb, sector, buff, 512 * count) > 0)
                    result = RES_OK;
                else
                    result = RES_ERROR;
            }
        break;
    }
	return result;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT result = RES_PARERR;

    if (pdrv == kDiskTypeExtFlash)
    {
        switch(cmd)
        {
            case CTRL_SYNC:
                    result = RES_OK; 
                    break;	 
            case GET_SECTOR_SIZE:
                    *(WORD*)buff = g_flash_sector_size;
                    result = RES_OK;
                    break;	 
            case GET_BLOCK_SIZE:
                    *(WORD*)buff = g_flash_block_count;
                    result = RES_OK;
                    break;	 
            case GET_SECTOR_COUNT:
                    *(DWORD*)buff = g_flash_sector_count;
                    result = RES_OK;
                    break;
            default:
                    result = RES_PARERR;
                    break;				
        }
    }
    else if (pdrv == kDiskTypeUsb)
    {
        switch (cmd) {
          case CTRL_SYNC :		/* Make sure that no pending write process */
            
            result = RES_OK;
            break;
            
          case GET_SECTOR_COUNT :	/* Get number of sectors on the disk (DWORD) */
            
            *(DWORD*)buff = (DWORD) TDrvUsbMasterGetCapacity(&g_drv_usb);
            result = RES_OK;
            break;
            
          case GET_SECTOR_SIZE :	/* Get R/W sector size (WORD) */
            *(WORD*)buff = 512;
            result = RES_OK;
            break;
            
          case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
            
            *(DWORD*)buff = 512;
            
            break;
          default:
            result = RES_PARERR;
          }
          
    }
    return result;
}

//获取系统时间
//User defined function to give a current time to fatfs module      */
//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{				 
	return 0;
}		

void disk_run_once(BYTE pdrv)
{
    if (pdrv == kDiskTypeUsb)
    {
        TDrvUsbMasterRunOnce(&g_drv_usb);
    }
}

