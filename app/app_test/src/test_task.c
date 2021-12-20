#include "test_task.h"
#include "kfc_memory.h"
#include "kfc_srt.h"
#include "srt_config.h"
#include "kfc_dev.h"
#include "kfc_timer.h"
#include "kfc_system.h"
#include "string.h"
#include "can_def.h"
#include "stdio.h"
#include "kfc_file.h"

/// 主任务对象
TTestTask g_test_task;


/// pwm设备打开
static void TTestTaskDevPwmOpen(TTestTask *self)
{
    UInt32 value;
    KfcDevOpen(kSfDevNumPwm1);
    value = 20; ///< 占空比 0~100
    KfcDevWrite(kSfDevNumPwm1, &value, 4);
}

/// phm测试
static void TTestTaskDevPhmTest(TTestTask *self)
{
    UInt16 write_value = 0x10;
    UInt16 read_value;
    ///< 设置参数
    KfcSrtPhmSetU16(kSrtPhmAppVersion, write_value);
    ///< 读取参数
    KfcSrtPhmGetU16(kSrtPhmAppVersion, &read_value);
}
/// 内部文件系统测试
static void TTestTaskDevFileTest(TTestTask *self)
{ 
    KfcFileFd fd;
    UInt8 flash_write[10] = {0x01, 0x02, 0x03, 0x04};
    UInt8 flash_read[10];
    UInt32 r_size;
    UInt32 w_size;
    TKfcFileResult m_result;
    if (KfcFileDeskIsReady(kKfcFileDiskTypeExtFlash) == kKfcFileResultOk)
    {
        /// m_result = KfcFileMkfs(kKfcFileDiskTypeExtFlash);    ///< 格式化磁盘
        m_result = KfcFileMount(kKfcFileDiskTypeExtFlash);
                    
        if (KfcFileOpen(kKfcFileDiskTypeExtFlash, &fd, "test.txt", kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite | kKfcFileOpenModeBitOpenAlways) == kKfcFileResultOk)
        {
//            KfcFileLSeek(&fd, 0);
            KfcFileWrite(&fd, &flash_write[0], sizeof(flash_write), &w_size);
//            
//            KfcFileLSeek(&fd, 0);
//            KfcFileRead(&fd, &flash_read[0], sizeof(flash_read), &r_size);
                    
            KfcFileClose(&fd);
        }
        KfcFileUnmount(kKfcFileDiskTypeExtFlash);
    }
}

/// 读u盘测试
static void TTestTaskDevUsbTest(TTestTask *self)
{ 
    KfcFileFd fd;
    UInt8 flash_write[10] = {0x01, 0x02, 0x03, 0x04};
    UInt8 flash_read[10];
    UInt32 r_size;
    UInt32 w_size;
    TKfcFileResult m_result;
    /// 判断是否u盘连接
    if (KfcFileDeskIsReady(kKfcFileDiskTypeUsb) == kKfcFileResultOk)
    {
        /// m_result = KfcFileMkfs(kKfcFileDiskTypeUsb);    ///< 格式化磁盘
        m_result = KfcFileMount(kKfcFileDiskTypeUsb);
                    
        if (KfcFileOpen(kKfcFileDiskTypeUsb, &fd, "test.txt", kKfcFileOpenModeBitRead | kKfcFileOpenModeBitWrite | kKfcFileOpenModeBitOpenAlways) == kKfcFileResultOk)
        {
//            KfcFileLSeek(&fd, 0);
            KfcFileWrite(&fd, &flash_write[0], sizeof(flash_write), &w_size);
//            
//            KfcFileLSeek(&fd, 0);
//            KfcFileRead(&fd, &flash_read[0], sizeof(flash_read), &r_size);
                    
            KfcFileClose(&fd);
        }
        KfcFileUnmount(kKfcFileDiskTypeUsb);
    }
}

/// 输出设备打开
static void TTestTaskDevPowerOpen(TTestTask *self)
{
    UInt32 i;
    for (i = kSfDevNumPowerDo0; i < TSfDevNumPowerMax; i++)
        KfcDevOpen(i);
}
/// 输入设备打开
static void TTestTaskDevSwitchOpen(TTestTask *self)
{
    UInt32 i;
    for (i = kSfDevNumSwitchDi0; i < TSfDevNumSwitchMax; i++)
        KfcDevOpen(i);
}
/// Adc设备打开
static void TTestTaskDevAdcOpen(TTestTask *self)
{
}
/// CAN设备打开
static void TTestTaskDevCanOpen(TTestTask *self)
{
    KfcDevOpen(kSfDevNumCan0);
    KfcDevOpen(kSfDevNumCan1);
    KfcDevOpen(kSfDevNumCan2);
}

/// 命令消息回调
static void TTestTaskRs485MsgNotify1(void *receiver, UInt8 serial_num, UInt8* data, UInt32 data_len)
{
    TTestTask *self = (TTestTask *)receiver;
    ///< 接收只判断流水号相等即可
    if (self->uart_process_[0].send_status == 2 && (self->uart_process_[0].send_cnt & 0xff) == serial_num)
    {
        self->uart_process_[0].resp_result = 1;
    }
    else if (self->uart_process_[0].send_status == 2)
    {
        self->uart_process_[0].resp_result = 2;
        self->uart_process_[0].rec_fail_cnt++;
    }
}

/// 串口发送函数
static UInt32 TTestTaskRs485Send1(void *receiver, UInt8 *data, UInt32 len)
{
//    TTestTask *self = (TTestTask *)receiver;

    return KfcDevWrite(kSfDevNumPRs485_1, data, len);
}

/// 命令消息回调
static void TTestTaskRs485MsgNotify2(void *receiver, UInt8 serial_num, UInt8* data, UInt32 data_len)
{
    TTestTask *self = (TTestTask *)receiver;
    ///< 接收只判断流水号相等即可
    if (self->uart_process_[1].send_status == 2 && (self->uart_process_[1].send_cnt & 0xff) == serial_num)
    {
        self->uart_process_[1].resp_result = 1;
    }
    else if (self->uart_process_[1].send_status == 2)
    {
        self->uart_process_[1].resp_result = 2;
        self->uart_process_[1].rec_fail_cnt++;
    }
}

/// 串口发送函数
static UInt32 TTestTaskRs485Send2(void *receiver, UInt8 *data, UInt32 len)
{
//    TTestTask *self = (TTestTask *)receiver;

    return KfcDevWrite(kSfDevNumPRs485_2, data, len);
}

/// Rs485设备打开
static void TTestTaskDevRs485Open(TTestTask *self)
{
    KfcDevOpen(kSfDevNumPRs485_1);
    KfcDevOpen(kSfDevNumPRs485_2);
    
    memset(&self->uart_send_data_[0], 0xA5, sizeof(self->uart_send_data_));
    
    TTestProtocolCreate(&self->uart_process_[0].test_protocol_, &self->uart_process_[0].protocol_buffer_[0], sizeof(self->uart_process_[0].protocol_buffer_),
    &self->uart_process_[0].protocol_send_buffer_[0], sizeof(self->uart_process_[0].protocol_send_buffer_));
    self->uart_process_[0].test_protocol_.receiver_ = self;
    self->uart_process_[0].test_protocol_.method_notify_ = TTestTaskRs485MsgNotify1;
    self->uart_process_[0].test_protocol_.method_send_ = TTestTaskRs485Send1;
    
    TTestProtocolCreate(&self->uart_process_[1].test_protocol_, &self->uart_process_[1].protocol_buffer_[0], sizeof(self->uart_process_[1].protocol_buffer_),
    &self->uart_process_[1].protocol_send_buffer_[0], sizeof(self->uart_process_[1].protocol_send_buffer_));
    self->uart_process_[1].test_protocol_.receiver_ = self;
    self->uart_process_[1].test_protocol_.method_notify_ = TTestTaskRs485MsgNotify2;
    self->uart_process_[1].test_protocol_.method_send_ = TTestTaskRs485Send2;
}
/// 命令处理-控制命令
static void TTestTaskMsgProcessCtl(TTestTask *self, UInt8 *data, UInt32 len)
{
    TProtocolPackBodyCtl *body = (TProtocolPackBodyCtl *)data;
    self->com_is_open_ = body->value;
}

/// 命令处理-power命令
static void TTestTaskMsgProcessPower(TTestTask *self, UInt8 *data, UInt32 len)
{
    TProtocolPackBodyPower *body = (TProtocolPackBodyPower *)data;
    UInt32 write_value;
    
    write_value = body->value;
    KfcDevWrite(kSfDevNumPowerDo0 + body->idx, &write_value, 4);
}

/// 命令处理-CAN命令
static void TTestTaskMsgProcessCan(TTestTask *self, UInt8 *data, UInt32 len)
{
    TProtocolPackBodyCanHead *can_head = (TProtocolPackBodyCanHead *)data;
    /// can发送
    if (can_head->cmd == kComProtocolCmdCanCmdSend)
    {
        TProtocolPackBodyCanSend *body = (TProtocolPackBodyCanSend *)data;
        
        self->can_process_[can_head->idx].is_send = body->send_cmd;
        self->can_process_[can_head->idx].send_inv = body->send_inv;
        /// 总数为0会一直发送
        self->can_process_[can_head->idx].send_total = body->send_cnt;
        self->can_process_[can_head->idx].send_data.id = body->send_id;
        
        if (self->can_process_[can_head->idx].send_data.id > 0x7ff)
            self->can_process_[can_head->idx].send_data.id_type = 1;
        else
            self->can_process_[can_head->idx].send_data.id_type = 0;
        self->can_process_[can_head->idx].send_data.bus_index = can_head->idx;
        self->can_process_[can_head->idx].send_data.frame_type = 0;
        self->can_process_[can_head->idx].send_data.length = 8;
        self->can_process_[can_head->idx].send_data.data[0] = 0x08;
        self->can_process_[can_head->idx].send_data.data[1] = 0x07;
        self->can_process_[can_head->idx].send_data.data[2] = 0x06;
        self->can_process_[can_head->idx].send_data.data[3] = 0x05;
        self->can_process_[can_head->idx].send_data.data[4] = 0x04;
        self->can_process_[can_head->idx].send_data.data[5] = 0x03;
        self->can_process_[can_head->idx].send_data.data[6] = 0x02;
        self->can_process_[can_head->idx].send_data.data[7] = 0x01;
        if (self->can_process_[can_head->idx].is_send)
        {
            self->can_process_[can_head->idx].send_cnt = 0;
            self->can_process_[can_head->idx].send_fail_cnt = 0;
            self->can_process_[can_head->idx].send_tick = 0;
        }
    }
    /// can接收
    else if (can_head->cmd == kComProtocolCmdCanCmdRec)
    {
        TProtocolPackBodyCanRec *body = (TProtocolPackBodyCanRec *)data;
        
        self->can_process_[can_head->idx].is_rec = body->rec_cmd;
        self->can_process_[can_head->idx].rec_id = body->rec_id;
        self->can_process_[can_head->idx].rec_cnt = 0;
    }
    /// 设置波特率
    else if (can_head->cmd == kComProtocolCmdCanCmdSetBaud)
    {
        TProtocolPackBodyCanSetBaud *body = (TProtocolPackBodyCanSetBaud *)data;
        UInt32 baud = body->baud;
        KfcDevIoctl(kSfDevNumCan0 + body->can_head.idx, kSfDevNumCanIoctlCmdSetBaud, &baud);
    }
}

/// 命令处理-Uart命令
static void TTestTaskMsgProcessUart(TTestTask *self, UInt8 *data, UInt32 len)
{
    TProtocolPackBodyUartHead *uart_head = (TProtocolPackBodyUartHead *)data;
    /// uart发送
    if (uart_head->cmd == kComProtocolCmdUartCmdSend)
    {
        TProtocolPackBodyUartSend *body = (TProtocolPackBodyUartSend *)data;
        
        self->uart_process_[uart_head->idx].is_send = body->send_cmd;
        self->uart_process_[uart_head->idx].send_inv = body->send_inv;
        /// 总数为0会一直发送
        self->uart_process_[uart_head->idx].send_total = body->send_cnt;
        self->uart_process_[uart_head->idx].send_size = body->send_size;
        /// 单包发送个数不能大于缓存
        if (self->uart_process_[uart_head->idx].send_size > sizeof(self->uart_send_data_))
            self->uart_process_[uart_head->idx].send_size = sizeof(self->uart_send_data_);
        if (self->uart_process_[uart_head->idx].is_send)
        {
            self->uart_process_[uart_head->idx].send_cnt = 0;
            self->uart_process_[uart_head->idx].send_fail_cnt = 0;
            self->uart_process_[uart_head->idx].send_tick = 0;
            self->uart_process_[uart_head->idx].send_status = 0;
        }
    }
    /// uart接收
    else if (uart_head->cmd == kComProtocolCmdUartCmdRec)
    {
        TProtocolPackBodyUartRec *body = (TProtocolPackBodyUartRec *)data;
        
        self->uart_process_[uart_head->idx].is_rec = body->rec_cmd;
        self->uart_process_[uart_head->idx].rec_cnt = 0;
        self->uart_process_[uart_head->idx].rec_fail_cnt = 0;
    }
    /// 设置波特率
    else if (uart_head->cmd == kComProtocolCmdUartCmdSetBaud)
    {
        TProtocolPackBodyUartSetBaud *body = (TProtocolPackBodyUartSetBaud *)data;
        UInt32 baud = body->baud;
        KfcDevIoctl(kSfDevNumPRs485_1 + body->uart_head.idx, kSfDevNumRs485IoctlCmdSetBaud, &baud);
    }
}
/// 命令处理
static void TTestTaskMsgProcess(TTestTask *self, UInt8 cmd , UInt8 *data, UInt32 len)
{
    switch(cmd)
    {
        case kComProtocolCmdCtl:
            TTestTaskMsgProcessCtl(self, data, len);
            break;
        case kComProtocolCmdPower:
            TTestTaskMsgProcessPower(self, data, len);
            break;
        case kComProtocolCmdCan:
            TTestTaskMsgProcessCan(self, data, len);
            break;
        case kComProtocolCmdUart:
            TTestTaskMsgProcessUart(self, data, len);
            break;
        default:
            break;
    };
}

/// 运行定时器上报
static void TTestTaskRunOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    
    /// 灯翻转闪烁
    self->led_status_ = 1 - self->led_status_;
    KfcDevWrite(kSfDevNumPowerDo0, &self->led_status_, 4);    
}
/// 启动定时器上报
static void TTestTaskStartOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    
    if (self->start_idx_ == 0)
    {
        TKfcTimerSetInterval(self->switch_timer_, 1000);
        TKfcTimerStart(self->switch_timer_);
    }
    else if (self->start_idx_ == 1)
    {
        TKfcTimerSetInterval(self->adc_timer_, 1000);
        TKfcTimerStart(self->adc_timer_);
    }
    else if (self->start_idx_ == 2)
    {
        TKfcTimerSetInterval(self->can_timer_, 1000);
        TKfcTimerStart(self->can_timer_);
        
    }
    else if (self->start_idx_ == 3)
    {
        TKfcTimerSetInterval(self->uart_timer_, 1000);
        TKfcTimerStart(self->uart_timer_);
    }
    else
        TKfcTimerStop(self->start_timer_);
    
    self->start_idx_++;
    
}

/// 开关量定时状态上报
static void TTestTaskSwitchOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    UInt32 i, read_value;
    TProtocolPackBodySwitch body;
    body.flag = 0;
    body.value = 0;
    if (self->com_is_open_)
    {
        for (i = 0; i < (TSfDevNumSwitchMax - kSfDevNumSwitchDi0); i++)
        {
            if (KfcDevRead(kSfDevNumSwitchDi0 + i, &read_value, 4))
            {
                if (read_value)
                    body.value |= (1 << i);
                body.flag |= (1 << i);
            }
        }
        
        TComProtocolSendSwitchValue(&self->com_protocol_, &body);
    }
}

/// Adc定时状态上报
static void TTestTaskAdcOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    if (self->com_is_open_)
    {
				/// Todo:读取adc值
        TComProtocolSendAdcValue(&self->com_protocol_, &self->adc_value_[0], 16);
    }
}

/// CAN定时状态上报
static void TTestTaskCanOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    UInt32 i;
    
    if (self->com_is_open_)
    {
        for (i = 0; i < 3; i++)
        {
            //if (self->can_process_[i].is_send)
            {
                TComProtocolSendCanSendResult(&self->com_protocol_, i, self->can_process_[i].send_cnt, self->can_process_[i].send_fail_cnt);
            }
            
            //if (self->can_process_[i].is_rec)
            {
                TComProtocolSendCanRecResult(&self->com_protocol_, i, self->can_process_[i].rec_cnt);
            }
        }
    }
}

/// UART定时状态上报
static void TTestTaskUartOnTimerIrq(void *receiver)
{
    TTestTask *self = (TTestTask *)receiver;
    UInt32 i;
    
    if (self->com_is_open_)
    {
        for (i = 0; i < 2; i++)
        {
            //if (self->uart_process_[i].is_send)
            {
                TComProtocolSendUartSendResult(&self->com_protocol_, i, self->uart_process_[i].send_cnt, self->uart_process_[i].send_fail_cnt);
            }
            
            //if (self->uart_process_[i].is_rec)
            {
                TComProtocolSendUartRecResult(&self->com_protocol_, i, self->uart_process_[i].rec_cnt, self->uart_process_[i].rec_fail_cnt);
            }
        }
    }
}

/// CAN检查
static void TTestTaskCanCheck(TTestTask *self)
{
    UInt32 i, j, now_tick, rec_len;
    now_tick = KfcGetTickCount();
    
    for (i = 0; i < 3; i++)
    {
        if (self->can_process_[i].is_send && (self->can_process_[i].send_total == 0 || (self->can_process_[i].send_cnt < self->can_process_[i].send_total)))
        {
            if (now_tick - self->can_process_[i].send_tick > self->can_process_[i].send_inv)
            {
                KfcDevWrite(kSfDevNumCan0 + i, &self->can_process_[i].send_data, sizeof(self->can_process_[i].send_data));
                self->can_process_[i].send_cnt++;
                self->can_process_[i].send_tick  = now_tick;
            }
        }
        
        if (self->can_process_[i].is_rec)
        {
            rec_len = KfcDevRead(kSfDevNumCan0 + i, &self->can_rec_data_[0], sizeof(self->can_rec_data_));
            if (rec_len > 0)
            {
                rec_len /= sizeof(TCanData);
                /// 如果不校验can id,直接增加个数
                if (self->can_process_[i].rec_id == 0)
                {
                    self->can_process_[i].rec_cnt += rec_len;
                }
                /// 校验can id
                else
                {
                    for (j = 0; j < rec_len; j++)
                    {
                        if (self->can_process_[i].rec_id == self->can_rec_data_[j].id)
                            self->can_process_[i].rec_cnt++;
                    }
                }
            }
        }
    }
}


/// uart检查
static void TTestTaskUartCheck(TTestTask *self)
{
    UInt32 i, j, now_tick, rec_len;
    now_tick = KfcGetTickCount();

    for (i = 0; i < 2; i++)
    {
        if (self->uart_process_[i].send_status == 1)
        {
            TTestProtocolSendData(&self->uart_process_[i].test_protocol_, self->uart_process_[i].send_cnt & 0xff, &self->uart_send_data_[0], self->uart_process_[i].send_size);
            self->uart_process_[i].send_status = 2;
            self->uart_process_[i].wait_resp_tick = now_tick;
            self->uart_process_[i].resp_result = 0;
        }
        else if (self->uart_process_[i].send_status == 2)
        {
            /// 超时错误或者错误
            if (now_tick - self->uart_process_[i].wait_resp_tick > 1000
                || self->uart_process_[i].resp_result == 2)
            {
                self->uart_process_[i].send_cnt++;
                self->uart_process_[i].rec_fail_cnt++;
                
                self->uart_process_[i].send_status = 0;
            }
            /// 应答成功
            else if (self->uart_process_[i].resp_result == 1)
            {
                self->uart_process_[i].send_cnt++;
                self->uart_process_[i].rec_cnt++;
                self->uart_process_[i].send_status = 0;
            }
        }
        else if (self->uart_process_[i].is_send && (self->uart_process_[i].send_total == 0 || (self->uart_process_[i].send_cnt < self->uart_process_[i].send_total)))
        {
            if (now_tick - self->uart_process_[i].send_tick > self->uart_process_[i].send_inv)
            {
                self->uart_process_[i].send_status = 1;
                self->uart_process_[i].send_tick  = now_tick;
            }
        }
        
        UInt32 read_len;
        
        read_len = KfcDevRead(kSfDevNumPRs485_1 + i, &self->uart_rec_data_[0], sizeof(self->uart_rec_data_));
        /// 循环解析
        if (read_len > 0)
        {
            TTestProtocolTransfer(&self->uart_process_[i].test_protocol_, &self->uart_rec_data_[0], read_len);
        }
    }
}


/// 命令消息回调
static void TTestTaskMsgNotify(void *receiver, UInt8 cmd, UInt8* data, UInt32 data_len)
{
    TTestTask *self = (TTestTask *)receiver;
    TTestTaskMsgProcess(self, cmd, data, data_len);
}

/// 串口发送函数
static UInt32 TTestTaskSend(void *receiver, UInt8 *data, UInt32 len)
{
//    TTestTask *self = (TTestTask *)receiver;

    return KfcDevWrite(kSfDevNumLog, data, len);
}
/**
 * 创建对象
 * @param object: 对象实例
 * @return 
 */
static void TTestTaskOnTaskCreate(TOfObject *object)
{
    TTestTask *self = (TTestTask *)object;
    
    TComProtocolCreate(&self->com_protocol_, &self->protocol_buffer_[0], sizeof(self->protocol_buffer_),
        &self->protocol_send_buffer_[0], sizeof(self->protocol_send_buffer_));
    self->com_protocol_.receiver_ = self;
    self->com_protocol_.method_notify_ = TTestTaskMsgNotify;
    self->com_protocol_.method_send_ = TTestTaskSend;
    
    self->com_is_open_ = False;
    int i;
    for (i = 0; i < 3; i++)
    {
        self->can_process_[i].is_send = 0;
        self->can_process_[i].is_rec = 1;
    }
    for (i = 0; i < 2; i++)
    {
        self->uart_process_[i].is_send = 0;
        self->uart_process_[i].is_rec = 1;
    }
}

/**
 * 启动
 * @param object: 对象实例
 * @return 
 */
static void TTestTaskOnTaskStart(TOfObject *object)
{
    TTestTask *self = (TTestTask *)object;
    
    KfcDevOpen(kSfDevNumLog);
    TTestTaskDevPowerOpen(self);
    TTestTaskDevSwitchOpen(self);
    TTestTaskDevAdcOpen(self);
    TTestTaskDevCanOpen(self);
    TTestTaskDevRs485Open(self);
    
    self->run_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->run_timer_->receiver = self;
    self->run_timer_->MethodOnTimerIrq = TTestTaskRunOnTimerIrq;
    TKfcTimerSetInterval(self->run_timer_, 1000);
    TKfcTimerStart(self->run_timer_);
    
    self->start_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->start_timer_->receiver = self;
    self->start_timer_->MethodOnTimerIrq = TTestTaskStartOnTimerIrq;
    TKfcTimerSetInterval(self->start_timer_, 200);
    TKfcTimerStart(self->start_timer_);
    self->start_idx_ = 0;
    
    self->switch_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->switch_timer_->receiver = self;
    self->switch_timer_->MethodOnTimerIrq = TTestTaskSwitchOnTimerIrq;
    
    self->adc_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->adc_timer_->receiver = self;
    self->adc_timer_->MethodOnTimerIrq = TTestTaskAdcOnTimerIrq;
    
    self->can_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->can_timer_->receiver = self;
    self->can_timer_->MethodOnTimerIrq = TTestTaskCanOnTimerIrq;
    
    
    self->uart_timer_ = TKfcTimerSrvCreateTimer(&g_kfc_timer_srv);
    self->uart_timer_->receiver = self;
    self->uart_timer_->MethodOnTimerIrq = TTestTaskUartOnTimerIrq;
    
    self->led_status_ = 0;
}

/**
 * 停止
 * @param object: 对象实例
 * @return 
 */
static void TTestTaskOnTaskStop(TOfObject *object)
{
//    TTestTask *self = (TTestTask *)object;
}

/**
 * 服务执行函数
 * @param object: 对象实例
 * @return 
 */
static void TTestTaskOnTaskExecute(TOfObject *object)
{
    TTestTask *self = (TTestTask *)object;

    while (!TKfcTaskTerminated((TKfcTask *)self))
    {
        UInt32 read_len;
        
        read_len = KfcDevRead(kSfDevNumLog, &self->com_read_buf_[0], sizeof(self->com_read_buf_));
        /// 循环解析
        if (read_len > 0)
        {
            TComProtocolTransfer(&self->com_protocol_, &self->com_read_buf_[0], read_len);
        }
        if (self->com_is_open_)
        {
            TTestTaskCanCheck(self);
            TTestTaskUartCheck(self);
        } 
        KfcSleep(1);
    }
}

/**
 * 析构函数
 * @param object: 对象实例
 * @return 
 */
static void TTestTaskDestroy(TOfObject *object)
{
    TTestTask *self = (TTestTask *)object;
    
    self->Destroy_(object);
}

void TTestTaskCreate(TTestTask *self)
{
#if defined (__GNUC__)
    static char stack[768] __attribute__ ((aligned (8)));
#else
    __align(8) static char stack[768];
#endif
    TKfcTaskCreate(&self->parent_, kKfcTaskPriority6, stack, sizeof(stack));
    OVERRIDE_METHOD(TOfObject, Destroy, TTestTaskDestroy);
    self->parent_.OnCreate = TTestTaskOnTaskCreate;
    self->parent_.OnStart = TTestTaskOnTaskStart;
    self->parent_.OnStop = TTestTaskOnTaskStop;
    self->parent_.OnExecute = TTestTaskOnTaskExecute;
    
    TKfcTaskResume(&self->parent_);
}
