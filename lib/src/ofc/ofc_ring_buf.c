#include "ofc_ring_buf.h"
#include "ofc_string.h"

static void TOfRingBufDestroy(TOfObject *self)
{
    ((TOfRingBuf *)self)->Destroy_(self);
}

void TOfRingBufCreate( TOfRingBuf *self, void *buf, Int32 buf_size )
{
    TOfObjectCreate((TOfObject *)self);
    OVERRIDE_METHOD(TOfObject, Destroy, TOfRingBufDestroy);

    self->buf_ = (char *)buf;
    self->write_pos_ = (char *)buf;
    self->read_pos_ = (char *)buf;
    self->end_pos_ = (char *)buf + buf_size;
    self->write_size_ = 0;
    self->read_size_ = 0;
}

Int32 TOfRingBufWrite( TOfRingBuf *self, const void *data, Int32 data_size )
{
    Int32 result;
    Int32 free_size = (self->end_pos_ - self->buf_) - (self->write_size_ - self->read_size_); //self->data_size_; 

    if (data != NULL && data_size > 0 && free_size > 0)
    {
        // 写入位置到缓冲区末尾的空闲大小
        Int32 size = self->end_pos_ - self->write_pos_;

        result = data_size >= free_size ? free_size : data_size;  // 可以写入的数据数

        if (size > result)  // write_pos_到末尾足够放下所有数据
            size = result;

        memmove(self->write_pos_, data, size);

        self->write_pos_ += size;
        if (self->write_pos_ >= self->end_pos_) // 实际上只有==
            self->write_pos_ = self->buf_;

        data_size = result - size;
        if (data_size > 0)  // 还有数据没有写入
        {
            memmove(self->write_pos_, (char*)data + size, data_size);
            self->write_pos_ += data_size;
        }

        self->write_size_ += result;
    }
    else
        result = 0;

    return result;
}

Int32 TOfRingBufRead( TOfRingBuf *self, void *buf, Int32 buf_size )
{
    Int32 result;
    Int32 data_size = self->write_size_ - self->read_size_; // self->data_size_;

    if (buf != NULL && buf_size > 0 && data_size > 0)
    {
        Int32 size = self->end_pos_ - self->read_pos_;  // 读取点到末尾的大小
        result = buf_size >= data_size ? data_size : buf_size; // 最大可以读取长度

        if (size > result)  // 读取点到末尾有足够的数据
            size = result;

        memmove(buf, (const void *)self->read_pos_, size);

        self->read_pos_ += size;
        if (self->read_pos_ >= self->end_pos_)
            self->read_pos_ = self->buf_;

        data_size = result - size;
        if (data_size > 0)  // 还有数据需要读取
        {            
            memmove((char *)buf + size, self->read_pos_, data_size);
            self->read_pos_ += data_size;
        }

        self->read_size_ += result;
    }
    else
        result = 0;

    return result;
}

Int32 TOfRingBufPeek( TOfRingBuf *self, void *buf, Int32 buf_size )
{
    Int32 result;
    Int32 data_size = self->write_size_ - self->read_size_; // self->data_size_;

    if (buf != NULL && buf_size > 0 && data_size > 0)
    {
        Int32 size = self->end_pos_ - self->read_pos_;  // 读取点到末尾的大小
        result = buf_size >= data_size ? data_size : buf_size; // 最大可以读取长度

        if (size > result)  // 读取点到末尾有足够的数据
            size = result;

        memmove(buf, (const void *)self->read_pos_, size);

        data_size = result - size;
        if (data_size > 0)  // 还有数据需要读取
            memmove((char *)buf + size, self->buf_, data_size);
    }
    else
        result = 0;

    return result;
}

void TOfRingBufClear( TOfRingBuf *self )
{
    Int32 data_size = self->write_size_ - self->read_size_; 

    if (data_size > 0)
    {
        Int32 size = self->end_pos_ - self->read_pos_;  // 读取点到末尾的大小

        if (size > data_size)  // 数据全部在读取点之后
            self->read_pos_ += data_size;
        else  // 数据刚好到末尾或者已经拐弯
            self->read_pos_ = self->buf_ + (data_size - size);  

        self->read_size_ += data_size;
    }
}

Int32 TOfRingBufFreeSize( TOfRingBuf *self )
{
    return (self->end_pos_ - self->buf_) - (self->write_size_ - self->read_size_);// self->data_size_;
}

Int32 TOfRingBufDataSize( TOfRingBuf *self )
{
    return self->write_size_ - self->read_size_; // self->data_size_;
}

Int32 TOfRingBufSize( TOfRingBuf *self )
{
    return self->end_pos_ - self->buf_; 
}


const char * TOfRingBufGetReadPos( TOfRingBuf *self, Int32 *readable_size )
{
    Int32 data_size = self->write_size_ - self->read_size_;

    if (readable_size != NULL && data_size > 0)
    {
        Int32 size = self->end_pos_ - self->read_pos_;  // 读取点到末尾的大小
        *readable_size = size < data_size ? size : data_size;
    }

    return data_size > 0 ? self->read_pos_ : NULL;
}

void TOfRingBufPop( TOfRingBuf *self, Int32 size )
{
    Int32 data_size = self->write_size_ - self->read_size_; 

    if (data_size > 0 && size > 0)
    {
        if (size < data_size)
            data_size = size;

        size = self->end_pos_ - self->read_pos_;  // 读取点到末尾的大小

        if (size > data_size)  // 数据全部在读取点之后
            self->read_pos_ += data_size;
        else  // 数据刚好到末尾或者已经拐弯
            self->read_pos_ = self->buf_ + (data_size - size);  

        self->read_size_ += data_size;
    }
}

char * TOfRingBufGetWritePos( TOfRingBuf *self, Int32 *writeable_size )
{
    Int32 free_size = (self->end_pos_ - self->buf_) - (self->write_size_ - self->read_size_); //self->data_size_; 

    if (writeable_size != NULL && free_size > 0)
    {
        Int32 size = self->end_pos_ - self->write_pos_;  // 写入位置到缓冲区末尾的空闲大小
        *writeable_size = (size < free_size) ? size : free_size;
    }

    return free_size > 0 ? self->write_pos_ : NULL;
}

void TOfRingBufPush( TOfRingBuf *self, Int32 size )
{
    Int32 free_size = (self->end_pos_ - self->buf_) - (self->write_size_ - self->read_size_); //self->data_size_; 

    if (size > 0 && free_size > 0)
    {
        if (size < free_size)
            free_size = size;

        // 写入位置到缓冲区末尾的空闲大小
        size = self->end_pos_ - self->write_pos_;

        if (size > free_size)
            self->write_pos_ += free_size;
        else
            self->write_pos_ = self->buf_ + (free_size - size);

        self->write_size_ += free_size;
    }
}

TOfRingBuf *OfCreateRingBuf( void *buf, Int32 buf_size )
{
    TOfRingBuf *result = NULL;

    if (buf != NULL && buf_size > 0)
    {
        result = (TOfRingBuf *)OfCreateObject(sizeof(*result));

        if (result != NULL)
            TOfRingBufCreate(result, buf, buf_size);
    }

    return result;
}
