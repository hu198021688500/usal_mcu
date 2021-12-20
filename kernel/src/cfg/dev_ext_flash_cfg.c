/**
 * @file dev_ext_flash_cfg.c
 * @brief 用于外部flash设备配置
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

TDevExtFlashConfig g_dev_ext_flash_cfg_[] =
{
    [0] = {.addr = 0x0000, .size = 0x1000},
    [1] = {.addr = 0x1000, .size = 0x1000},
};

TDevPerExtFlashConfig g_dev_per_ext_flash_cfg_ = 
{
    .cs_port = &g_drv_port[kDrvPortNumA],
    .cs_pin = 15,
};
