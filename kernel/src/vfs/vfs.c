
#include "vfs.h"

TVfsDevMgr g_dev_mgr;        ///< 设备管理对象

/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TVfsDevBaseDestroy(TOfObject *obj)
{
    TVfsDevBase *self = (TVfsDevBase *)obj;

    self->Destroy_(obj);
}

void TVfsDevBaseCreate(TVfsDevBase *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TVfsDevBaseDestroy);
    self->open_ = NULL;
    self->close_ = NULL;
    self->ioctl_ = NULL;
    self->read_ = NULL;
    self->write_ = NULL;
}


Bool TVfsDevBaseOpen(TVfsDevBase *self, UInt8 sub_id)
{
    if (self->open_ != NULL)
        return self->open_(self, sub_id);
    else
        return False;
}

Bool TVfsDevBaseClose(TVfsDevBase *self, UInt8 sub_id)
{
    if (self->close_ != NULL)
        return self->close_(self, sub_id);
    else
        return False;
}


Bool TVfsDevBaseIoctl(TVfsDevBase *self, UInt8 sub_id, UInt32 cmd, void *argv)
{
    if (self->ioctl_ != NULL)
        return self->ioctl_(self, sub_id, cmd, argv);
    else
        return False;
}

Int32 TVfsDevBaseRead(TVfsDevBase *self, UInt8 sub_id, void *buf, UInt32 len)
{
    if (self->read_ != NULL)
        return self->read_(self, sub_id, buf, len);
    else
        return -1;
}

Int32 TVfsDevBaseWrite(TVfsDevBase *self, UInt8 sub_id, void *buf, UInt32 len)
{
    if (self->write_ != NULL)
        return self->write_(self, sub_id, buf, len);
    else
         return -1;
}




/**
 * 析构函数
 * @param object: 对象实例
 * @return 空
 */
static void TVfsDevMgrDestroy(TOfObject *obj)
{
    TVfsDevMgr *self = (TVfsDevMgr *)obj;

    self->Destroy_(obj);
}

void TVfsDevMgrCreate(TVfsDevMgr *self)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TVfsDevMgrDestroy);
}

Bool TVfsDevMgrRegister(TVfsDevMgr *self, UInt8 dev_id, TVfsDevBase *dev)
{
    Bool result = False;
    if (dev != NULL && dev_id < (sizeof(self->dev_storage_) / sizeof(self->dev_storage_[0])))
    {
        self->dev_storage_[dev_id] = dev;
        result = True;
    }
    return result;    
}

Bool TVfsDevMgrUnregister(TVfsDevMgr *self, UInt8 dev_id)
{
    Bool result = False;
    if (self->dev_storage_[dev_id] != NULL)
    {
        /// 不执行析构，只是释放
        self->dev_storage_[dev_id] = NULL;
        result = True;
    }
    return result;    
}


Bool TVfsDevMgrOpenDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id)
{
    if (self->dev_storage_[dev_id] != NULL)
    {
        if (TVfsDevBaseOpen(self->dev_storage_[dev_id], sub_id))
        {
            return True;
        }
    }
    return False;
}

Bool TVfsDevMgrCloseDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id)
{
    if (self->dev_storage_[dev_id] != NULL)
    {
        if (TVfsDevBaseClose(self->dev_storage_[dev_id], sub_id))
            return True;
    }
    return False;
}

Bool TVfsDevMgrIoctlDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, UInt32 cmd, void *argv)
{
    if (self->dev_storage_[dev_id] != NULL)
    {
        if (TVfsDevBaseIoctl(self->dev_storage_[dev_id], sub_id, cmd, argv))
            return True;
    }
    return False;
}

Int32 TVfsDevMgrReadDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, void *buf, UInt32 len)
{
    if (self->dev_storage_[dev_id] != NULL)
    {
        return TVfsDevBaseRead(self->dev_storage_[dev_id], sub_id, buf, len);
    }
    return 0; 
}

Int32 TVfsDevMgrWriteDev(TVfsDevMgr *self, UInt8 dev_id, UInt8 sub_id, void *buf, UInt32 len)
{
    if (self->dev_storage_[dev_id] != NULL)
    {
        return TVfsDevBaseWrite(self->dev_storage_[dev_id], sub_id, buf, len);
    }
    return 0;
}

