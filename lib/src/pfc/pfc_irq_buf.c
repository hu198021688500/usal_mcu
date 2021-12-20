#include "pfc_irq_buf.h"
#include <ofc_string.h>

static void TPfIrqWriteBufDestroy(TOfObject *self)
{
    ((TPfIrqWriteBuf *)self)->Destroy_(self);
}

void TPfIrqWriteBufCreate( TPfIrqWriteBuf *self, void *buf, Int32 buf_size)
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TPfIrqWriteBufDestroy);
    
    self->buf_ = (char *)buf;
    self->buf_size_ = buf_size;
    self->write_pos_ = 0;
    self->read_pos_ = 0;
    self->clean_pos_ = 0;
    self->reading_ = False;
}

/**
 * 把未读取的数据移动到缓冲区最前面
 * @param self: 对象实例
 * @return 
 */
static void TPfIrqWriteBufPack(TPfIrqWriteBuf *self)
{
    self->write_pos_ -= self->read_pos_;
    self->clean_pos_ -= self->read_pos_;

    if (self->write_pos_ > 0)
        memmove(self->buf_, self->buf_ + self->read_pos_, self->write_pos_);

    self->read_pos_ = 0;
}

void * TPfIrqWriteBufWriteBegin( TPfIrqWriteBuf *self, Int32 *free_size, Int32 pack_size )
{
    // 如果已经有数据读出,且当前不在读取中,则把未读取的数据移动到缓冲区最前面
    if (pack_size > 0 && self->read_pos_ >= pack_size && !self->reading_)
        TPfIrqWriteBufPack(self);

    *free_size = self->buf_size_ - self->write_pos_;

    return self->buf_ + self->write_pos_;
}

void TPfIrqWriteBufWriteEnd( TPfIrqWriteBuf *self, Int32 data_size )
{
    if (data_size > 0)
        self->write_pos_ += data_size;
}

void * TPfIrqWriteBufReadBegin( TPfIrqWriteBuf *self, Int32 *data_size )
{
    self->reading_ = True;  // 保证中断中不会修改read_pos_和clean_pos_

    Int32 pos = self->write_pos_;  // 这个变量不要优化去掉

    *data_size = pos - self->read_pos_;
    self->clean_pos_ = pos;
    
    return self->buf_ + self->read_pos_;
}

void TPfIrqWriteBufReadEnd( TPfIrqWriteBuf *self, Int32 data_size )
{
    if (data_size > 0)
        self->read_pos_ += data_size;

    self->reading_ = False;
}

Bool TPfIrqWriteBufDirty(TPfIrqWriteBuf *self)
{
    return self->write_pos_ > self->clean_pos_;
}

void TPfIrqWriteBufClear( TPfIrqWriteBuf *self )
{
    self->write_pos_ = 0;
    self->read_pos_ = 0;
    self->clean_pos_ = 0;
}

Int32 TPfIrqWriteBufMakeRoom( TPfIrqWriteBuf *self, Int32 room_size )
{
    Int32 result;
    Int32 free_size = self->buf_size_ - self->write_pos_;

    if (free_size >= room_size)
    {
        result = room_size;
    }
    else if (self->reading_)
    {
        // 在读取中,不能修改读取位置,故有多少空间返回多少
        result = free_size;
    }
    else if (room_size >= self->buf_size_)
    {
        // 需要的空间比总的空间还要大
        TPfIrqWriteBufClear(self);
        result = self->buf_size_;
    }
    else 
    {
        // 已经读取的+后面可写入的大小足够,只需要压缩空间即可,否则还需要丢弃一些待读取的数据
        if (self->read_pos_ + free_size < room_size)
            self->read_pos_ = room_size - free_size;

        TPfIrqWriteBufPack(self);
        result = room_size;
    }

    return result;
}

Int32 TPfIrqWriteBufSize(TPfIrqWriteBuf *self)
{
    return self->buf_size_;
}

Bool TPfIrqWriteBufReadable(TPfIrqWriteBuf *self)
{
    return self->write_pos_ > self->read_pos_;
}

TPfIrqWriteBuf * PfCreateIrqWriteBuf( void *buf, Int32 buf_size )
{
    TPfIrqWriteBuf *result = (TPfIrqWriteBuf *)OfCreateObject(sizeof(*result));
    TPfIrqWriteBufCreate(result, buf, buf_size);

    return result;
}

//--------------------------------------------------------------------------------
static void TPfIrqReadBufDestroy(TOfObject *self)
{
    ((TPfIrqReadBuf *)self)->Destroy_(self);
}

void TPfIrqReadBufCreate( TPfIrqReadBuf *self, void *buf, Int32 buf_size )
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TPfIrqReadBufDestroy);

    self->buf_ = (char *)buf;
    self->buf_size_ = buf_size;
    self->write_pos_ = 0;
    self->read_pos_ = 0;
    self->writing_ = False;
}

void * TPfIrqReadBufWriteBegin( TPfIrqReadBuf *self, Int32 *free_size )
{
    self->writing_ = True;  // 保证中断中不会修改write_pos_和clean_pos_

    *free_size = self->buf_size_ - self->write_pos_;

    return self->buf_ + self->write_pos_;

}

void TPfIrqReadBufWriteEnd( TPfIrqReadBuf *self, Int32 data_size )
{
    if (data_size > 0)
        self->write_pos_ += data_size;

    self->writing_ = False;
}

void * TPfIrqReadBufReadBegin( TPfIrqReadBuf *self, Int32 *data_size )
{
    *data_size = self->write_pos_ - self->read_pos_;
    return self->buf_ + self->read_pos_;
}

void TPfIrqReadBufReadEnd( TPfIrqReadBuf *self, Int32 data_size, Int32 pack_size )
{
    if (data_size > 0)
        self->read_pos_ += data_size;

    if (!self->writing_ && self->read_pos_ > 0)
    {
        if (self->read_pos_ >= self->write_pos_)  // 这里应该是不能>的,只有==
        {
            // 数据用尽,直接移动
            self->write_pos_ = 0;
            self->read_pos_ = 0;
        } 
        else if (self->read_pos_ >= pack_size)
        {
            self->write_pos_ -= self->read_pos_;
            memmove(self->buf_, self->buf_ + self->read_pos_, self->write_pos_);
            self->read_pos_ = 0;
        }
    }
}

void TPfIrqReadBufClear( TPfIrqReadBuf *self )
{
    self->write_pos_ = 0;
    self->read_pos_ = 0;
}

TPfIrqReadBuf * PfCreateIrqReadBuf( void *buf, Int32 buf_size )
{
    TPfIrqReadBuf *result = (TPfIrqReadBuf *)OfCreateObject(sizeof(*result));
    TPfIrqReadBufCreate(result, buf, buf_size);

    return result;
}

Int32 TPfIrqReadBufSize(TPfIrqReadBuf *self)
{
    return self->buf_size_;
}

Bool TPfIrqReadBufWriteable(TPfIrqReadBuf *self)
{
    return self->buf_size_ > self->write_pos_;
}
