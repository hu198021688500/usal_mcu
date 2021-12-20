#include "kfc_dev.h"
#include "vfs.h"


Bool KfcDevOpen(UInt16 dev_num)
{
   return TVfsDevMgrOpenDev(&g_dev_mgr, (dev_num >> 8), (dev_num & 0xff));
}

Bool KfcDevClose(UInt16 dev_num)
{
   return TVfsDevMgrCloseDev(&g_dev_mgr, (dev_num >> 8), (dev_num & 0xff));
}

Bool KfcDevIoctl(UInt16 dev_num, UInt32 cmd, void *argv)
{
   return TVfsDevMgrIoctlDev(&g_dev_mgr, (dev_num >> 8), (dev_num & 0xff), cmd, argv);    
}

Int32 KfcDevRead(UInt16 dev_num, void *buf, UInt32 len)
{
   return TVfsDevMgrReadDev(&g_dev_mgr, (dev_num >> 8), (dev_num & 0xff), buf, len);    
}


Int32 KfcDevWrite(UInt16 dev_num, void *buf, UInt32 len)
{
   return TVfsDevMgrWriteDev(&g_dev_mgr, (dev_num >> 8), (dev_num & 0xff), buf, len);    
}


