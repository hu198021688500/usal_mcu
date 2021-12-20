/**
 * @file dev_rs485_cfg.c
 * @brief 用于485设备配置
 * @note 
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */
 
TDevRs485Config g_dev_rs485_cfg_[] =
{
    [0] = {.drv_uart_chn = 2, .baud_default = 9600,},
    [1] = {.drv_uart_chn = 3, .baud_default = 9600,},
};

