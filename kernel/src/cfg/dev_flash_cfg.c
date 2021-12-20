/**
 * @file dev_flash_cfg.c
 * @brief 用于flash设备配置
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
//  /// 页大小
// TDrvFlashPageSize g_flash_page_size[] = 
// {
//     {0x00000000, 0x4000},       ///< page 0 16K
//     {0x00004000, 0x4000},       ///< page 1 16K
//     {0x00008000, 0x4000},       ///< page 2 16K
//     {0x0000C000, 0x4000},       ///< page 3 16K
//     {0x00010000, 0x10000},      ///< page 4 64K
//     {0x00020000, 0x20000},      ///< page 5 128K
//     {0x00040000, 0x20000},      ///< page 6 128K
//     {0x00060000, 0x20000},      ///< page 7 128K
//     {0x00080000, 0x20000},      ///< page 8 128K
//     {0x000A0000, 0x20000},      ///< page 9 128K
//     {0x000C0000, 0x20000},      ///< page 10 128K
//     {0x000E0000, 0x20000},      ///< page 11 128K
// };

TDevFlashConfig g_dev_flash_cfg_[] =
{
    ///[0] = {.addr = 0x00020000, .size = 0x20000},    ///< app区(应用层不准操作)
    
    [0] = {.addr = 0x00008000, .size = 0x4000},     ///< app升级信息区
    [1] = {.addr = 0x00040000, .size = 0x20000},    ///< app备份区
};
