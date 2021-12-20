#include "per_can_mcp2515.h"
#include "drv_spi.h"
#include "string.h"

TPerCanMcp2515 g_per_can_mcp2515;

/// 操作命令
enum
{
    kMcp2515cmdWrite        = 0x02,     ///< 写命令
    kMcp2515cmdRead         = 0x03,     ///< 读命令
    kMcp2515cmdReset        = 0xC0,     ///< 复位命令
    
    kMcp2515cmdRdStatus     = 0xA0,     ///< 读can发送状态
    kMcp2515cmdRxStatus     = 0xB0,     ///< 读can获取状态
    
    kMcp2515cmdTXB0SIDH     = 0x40,     ///< TX0 ID location
    kMcp2515cmdTXB0D0       = 0x41,     ///< TX0 Data location
    kMcp2515cmdTXB1SIDH     = 0x42,     ///< TX1 ID location
    kMcp2515cmdTXB1D0       = 0x43,     ///< TX1 Data location
    kMcp2515cmdTXB2SIDH     = 0x44,     ///< TX2 ID location
    kMcp2515cmdTXB2D0       = 0x45,     ///< TX2 Data location
    
    kMcp2515cmdRtsTx0       = 0x81,     ///< 请求发送TX0
    kMcp2515cmdRtsTx1       = 0x82,     ///< 请求发送TX1
    kMcp2515cmdRtsTx2       = 0x84,     ///< 请求发送TX2
    kMcp2515cmdRtsTxAll     = 0x87,     ///< 请求发送所有

    kMcp2515cmdRdRXB0SIDH   = 0x90,
    kMcp2515cmdRdRXB0D0     = 0x92,
    kMcp2515cmdRdRXB1SIDH   = 0x94,
    kMcp2515cmdRdRXB1D0     = 0x96,

};
/// 寄存器定义
enum
{
    kMcp2515RegisterCANSTAT = 0x0E,     ///< 状态寄存器
    kMcp2515RegisterCANCTRL = 0x0F,     ///< 控制寄存器
    
    kMcp2515RegisterCNF3    = 0x28,
    kMcp2515RegisterCNF2    = 0x29,
    kMcp2515RegisterCNF1    = 0x2A,    
    
    ///< bit0:接收器0中断 bit1:接收器1满中断 bit2:发送缓冲区0空中断 bit3:发送缓冲区1空中断
    ///< bit4:发送缓冲区2空中断 bit5:错误中断(详读EFLG寄存器) bit6:唤醒中断 bit7:报文错误中断
    kMcp2515RegisterCANINTE    = 0x2B,  
    kMcp2515RegisterCANINTF    = 0x2C, 
    ///<错误标志 bit0:错误警告 bit1:接收错误警告 bit2:发送错误警告 bit3:接收被动错误
    ///< bit4:发送被动错误 bit5:总线关闭错误 bit6:接收缓冲区0溢出 bit7:接收缓冲区1溢出
    kMcp2515RegisterCANEFLG    = 0x2D,  ///< 错误
    
    /// Receive Masks
    kMcp2515RegisterRXM0SIDH    = 0x20,
    kMcp2515RegisterRXM0SIDL    = 0x21,
    kMcp2515RegisterRXM0EID8    = 0x22,
    kMcp2515RegisterRXM0EID0    = 0x23,
    kMcp2515RegisterRXM1SIDH    = 0x24,
    kMcp2515RegisterRXM1SIDL    = 0x25,
    kMcp2515RegisterRXM1EID8    = 0x26,
    kMcp2515RegisterRXM1EID0    = 0x27,
    /// Recieve Filters
    kMcp2515RegisterRXF0SIDH    = 0x00,
    kMcp2515RegisterRXF0SIDL    = 0x01,
    kMcp2515RegisterRXF0EID8    = 0x02,
    kMcp2515RegisterRXF0EID0    = 0x03,
    kMcp2515RegisterRXF1SIDH    = 0x04,
    kMcp2515RegisterRXF1SIDL    = 0x05,
    kMcp2515RegisterRXF1EID8    = 0x06,
    kMcp2515RegisterRXF1EID0    = 0x07,
    kMcp2515RegisterRXF2SIDH    = 0x08,
    kMcp2515RegisterRXF2SIDL    = 0x09,
    kMcp2515RegisterRXF2EID8    = 0x0A,
    kMcp2515RegisterRXF2EID0    = 0x0B,
    kMcp2515RegisterRXF3SIDH    = 0x10,
    kMcp2515RegisterRXF3SIDL    = 0x11,
    kMcp2515RegisterRXF3EID8    = 0x12,
    kMcp2515RegisterRXF3EID0    = 0x13,
    kMcp2515RegisterRXF4SIDH    = 0x14,
    kMcp2515RegisterRXF4SIDL    = 0x15,
    kMcp2515RegisterRXF4EID8    = 0x16,
    kMcp2515RegisterRXF4EID0    = 0x17,
    kMcp2515RegisterRXF5SIDH    = 0x18,
    kMcp2515RegisterRXF5SIDL    = 0x19,
    kMcp2515RegisterRXF5EID8    = 0x1A,
    kMcp2515RegisterRXF5EID0    = 0x1B,
    /// Rx Buffer 0
    kMcp2515RegisterRXB0CTRL    = 0x60,
    /// Rx Buffer 1
    kMcp2515RegisterRXB1CTRL    = 0x70,
};

#pragma anon_unions
typedef union
{
  struct
  {
    UInt8 filter     : 3;
    UInt8 msgType    : 2; ///< 1:std 2:ext
    UInt8 unusedBit  : 1;
    UInt8 rxBuffer   : 2; ///< 1:buffer0, 2:buffer1 3:both
  };
  UInt8 rx_status;
  
}TMcp2515RxStatus;

typedef union{
  struct{
    UInt8 RXBnSIDH;
    UInt8 RXBnSIDL;
    UInt8 RXBnEID8;
    UInt8 RXBnEID0;
    UInt8 RXBnDLC;
    UInt8 RXBnD0;
    UInt8 RXBnD1;
    UInt8 RXBnD2;
    UInt8 RXBnD3;
    UInt8 RXBnD4;
    UInt8 RXBnD5;
    UInt8 RXBnD6;
    UInt8 RXBnD7;
  };
  UInt8 rx_reg_array[13];
}TMcp2515RxReg;

typedef union{
  struct{
    unsigned RX0IF      : 1;
    unsigned RX1IF      : 1;
    unsigned TXB0REQ    : 1;
    unsigned TX0IF      : 1;
    unsigned TXB1REQ    : 1;
    unsigned TX1IF      : 1;
    unsigned TXB2REQ    : 1;
    unsigned TX2IF      : 1;
  };
  UInt8 ctrl_status;  
}TMcp2515CtrlStatus;


typedef struct{
  UInt8 tempSIDH;
  UInt8 tempSIDL;
  UInt8 tempEID8;
  UInt8 tempEID0;
}TMcp2515IdReg;


typedef struct TMcp2515BaudCfgTag TMcp2515BaudCfg; 
struct TMcp2515BaudCfgTag
{
    UInt8 cfg1;
    UInt8 cfg2;
    UInt8 cfg3;
};
/// 8M频率波特率设置
static TMcp2515BaudCfg g_mcp2515_baud_cfg_8m_[] =
{
    {0x00, 0xB1, 0x85},             ///< 250K
    {0x00, 0x90, 0x82},             ///< 500K
    {0x00, 0x80, 0x80},             ///< 1000K
    {0x01, 0xB4, 0x86},             ///< 100K
    {0x01, 0xB1, 0x85},             ///< 125K
};
/// 16M频率波特率设置
static TMcp2515BaudCfg g_mcp2515_baud_cfg_16m_[] =
{
    {0x41, 0xF1, 0x85},             ///< 250K
    {0x00, 0xF0, 0x86},             ///< 500K
    {0x00, 0xD0, 0x82},             ///< 1000K
    {0x03, 0xFA, 0x87},             ///< 100K
    {0x03, 0xF0, 0x86},             ///< 125K
};
/**
 * spi片选设置
 * @param object: 对象实例
 * @param value: True:高  False:低
 * @return 
 */
static void TPerCanSpiCs(TPerCanMcp2515 *self, Bool value)
{
    TDrvPortPinSet(self->cs_port_, self->cs_pin_, value);
}
/**
 * Mcp2515往地址写1字节数据
 * @param object: 对象实例
 * @param addr: mcp2515寄存器地址
 * @param data: 1字节数据
 * @return
 */
static void TPerCanMcp2515WriteByte(TPerCanMcp2515 *self, UInt8 addr, UInt8 data)
{
    TPerCanSpiCs(self, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, kMcp2515cmdWrite);
    TDrvSpiWriteAndRead(&self->drv_spi_, addr);
    TDrvSpiWriteAndRead(&self->drv_spi_, data);
    
    TPerCanSpiCs(self, True);
}
/**
 * Mcp2515命令复位
 * @param object: 对象实例
 * @return
 */
static void TPerCanMcp2515Reset(TPerCanMcp2515 *self)
{
    TPerCanSpiCs(self, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, kMcp2515cmdReset);
    
    TPerCanSpiCs(self, True);
}

/**
 * Mcp2515命令获取can接收状态
 * @param object: 对象实例
 * @return 返回can接收状态
 */
static UInt8 TPerCanMcp2515GetRxStatus(TPerCanMcp2515 *self)
{
    UInt8 result = 0;
    
    TPerCanSpiCs(self, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kMcp2515cmdRxStatus);
    result = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff);
    TPerCanSpiCs(self, True);
    
    return result;
}

/**
 * Mcp2515命令获取can发送状态
 * @param object: 对象实例
 * @return 返回can发送状态
 */
static UInt8 TPerCanMcp2515GetRdStatus(TPerCanMcp2515 *self)
{
    UInt8 result = 0;
    
    TPerCanSpiCs(self, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, kMcp2515cmdRdStatus);
    result = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff);
    TPerCanSpiCs(self, True);
    
    return result;
}

/**
 * Mcp2515从地址读1字节数据
 * @param object: 对象实例
 * @param addr: mcp2515寄存器地址
 * @param data: 1字节数据
 * @return
 */
static UInt8 TPerCanMcp2515ReadByte(TPerCanMcp2515 *self, UInt8 addr)
{
    UInt8 result = 0;
    
    TPerCanSpiCs(self, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, kMcp2515cmdRead);
    TDrvSpiWriteAndRead(&self->drv_spi_, addr);
    result = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff);
    
    TPerCanSpiCs(self, True);
    
    return result;
}
/**
 * Mcp2515多字节数据读取
 * @param object: 对象实例
 * @param cmd: mcp2515命令值
 * @param buf: 数据地址
 * @param len: 读取长度
 * @return
 */
void TPerCanMcp2515ReadSequence(TPerCanMcp2515 *self, UInt8 cmd, UInt8 *buf, UInt8 len)
{
    UInt32 i;
    
    TPerCanSpiCs(self, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, cmd);
    for (i = 0; i < len; i++)
        buf[i] = TDrvSpiWriteAndRead(&self->drv_spi_, 0xff);
    
    TPerCanSpiCs(self, True);
}
/**
 * Mcp2515发送can数据到缓冲区
 * @param object: 对象实例
 * @param instruction: 缓冲区寄存器
 * @param id_reg: id寄存器(4字节)
 * @param dlc: 数据长度
 * @param data: 数据地址
 * @return
 */
void TPerCanMcp2515CanTxSequence(TPerCanMcp2515 *self, UInt8 instruction, UInt8 *id_reg, UInt8 dlc, UInt8 *data)
{
    UInt32 i;
    TPerCanSpiCs(self, False);
    TDrvSpiWriteAndRead(&self->drv_spi_, instruction);
    for (i = 0; i < 4; i++)
        TDrvSpiWriteAndRead(&self->drv_spi_, id_reg[i]);
    TDrvSpiWriteAndRead(&self->drv_spi_, dlc);
    for (i = 0; i < dlc; i++)
        TDrvSpiWriteAndRead(&self->drv_spi_, data[i]);
    TPerCanSpiCs(self, True);
}
/**
 * Mcp2515请求发送
 * @param object: 对象实例
 * @param instruction: 请求发送对应缓存命令
 * @return
 */
void TPerCanMcp2515CanReqToSend(TPerCanMcp2515 *self, UInt8 instruction)
{
    TPerCanSpiCs(self, False);
    
    TDrvSpiWriteAndRead(&self->drv_spi_, instruction);
      
    TPerCanSpiCs(self, True);
}


/**
 * Mcp2515设置进入配置模式
 * @param object: 对象实例
 * @return True:成功 False:失败
 */
static Bool TPerCanMcp2515SetConfigMode(TPerCanMcp2515 *self)
{
    Bool result = False;
    UInt32 loop = 10;
    
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANCTRL, 0x80);
    while(loop > 0)
    {
        if ((TPerCanMcp2515ReadByte(self, kMcp2515RegisterCANSTAT) & 0xE0) == 0x80)
        {
            result = True;
            break;
        }
        loop--;
    }
    return result;
}
/**
 * Mcp2515设置进入正常模式
 * @param object: 对象实例
 * @return True:成功 False:失败
 */
static Bool TPerCanMcp2515SetNormalMode(TPerCanMcp2515 *self)
{
    Bool result = False;
    UInt32 loop = 10;
    
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANCTRL, 0x00);
    while(loop > 0)
    {
        if ((TPerCanMcp2515ReadByte(self, kMcp2515RegisterCANSTAT) & 0xE0) == 0x00)
        {
            result = True;
            break;
        }
        loop--;
    }
    return result;
}
/**
 * Mcp2515设置进入休眠模式
 * @param object: 对象实例
 * @return True:成功 False:失败
 */
static Bool TPerCanMcp2515SetSleepMode(TPerCanMcp2515 *self)
{
    Bool result = False;
    UInt32 loop = 10;
    
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANCTRL, 0x20);
    while(loop > 0)
    {
        if ((TPerCanMcp2515ReadByte(self, kMcp2515RegisterCANSTAT) & 0xE0) == 0x20)
        {
            result = True;
            break;
        }
        loop--;
    }
    return result;
}

/**
 * Mcp2515配置波特率
 * @param object: 对象实例
 * @param baud_index: 波特率序列
 * @param clk_item: 晶振使用 0:8M 1:16M
 * @return
 */
static void TPerCanMcp2515SetBaud(TPerCanMcp2515 *self, TPerCanMcp2515BaudRate baud_item, UInt8 clk_item)
{
    TMcp2515BaudCfg *baud_cfg = &g_mcp2515_baud_cfg_8m_[0];
    if (clk_item == 1)
        baud_cfg = &g_mcp2515_baud_cfg_16m_[0];
    
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCNF1, baud_cfg[baud_item].cfg1);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCNF2, baud_cfg[baud_item].cfg2);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCNF3, baud_cfg[baud_item].cfg3);    
}
/**
 * Mcp2515配置过滤表
 * @param object: 对象实例
 * @return
 */
static void TPerCanMcp2515SetFilter(TPerCanMcp2515 *self)
{
    /// 配置接收掩码寄存器
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM0SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM0SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM0EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM0EID0, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM1SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM1SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM1EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXM1EID0, 0x00);
    
    /// 配置接收过滤器
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF0SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF0SIDL, 0x00);      ///< Starndard Filter
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF0EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF1SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF1SIDL, 0x08);      ///< Exntended Filter
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF1EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF1EID0, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF2SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF2SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF2EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF2EID0, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF3SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF3SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF3EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF3EID0, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF4SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF4SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF4EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF5SIDH, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF5SIDL, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF5EID8, 0x00);
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXF5EID0, 0x00);
    
    /// 接收所有帧
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXB0CTRL, 0x04);    ///< Enable BUKT, Accept Filter 0
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterRXB1CTRL, 0x01);    ///< Accept Filter 1
}

/// 根据寄存器转换成扩展帧
static UInt32 TPerCanMcp2515RegConvertExtId(TPerCanMcp2515 *self, UInt8 tempRXBn_EIDH, UInt8 tempRXBn_EIDL, UInt8 tempRXBn_SIDH, UInt8 tempRXBn_SIDL) 
{
  UInt32 returnValue = 0;
  UInt32 ConvertedID = 0;
  UInt8 CAN_standardLo_ID_lo2bits;
  UInt8 CAN_standardLo_ID_hi3bits;
  
  CAN_standardLo_ID_lo2bits = (tempRXBn_SIDL & 0x03);
  CAN_standardLo_ID_hi3bits = (tempRXBn_SIDL >> 5);
  ConvertedID = (tempRXBn_SIDH << 3);
  ConvertedID = ConvertedID + CAN_standardLo_ID_hi3bits;
  ConvertedID = (ConvertedID << 2);
  ConvertedID = ConvertedID + CAN_standardLo_ID_lo2bits;
  ConvertedID = (ConvertedID << 8);
  ConvertedID = ConvertedID + tempRXBn_EIDH;
  ConvertedID = (ConvertedID << 8);
  ConvertedID = ConvertedID + tempRXBn_EIDL;
  returnValue = ConvertedID;    
  return (returnValue);
}

/// 根据寄存器转换成标准帧
static UInt32 TPerCanMcp2515RegConvertStdId(TPerCanMcp2515 *self, UInt8 tempRXBn_SIDH, UInt8 tempRXBn_SIDL) 
{
  UInt32 returnValue = 0;
  UInt32 ConvertedID;
  
  ConvertedID = (tempRXBn_SIDH << 3);
  ConvertedID = ConvertedID + (tempRXBn_SIDL >> 5);
  returnValue = ConvertedID;
  
  return (returnValue);
}


static void TPerCanMcp2515CanIdToReg(TPerCanMcp2515 *self, UInt32 tempPassedInID, UInt8 canIdType, TMcp2515IdReg *passedIdReg) 
{
  UInt8 wipSIDL = 0;
  
  /// ext_id
  if (canIdType == 1) 
  {
    //EID0
    passedIdReg->tempEID0 = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;
    
    //EID8
    passedIdReg->tempEID8 = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;
    
    //SIDL
    wipSIDL = 0x03 & tempPassedInID;
    tempPassedInID = tempPassedInID << 3;
    wipSIDL = (0xE0 & tempPassedInID) + wipSIDL;
    wipSIDL = wipSIDL + 0x08;
    passedIdReg->tempSIDL = 0xEB & wipSIDL;
    
    //SIDH
    tempPassedInID = tempPassedInID >> 8;
    passedIdReg->tempSIDH = 0xFF & tempPassedInID;
  } 
  else
  {
    passedIdReg->tempEID8 = 0;
    passedIdReg->tempEID0 = 0;
    tempPassedInID = tempPassedInID << 5;
    passedIdReg->tempSIDL = 0xFF & tempPassedInID;
    tempPassedInID = tempPassedInID >> 8;
    passedIdReg->tempSIDH = 0xFF & tempPassedInID;
  }
}

/**
 * Mcp2515 CAN读取一帧
 * @param object: 对象实例
 * @param can_msg: can存储数据
 * @return True: 获取成功 False:失败
 */
static Bool TPerCanMcp2515Receive(TPerCanMcp2515 *self, TCanData *can_msg)
{
    Bool result = False;
    
    TMcp2515RxStatus rx_status;
    TMcp2515RxReg rx_reg;
    rx_status.rx_status = TPerCanMcp2515GetRxStatus(self);
    
    if (rx_status.rxBuffer != 0)
    {
        /// 读取buffer0
        if ((rx_status.rxBuffer == 1)|(rx_status.rxBuffer == 3))
        {
            TPerCanMcp2515ReadSequence(self, kMcp2515cmdRdRXB0SIDH, &rx_reg.rx_reg_array[0], sizeof(rx_reg.rx_reg_array));
        }
        /// 读取buffer1
        else if (rx_status.rxBuffer == 2)
        {
            TPerCanMcp2515ReadSequence(self, kMcp2515cmdRdRXB1SIDH, &rx_reg.rx_reg_array[0], sizeof(rx_reg.rx_reg_array));
        }
        
        /// 扩展帧
        if (rx_status.msgType == 2)
        {
            can_msg->id_type = 1;
            can_msg->id = TPerCanMcp2515RegConvertExtId(self, rx_reg.RXBnEID8, rx_reg.RXBnEID0, rx_reg.RXBnSIDH, rx_reg.RXBnSIDL);
        } 
        else 
        {
            can_msg->id_type = 0;
            can_msg->id = TPerCanMcp2515RegConvertStdId(self, rx_reg.RXBnSIDH, rx_reg.RXBnSIDL);
        }
    
        can_msg->length   = rx_reg.RXBnDLC;
        can_msg->data[0] = rx_reg.RXBnD0;
        can_msg->data[1] = rx_reg.RXBnD1;
        can_msg->data[2] = rx_reg.RXBnD2;
        can_msg->data[3] = rx_reg.RXBnD3;
        can_msg->data[4] = rx_reg.RXBnD4;
        can_msg->data[5] = rx_reg.RXBnD5;
        can_msg->data[6] = rx_reg.RXBnD6;
        can_msg->data[7] = rx_reg.RXBnD7;
        
        result = True;
    }
    return result;
}
/**
 * Mcp2515 CAN发送
 * @param object: 对象实例
 * @param can_msg: can发送数据
 * @return True: 获取成功 False:失败
 */
Bool TPerCanMcp2515Transmit(TPerCanMcp2515 *self, TCanData *can_msg) 
{
    Bool result = False;
    TMcp2515IdReg id_reg;
    TMcp2515CtrlStatus ctrl_status;
    UInt32 i;
    for (i = 0; i < 100; i++)
    {
        ctrl_status.ctrl_status = TPerCanMcp2515GetRdStatus(self);

        if (ctrl_status.TXB0REQ != 1)
        {
            TPerCanMcp2515CanIdToReg(self, can_msg->id, can_msg->id_type, &id_reg);
            /// 发送数据到缓冲区
            TPerCanMcp2515CanTxSequence(self, kMcp2515cmdTXB0SIDH, &(id_reg.tempSIDH), can_msg->length, &can_msg->data[0]);
            /// 请求发送Tx0
            TPerCanMcp2515CanReqToSend(self, kMcp2515cmdRtsTx0);
            result = True;
            break;
        }
        else if (ctrl_status.TXB1REQ != 1)
        {
            TPerCanMcp2515CanIdToReg(self, can_msg->id, can_msg->id_type, &id_reg);
            /// 发送数据到缓冲区
            TPerCanMcp2515CanTxSequence(self, kMcp2515cmdTXB1SIDH, &(id_reg.tempSIDH), can_msg->length, &can_msg->data[0]);
            /// 请求发送Tx0
            TPerCanMcp2515CanReqToSend(self, kMcp2515cmdRtsTx1);
            result = True;
            break;
        }
        else if (ctrl_status.TXB2REQ != 1)
        {
            TPerCanMcp2515CanIdToReg(self, can_msg->id, can_msg->id_type, &id_reg);
            /// 发送数据到缓冲区
            TPerCanMcp2515CanTxSequence(self, kMcp2515cmdTXB2SIDH, &(id_reg.tempSIDH), can_msg->length, &can_msg->data[0]);
            /// 请求发送Tx0
            TPerCanMcp2515CanReqToSend(self, kMcp2515cmdRtsTx2);
            result = True;
            break;
        }
    }
    return result;    
}


/**
 * can中断函数
 * @param object: 对象实例
 * @return 
 */
void TPerCanMcp2515IntIrqEvent(void *recevier)
{
    TPerCanMcp2515 *self = (TPerCanMcp2515 *)recevier;
    
    UInt8 flag = TPerCanMcp2515ReadByte(self, kMcp2515RegisterCANINTF);
    
    if (flag & 0x03)
    {
        TCanData msg;
        ///< 不用while循环有问题,mcp2515如果缓冲区满不会再产生中断
        while (TPerCanMcp2515Receive(self, &msg))
        {
            msg.bus_index = 2;
            msg.param.recv_tick = 0;
            
            TOfRingBufWrite(&self->rx_buf_, &msg, sizeof(msg));
        }
    }
    /// 数据错误中断(需要读取EFLG寄存器)
    if (flag & (1 << 5))
    {
    }
    /// 报文错误中断
    if (flag & (1 << 7))
    {
    }
//    /// 清中断标记
    TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANINTF, 0);
}
/**
 * 定时器1ms中断，循环读取can报文
 * @param object: 对象实例
 * @note 备注:芯片不能打开中断接收，不然can出现busoff不会自动恢复,原因不明
 * @return 
 */
static void TPerCanOnTimerIrq(void *receiver)
{
    TPerCanMcp2515 *self = (TPerCanMcp2515 *)receiver;
    TCanData msg;
    while (TPerCanMcp2515Receive(self, &msg))
    {
        msg.bus_index = 2;
        msg.param.recv_tick = 0;
        
        TOfRingBufWrite(&self->rx_buf_, &msg, sizeof(msg));
    }
}
/**
 * 类的析构函数
 * @param object: 对象实例
 * @return 
 */
static void TPerCanMcp2515Destroy(TOfObject *object)
{
    TPerCanMcp2515 *self = (TPerCanMcp2515 *)object;

    self->Destroy_(object);
}

void TPerCanMcp2515Create(TPerCanMcp2515 *self, void *rx_buf, Int32 rx_buf_size, TDrvPort *cs_port, UInt8 cs_pin, TDrvPortNum int_port, Byte int_pin)
{
    /// 构建基类
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TPerCanMcp2515Destroy);
    
    TDrvSpiCreate(&self->drv_spi_, 1);
    self->cs_port_ = cs_port;
    self->cs_pin_ = cs_pin;
    
//    TDrvExtIntCreate(&self->drv_ext_int_, int_port, int_pin);
//    self->drv_ext_int_.MethodOnIrq = TPerCanMcp2515IntIrqEvent;
//    self->drv_ext_int_.recevier = self;
//    
    TOfRingBufCreate(&self->rx_buf_, rx_buf, rx_buf_size);
    /// 创建1ms定时器循环检查数据
    self->check_timer = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->check_timer->receiver = self;
    self->check_timer->MethodOnTimerIrq = TPerCanOnTimerIrq;
    TKfcTimerSetInterval(self->check_timer, 1);
}

Bool TPerCanMcp2515Open(TPerCanMcp2515 *self, TPerCanMcp2515BaudRate baud_item)
{
    Bool result = False;
    
    TDrvSpiOpen(&self->drv_spi_);
    TDrvPortPinOpen(self->cs_port_, self->cs_pin_, True, NULL);

    /// 进入配置模式
    if (TPerCanMcp2515SetConfigMode(self))
    {
        /// 配置波特率
        TPerCanMcp2515SetBaud(self, baud_item, 1);
        /// 配置过滤表,接收所有帧
        TPerCanMcp2515SetFilter(self);
        
        ///<备注:芯片不能打开中断，不然can出现busoff不会自动恢复,原因不明
        //TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANINTF,0x00); ///< 清中断标记寄存器
        //TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANINTE,0x03); ///< 打开缓存区0和缓存区1中断使能
        //TPerCanMcp2515WriteByte(self, kMcp2515RegisterCANINTE,0xA3); ///< 打开缓存区0和缓存区1中断使能和错误中断
	
        /// 进入正常模式
        TPerCanMcp2515SetNormalMode(self);
        /// 打开定时器
        TKfcTimerStart(self->check_timer);
        result = True;
    }
    
    //TDrvExtIntSetTrigerMode(&self->drv_ext_int_, kDrvExtIntFalling);
    return result;
}

void TPerCanMcp2515Close(TPerCanMcp2515 *self)
{
    TKfcTimerStop(self->check_timer);
    ///TDrvExtIntSetTrigerMode(&self->drv_ext_int_, kDrvExtIntDisable);
}

Bool TPerCanMcp2515Write(TPerCanMcp2515 *self, TCanData *data)
{
    return TPerCanMcp2515Transmit(self, data);
}

Int32 TPerCanMcp2515Read(TPerCanMcp2515 *self, TCanData *data, Int32 count)
{
    Int32 result = TOfRingBufDataSize(&self->rx_buf_);
    if (result > 0)
    {
        result /= sizeof(*data);
        if (result > count)
           result = count;

        TOfRingBufRead(&self->rx_buf_, (void *)data, result * sizeof(*data));
    }
    return result;
}

