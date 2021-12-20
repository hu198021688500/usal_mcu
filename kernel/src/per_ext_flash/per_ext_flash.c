#include "per_ext_flash.h"
#include "drv_spi.h"

TPerExtFlash g_per_ext_flash;

/**
 * 设备配置结构体
 */
typedef struct TPerExtFlashConfigTag TPerExtFlashConfig;
struct TPerExtFlashConfigTag
{
    UInt8       drv_spi_chn;   ///< spi驱动通道号
    TDrvPort   *cs_port;       ///< 端口枚举
    UInt8       cs_pin;        ///< 引脚
};

#include "per_ext_flash_cfg.c"

enum TExtFlashCmdTag
{
    kExtFlashCmdWriteStatus   = 0x01,     ///< 写状态寄存器
    kExtFlashCmdWritePage     = 0x02,     ///< 写扇区页命令
    kExtFlashCmdReadPage      = 0x03,     ///< 读扇区页命令
    kExtFlashCmdReadStatus    = 0x05,     ///< 读状态寄存器命令
    kExtFlashCmdWriteEnabe    = 0x06,     ///< 写使能命令
    
    kExtFlashCmdEraseSector   = 0x20,     ///< 擦除扇区命令
    kExtFlashCmdEraseChip     = 0xC7,     ///< 擦除整个芯片命令
    
    kExtFlashCmdPowerDown     = 0xB9,     ///< 芯片进入掉电模式
    kExtFlashCmdReleasePowerDown= 0xAB,   ///< 芯片退出掉电模式(唤醒)
    
    kExtFlashCmdReadChipId    = 0x9F,     ///< 读器件ID命令
    
};

enum
{
    kExtFlashTypeIdW25Q64BV           = 0xEF4017,
    kExtFlashTypeIdMx25L1606E         = 0xC22015,
    kExtFlashTypeIdSST25VF016B        = 0xBF2541,
    
};
/// 获取芯片id
static UInt32 TPerExtFlashReadChipId(TPerExtFlash *self)
{
    UInt32 result = 0;
    UInt8 id[3];
    ///使能片选
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdReadChipId);
    id[0] = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff); ///< id第一字节
    id[1] = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff); ///< id第二字节
    id[2] = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff); ///< id第三字节
    
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
    
    result = (id[0] << 16) | (id[1] << 8) | id[2];
    
    return result;
}

/// 写使能
static void TPerExtFlashWriteEnable(TPerExtFlash *self)
{
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdWriteEnabe);
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
}

static UInt8 TPerExtFlashReadStatus(TPerExtFlash *self)
{
    UInt8 result = 0;
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdReadStatus);
    result = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff);
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
    return result;
}
/// 读取状态标记等待写命令结束
static void TPerExtFlashWaitForWriteEnd(TPerExtFlash *self)
{
//    UInt32 cnt = 10;
//    while(cnt-- > 0)
//    {
//        if ((TPerExtFlashReadStatus(self) & 0x01) != 1)
//            break;
//    }
    
      while((TPerExtFlashReadStatus(self) & 0x01) == 1);
}
/**
 * 擦除扇区
 * @param object: 对象实例
 * @param addr: 地址(扇区起始地址)
 * @return
 */
static void TPerExtFlashEraseSector(TPerExtFlash *self, UInt32 sector_addr)
{
    sector_addr *= self->sector_size_;
    
    TPerExtFlashWriteEnable(self);
    TPerExtFlashWaitForWriteEnd(self);
    
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdEraseSector);
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((sector_addr) >> 16));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((sector_addr) >> 8));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)(sector_addr));
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
    
    TPerExtFlashWaitForWriteEnd(self);
}
/**
 * 芯片数据读取
 * @param object: 对象实例
 * @param addr: 地址
 * @param buffer: 缓存地址
 * @param size: 读取长度
 * @return
 */
static void TPerExtFlashReadData(TPerExtFlash *self, UInt32 addr, UInt8 *buffer, Int32 size)
{
    Int32 i;
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdReadPage);
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((addr) >> 16));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((addr) >> 8));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)(addr));
    
    UInt8 temp = 0xff;
    for (i = 0; i < size; i++)
    {
        buffer[i] = TDrvSpiWriteAndRead(&self->drv_spi_, temp);
    }
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
}

/**
 * 芯片页写入
 * @param object: 对象实例
 * @param addr: 地址
 * @param data: 数据地址
 * @param size: 数据大小(起始地址为页开始地址,最大单页最大字节, 如地址为页中间位置,最大自己为该页剩余大小)
 * @return
 */
static void TPerExtFlashWritePage(TPerExtFlash *self, UInt32 addr, const UInt8 *data, Int32 size)
{
    UInt32 i;
    TPerExtFlashWriteEnable(self);
    
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdWritePage);
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((addr) >> 16));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)((addr) >> 8));
    TDrvSpiWriteAndRead(&self->drv_spi_, (UInt8)(addr));
    for (i = 0; i < size; i++)
    {
        TDrvSpiWriteAndRead(&self->drv_spi_, data[i]);
    }
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
    
    TPerExtFlashWaitForWriteEnd(self);
}
/**
 * 芯片页自动写入(支持自动换页,但是无擦除,如果遇到未擦除非0xff区域会写入失败)
 * @param object: 对象实例
 * @param addr: 地址
 * @param data: 数据地址
 * @param size: 数据大小
 * @return
 */
static void TPerExtFlashWriteAutoPage(TPerExtFlash *self, UInt32 addr, const UInt8 *data, Int32 size)
{
    UInt32 page_remain;	   
	page_remain=self->page_size_ - (addr % self->page_size_); ///< 单页剩余的字节数
    
	if(size <= page_remain)
        page_remain = size; ///< 不大于单页大小
	while(1)
	{
		TPerExtFlashWritePage(self, addr, data, page_remain);
        
		if(size == page_remain)
            break; ///< 写入结束了
	 	else
		{
			data += page_remain;
			addr += page_remain;
			size -= page_remain;			  ///< 减去已经写入了的字节数
            
			if(size > self->page_size_)
                page_remain = self->page_size_;  ///< 一次可以写入256个字节
			else 
                page_remain = size; 	      ///< 不够256个字节了
		}
	};
}
/**
 * 芯片数据自动写入(支持自动擦除和写入,地址和大小只要不超过芯片最大限值就好)
 * @param object: 对象实例
 * @param addr: 地址
 * @param data: 数据地址
 * @param size: 数据大小
 * @return
 */
static void TPerExtFlashWriteAuto(TPerExtFlash *self, Int32 addr, const UInt8 *data, Int32 size)
{
    UInt32 secpos;
	UInt16 secoff;
	UInt16 secremain;	   
 	UInt16 i;    
	UInt8 * W25QXX_BUF;	  
   	W25QXX_BUF = self->change_buffer_;
    
 	secpos = addr / self->sector_size_;//扇区地址  
	secoff = addr % self->sector_size_;//在扇区内的偏移
	secremain = self->sector_size_ - secoff;//扇区剩余空间大小
 	if(size <= secremain)
        secremain = size;//不大于4096个字节
	while(1) 
	{	
		TPerExtFlashReadData(self, secpos * self->sector_size_, W25QXX_BUF, self->sector_size_);//读出整个扇区的内容
		/// 判断是否需要重新写入
        for(i = 0; i < secremain; i++)
		{
			if(W25QXX_BUF[secoff + i] != data[i])
                break;//需要擦除
		}
        /// 需要重新写入
        if(i < secremain)
        {
            /// 判断是否需要擦除
            for(i = 0; i < secremain; i++)
            {
                if(W25QXX_BUF[secoff + i] != 0XFF)
                    break;//需要擦除  	  
            }
            if(i < secremain)//需要擦除
            {
                TPerExtFlashEraseSector(self, secpos);//擦除这个扇区
                for(i = 0; i < secremain; i++)	   //复制
                {
                    W25QXX_BUF[i + secoff] = data[i];	  
                }
                TPerExtFlashWriteAutoPage(self, secpos * self->sector_size_, W25QXX_BUF, self->sector_size_);//写入整个扇区  
            }
            else
                TPerExtFlashWriteAutoPage(self, addr, data, secremain);//写已经擦除了的,直接写入扇区剩余区间. 
        }
        				   
		if(size == secremain)
            break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	data += secremain;  //指针偏移
			addr += secremain;//写地址偏移	   
		   	size -= secremain;				//字节数递减
			if(size > self->sector_size_)
                secremain = self->sector_size_;	//下一个扇区还是写不完
			else 
                secremain = size;			//下一个扇区可以写完了
		}	 
	};
}


/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TPerExtFlashDestroy(TOfObject *object)
{
    TPerExtFlash *self = (TPerExtFlash *)object;

    self->Destroy_(object);
}

void TPerExtFlashCreate(TPerExtFlash *self)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TPerExtFlashDestroy);
    
    TDrvSpiCreate(&self->drv_spi_, g_per_ext_flash_cfg_.drv_spi_chn);
    self->cs_port_ = g_per_ext_flash_cfg_.cs_port;
    self->cs_pin_ = g_per_ext_flash_cfg_.cs_pin;
    
    self->chip_type_ = kPerExtFlashTypeMax;
    self->total_size_ = 0;
    self->page_size_ = 0;

}

Bool TPerExtFlashOpen(TPerExtFlash *self)
{
    Bool result = False;
    UInt32 chip_id;
    TDrvSpiOpen(&self->drv_spi_);
    TDrvPortPinOpen(self->cs_port_, self->cs_pin_, True, NULL);
    
    chip_id = TPerExtFlashReadChipId(self);
    
    if (chip_id == kExtFlashTypeIdW25Q64BV)
    {
        self->chip_type_ = kPerExtFlashTypeW25Q64;
        self->total_size_ = 8 * 1024 * 1024;        ///< 8M
        self->sector_size_ = 4096;                  ///< 4K
        self->page_size_ = 256;                     ///< 单次写入页
        
        result = True;
    }
    return result;
}

void TPerExtFlashClose(TPerExtFlash *self)
{
    self->chip_type_ = kPerExtFlashTypeMax;
    TDrvSpiClose(&self->drv_spi_);
}

Bool TPerExtFlashErasePage(TPerExtFlash *self, UInt32 addr, UInt32 size)
{
    Bool result = False;
    if (self->chip_type_ < kPerExtFlashTypeMax && addr < self->total_size_ && size > 0
        && (addr % self->sector_size_) == 0 && (size % self->sector_size_) == 0)
    {
        while(size > 0 && addr < self->total_size_)
        {
            TPerExtFlashEraseSector(self, addr);
            addr += self->sector_size_;
            size -= self->sector_size_;
        }
        result = True;
    }
    return result;
}

Int32 TPerExtFlashWrite(TPerExtFlash *self, UInt32 addr, const UInt8 *data, Int32 size)
{
    Int32 result = 0;
    
    if (self->chip_type_ < kPerExtFlashTypeMax && data != NULL && size > 0
        && (addr + size) <= self->total_size_)
    {
        TPerExtFlashWriteAuto(self, addr, data, size);
        result = size;
    }
    
    return result;
}

Int32 TPerExtFlashRead(TPerExtFlash *self, UInt32 addr, UInt8 *buf, Int32 size)
{
    Int32 result = 0;
    
    if (self->chip_type_ < kPerExtFlashTypeMax && buf != NULL && size > 0
        && (addr + size) <= self->total_size_)
    {
        TPerExtFlashReadData(self, addr, buf, size);
        result = size;
    }
    
    return result;
}

void TPerExtFlashPowerDown(TPerExtFlash *self)
{
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdPowerDown);
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
}

void TPerExtFlashWakeUp(TPerExtFlash *self)
{
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdReleasePowerDown);
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
}

void TPerExtFlashEraseChip(TPerExtFlash *self)
{
    TPerExtFlashWriteEnable(self);
    TPerExtFlashWaitForWriteEnd(self);
    
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kExtFlashCmdEraseChip);
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, True);
    
    TPerExtFlashWaitForWriteEnd(self);
}
