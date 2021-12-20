#include "ofc_list.h"
#include "ofc_string.h"

/**
 * 设置队列的容量
 * @param self: 对象指针
 * @param new_capacity: 新容量
 * @return: 设置是否成功
 */
static Bool TOfListSetCapacity(TOfList *self, Int32 new_capacity)
{
    Bool result;

    if (new_capacity != self->capacity_)
    {
        void **new_list;
        
        result = True;

        if (new_capacity > 0)
        {
            Int32 new_size = new_capacity * sizeof(void *);
            Int32 size = self->capacity_ * sizeof(void *);

            new_list = (void **)OfMalloc(new_size);

            if (new_list == NULL)
            {
                result = False;
            }
            else if (new_size > size)
            {
                memcpy(new_list, self->list_, size);
                memset((char *)new_list + size, 0, new_size - size);
            }
            else
                memcpy(new_list, self->list_, new_size);
        }
        else
        {
            new_list = 0;
        }

        if (result)
        {
            if (self->capacity_ > 0)
                OfFree((void *)self->list_);

            self->capacity_ = new_capacity;
            self->list_ = new_list;
        }
    }
    else
        result = False;

    return result;
}

static Bool Grow(TOfList *self)
{
    Int32 delta;

    if (self->capacity_ > 64)
        delta = self->capacity_ >> 2;
    else 
        delta = 16;

    return TOfListSetCapacity(self, self->capacity_ + delta);
}

static void TOfListDestroy( TOfObject *self )
{
    TOfListClear((TOfList *)self);
}

void TOfListCreate( TOfList *self )
{
    memset(self, 0, sizeof(*self));

    TOfObjectCreate(&self->parent_);
    OVERRIDE_METHOD(TOfObject, Destroy, TOfListDestroy);
}

Int32 TOfListAdd( TOfList *self, void *item )
{
    Int32 result = self->count_;
    Bool grow_result = True;

    if (result == self->capacity_)
        grow_result = Grow(self);

    if (grow_result)
    {
        self->list_[result] = item;
        self->count_++;
    }
    else
        result = -1;

    return result;    
}

void * TOfListItems( TOfList *self, Int32 index )
{
    return index >= 0 && index < self->count_ ? self->list_[index] : NULL;
}

void TOfListSetItem( TOfList *self, Int32 index, void *item )
{
    if (index >= 0 && index < self->count_)
    {
        void *temp = self->list_[index];

        self->list_[index] = item;

        if (temp != NULL && self->OnDelete != NULL)
            self->OnDelete(self, temp);        
    }
}

void TOfListClear(TOfList *self)
{
    TOfListSetCount(self, 0);
    TOfListSetCapacity(self, 0);
}

void TOfListDelete( TOfList *self, Int32 index )
{
    if (index >= 0 && index < self->count_)
    {
        void *temp = self->list_[index];
        self->count_--;

        if (index < self->count_)
            memmove(&self->list_[index], &self->list_[index + 1], (self->count_ - index) * sizeof(void *));

        if (self->OnDelete)
            self->OnDelete(self, temp);
    }
}

Int32 TOfListIndexOf( TOfList *self, void *item )
{
    Int32 result = self->count_ - 1;

    while (result >= 0 && self->list_[result] != item)
        result--;

    return result;
}

Bool TOfListInsert( TOfList *self, Int32 index, void *item )
{
    Bool result;

    if (index >= 0 && index <= self->count_)
    {
        result = True;

        if (self->count_ == self->capacity_)
            result = Grow(self);

        if (result)
        {
            if (index < self->count_)
                memmove(&self->list_[index + 1], &self->list_[index], (self->count_ - index) * sizeof(void *));

            self->list_[index] = item;
            self->count_++;
        }
    }
    else
        result = False;

    return result;
}

Int32 TOfListRemove( TOfList *self, void *item )
{
    Int32 result = TOfListIndexOf(self, item);

    if (result >= 0)
        TOfListDelete(self, result);

    return result;
}

Int32 TOfListCount( TOfList *self )
{
    return self->count_;
}

/**
 * 设置队列的个数
 * @param self: 对象指针
 * @param new_count: 个数
 * @return 设置是否成功
 */
Bool TOfListSetCount( TOfList *self, Int32 new_count )
{
    Bool result = True;

    if (new_count < 0)
        new_count = 0;

    if (new_count > self->capacity_)
        result = TOfListSetCapacity(self, new_count);

    if (result)
    {
        if (new_count > self->count_)
            memset(&self->list_[self->count_], 0, (new_count - self->count_) * sizeof(void *));
        else
        {
            Int32 i;

            for (i = self->count_ - 1; i >= new_count; i--)
                TOfListDelete(self, i);
        }

        self->count_ = new_count;
    }
    
    return result;
}

/**
 * 通过快速查找方法找到所对应的索引，需要先排序好后再查找
 * @param self: 对象指针
 * @param item: 所要查找的数据
 * @param compare: 查找时所用的方法
 * @param index: 查找的所对应地址
 * @return 查找是否成功,成功时才允许用index的返回值
 */
Bool TOfListQuickFind( TOfList *self, void *item, TOfListSortCompare compare, Int32 *index )
{
    Bool result = False;
    
    if (compare != NULL && index != NULL)
    {
        Int32 start;
        Int32 end;
        Int32 mid;
        Int32 cmp;

        start = 0;
        end = self->count_ - 1;

        while (start <= end)
        {
            mid = (start + end) >> 1;

            cmp = compare(self->list_[mid], item);

            if (cmp < 0)
                start = mid + 1;
            else
            {
                end = mid - 1;

                if (cmp == 0)
                {
                    result = True;
                    start = mid;
                }
            }
        }

        *index = start;
    }

    return result;
}

static void QuickSort( void ** sort_list, Int32 left, Int32 right, TOfListSortCompare sort_compare )
{
    Int32 i, j;
    void *ptr, *temp;

    do
    {
        i = left;
        j = right;
        ptr = sort_list[(left + right) >> 1];

        do
        {
            while (sort_compare(sort_list[i], ptr) < 0)
                i++;

            while (sort_compare(sort_list[j], ptr) > 0)
                j--;

            if (i <= j)
            {
                temp = sort_list[i];
                sort_list[i] = sort_list[j];
                sort_list[j] = temp;
                i++;
                j--;
            }
        }
        while (i <= j);

        if (left < j)
            QuickSort(sort_list, left, j, sort_compare);

        left = i;
    }
    while (i < right);
}

/**
 * 列表排序
 * @note 内部会递归，小内存平台慎用，递归深度跟内部对象个数有关系
 * @param self: 对象指针
 * @param compare: 排序方法
 * @return 
 */
void TOfListSort( TOfList *self, TOfListSortCompare compare )
{
    if ((self->list_ != NULL) && (self->count_ > 0) && compare != NULL)
        QuickSort(self->list_, 0, self->count_ - 1, compare);
}

void TOfListDeleteEx( TOfList *self, Int32 index, Int32 count )
{
    if (count > 0)
    {
        if (index < 0)
            index = 0;

        if (index + count > self->count_)
            count = self->count_ - index;

        if (count > 0)
        {
            Int32 eof_index = index + count;
            Int32 move_count;
            TOfListOnDelete OnDelete = self->OnDelete;
            Int32 i;

            if (OnDelete != NULL)
            {
                for (i = index; i < eof_index; i++)
                {
                    void *temp = self->list_[i];

                    if (temp != NULL)
                        OnDelete(self, temp);
                }
            }
            

            move_count = self->count_ - eof_index;
            self->count_ -= count;

            if (move_count > 0)
                memmove(&self->list_[index], &self->list_[eof_index], move_count * sizeof(void *));
        }
    }
}

Int32 TOfListCapacity( TOfList *self )
{
    return self->capacity_;
}

TOfList * OfCreateList( void )
{
    TOfList *result = (TOfList *)OfCreateObject(sizeof(*result));
    
    if (result != NULL)
        TOfListCreate(result);

    return result;
}


