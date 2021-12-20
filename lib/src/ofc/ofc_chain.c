#include "ofc_chain.h"

/**
 * 链表节点析构函数
 * @param sender: 对象指针
 */
static void TOfChainNodeDestroy(TOfObject *sender)
{
    TOfChainNode *self = (TOfChainNode *)sender;

    if (self->Destroy_ != NULL)
        self->Destroy_(sender);
}

void TOfChainNodeCreate( TOfChainNode *self, TOfChain *owner, void *data )
{
    TOfObjectCreate(&self->parent_);

    OVERRIDE_METHOD(TOfObject, Destroy, TOfChainNodeDestroy);

    self->owner_ = owner;
    self->data_ = data;
    self->prev_ = NULL;
    self->next_ = NULL;
    self->freeing_ = False;
}

TOfChain * TOfChainNodeOwner( TOfChainNode *self )
{
    return self->owner_;
}

TOfChainNode * TOfChainNodePrev( TOfChainNode *self )
{
    return self->prev_;
}

TOfChainNode * TOfChainNodeNext( TOfChainNode *self )
{
    return self->next_;
}

void * TOfChainNodeData( TOfChainNode *self )
{
    return self->data_;
}

void TOfChainNodeSetData( TOfChainNode *self, void *data )
{
    self->data_ = data;
}

/**
 * 添加下一个结点
 * @param self: 对象指针
 * @param next: 下一项的结点
 * @return 增添是否成功
 */
static Bool TOfChainNodeAdd(TOfChainNode *self, TOfChainNode *next)
{
    if (next != NULL && next != self)
    {
        next->prev_ = self;
        next->next_ = self->next_;

        if (self->next_ != NULL)
            self->next_->prev_ = next;

        self->next_ = next;

        return True;
    }
    else
        return False;
}

/**
 * 插入上一个结点
 * @param self: 对象指针
 * @param prev: 上一个结点
 * @return 插入是否成功
 */
static Bool TOfChainNodeInsert(TOfChainNode *self, TOfChainNode *prev)
{
    if (prev != NULL && prev != self)
    {
        if (self->prev_ != NULL)
            self->prev_->next_ = prev;

        prev->next_ = self;
        prev->prev_ = self->prev_;
        self->prev_ = prev;

        return True;
    }
    else
        return False;
}

/**
 * 移除当前结点
 * @param self: 对象指针
 * @return
 */
static void TOfChainNodeRemove(TOfChainNode *self)
{
    if (self->prev_ != NULL)
        self->prev_->next_ = self->next_;

    if (self->next_ != NULL)
        self->next_->prev_ = self->prev_;

    self->prev_ = NULL;
    self->next_ = NULL;
}

static void TOfChainDestroy(TOfObject *sender)
{
    TOfChain *self = (TOfChain *)sender;

    TOfChainClear(self);

    self->Lock_.Method = NULL;
    self->Lock_.object = NULL;
    self->Unlock_.Method = NULL;
    self->Unlock_.object = NULL;

    if (self->Destroy_ != NULL)
    {
        self->Destroy_(sender);
    }
}

static void TOfChainEmptyMethod(void *sender)
{

}


#define TOfChainLock(self)  self->Lock_.Method(self->Lock_.object);
#define TOfChainUnlock(self)  self->Unlock_.Method(self->Unlock_.object)


void TOfChainCreate( TOfChain *self, TOfChainOnLock *lock, TOfChainOnLock *unlock )
{
    TOfObjectCreate(&self->parent_);

    OVERRIDE_METHOD(TOfObject, Destroy, TOfChainDestroy);

    if (lock != NULL && lock->Method != NULL && unlock != NULL && unlock->Method != NULL)
    {
        self->Lock_ = *lock;
        self->Unlock_ = *unlock;
    }
    else
    {
        self->Lock_.Method = TOfChainEmptyMethod;
        self->Lock_.object = (void *)self;
        self->Unlock_.Method = TOfChainEmptyMethod;
        self->Unlock_.object = (void *)self;
    }

    // 初始化
    self->data_ = NULL;
    self->head_ = NULL;
    self->tail_ = NULL;
    self->count_ = 0;

    self->OnDeletion.Method = NULL;
    self->OnDeletion.object = NULL;
}

void * TOfChainData( TOfChain *self )
{
    return self->data_;
}

Int32 TOfChainCount( TOfChain *self )
{
    return self->count_;
}

void TOfChainSetData( TOfChain *self, void *data )
{
    self->data_ = data;
}



void TOfChainClear( TOfChain *self )
{
    TOfChainNode      *next_node;
    TOfChainNode      *head_node = NULL;
    TOfChainOnDeletion on_deletion = self->OnDeletion;

    TOfChainLock(self);
    {
        head_node = self->head_;
        self->count_ = 0;
        self->head_ = NULL;
        self->tail_ = NULL;
        next_node = head_node;

        /// 优先进行变量赋值，提高效率
        while (next_node != NULL)
        {
            next_node->freeing_ = True;
            next_node = TOfChainNodeNext(next_node);
        }
    }
    TOfChainUnlock(self);

    if (head_node == NULL)
        ;
    else if (on_deletion.Method != NULL)
    {
        do
        {
            next_node = TOfChainNodeNext(head_node);
            TOfChainNodeRemove(head_node);
            on_deletion.Method(on_deletion.object, head_node);
            OfDestroyObject((TOfObject *)head_node);
            head_node = next_node;
        } while (head_node != NULL);
    }
    else
    {
        do
        {
            next_node = TOfChainNodeNext(head_node);
            TOfChainNodeRemove(head_node);
            OfDestroyObject((TOfObject *)head_node);
            head_node = next_node;
        } while (head_node != NULL);
    }
}

TOfChainNode * TOfChainAddData( TOfChain *self, void *data )
{
    TOfChainNode *add_node = (TOfChainNode *)OfMalloc(sizeof(TOfChainNode));

    if (add_node != NULL)
    {
        TOfChainNodeCreate(add_node, self, data);

        return TOfChainAddNode(self, add_node);
    }
    else
        return NULL;
}

TOfChainNode * TOfChainAddNode( TOfChain *self, TOfChainNode *node )
{
    TOfChainNode *result = NULL;
    TOfChainNode *new_node = node;

    TOfChainLock(self);

    if (new_node != NULL && new_node->owner_ == self && !new_node->freeing_)
    {
        if (self->head_ == NULL)
        {
            self->head_ = new_node;
            self->tail_ = new_node;
            result = new_node;
            self->count_ = 1;
        }
        else
        {
            if (TOfChainNodeAdd(self->tail_, new_node))
            {
                self->tail_ = new_node;
                result = new_node;
                self->count_++;
            }
        }
    }

    TOfChainUnlock(self);

    return result;
}

TOfChainNode * TOfChainInsertData( TOfChain *self, void *data, TOfChainNode *node )
{
    TOfChainNode *new_node = (TOfChainNode *)OfMalloc(sizeof(TOfChainNode));
    
    if (new_node != NULL)
    {
        TOfChainNodeCreate(new_node, self, data);

        return TOfChainInsertNode(self, new_node, node);
    }
    else
    {
        return NULL;;
    }
}

TOfChainNode * TOfChainInsertNode( TOfChain *self, TOfChainNode *insert_node, TOfChainNode *node )
{
    TOfChainNode *result = NULL;
    TOfChainNode *new_node = insert_node;

    TOfChainLock(self);

    if (new_node != NULL && !new_node->freeing_)
    {
        if (self->head_ == NULL)
        {
            self->head_ = new_node;
            self->tail_ = new_node;
            result = new_node;
            self->count_ = 1;
        }
        else
        {
            /// 如果为NULL，则在头部插入，这里不需要判断返回值
            if (node == NULL || node->owner_ != self || node->freeing_)
            {
                TOfChainNodeInsert(self->head_, new_node);
                self->head_ = new_node;
            }
            else
            {
                TOfChainNodeInsert(node, new_node);

                if (self->head_ == node)
                    self->head_ = new_node;
            }

            result = new_node;
            self->count_++;
        }
    }

    TOfChainUnlock(self);

    return result;
}

static Bool TOfChainDoFindNode(TOfChain *self, TOfChainNode *node)
{
    Bool result = False;
    TOfChainNode *next_node = self->head_;

    if (node != NULL && node->owner_ == self && !node->freeing_)
    {
        while (next_node != NULL)
        {
            if (next_node == node)
            {
                result = True;
                break;
            }
            else
                next_node = TOfChainNodeNext(next_node);
        }
    }

    return result;
}

static TOfChainNode *TOfChainDoFindDataNormal(TOfChain *self, void *data)
{
    TOfChainNode *result = NULL;
    TOfChainNode *next_node = self->head_;

    while (next_node != NULL)
    {
        if (TOfChainNodeData(next_node) == data)
        {
            result = next_node;
            break;
        }
        else
            next_node = TOfChainNodeNext(next_node);
    }

    // 返回结果
    return result;
}

static TOfChainNode *TOfChainDoFindData(TOfChain *self, void *data)
{
    return TOfChainDoFindDataNormal(self, data);
}

Bool TOfChainDeleteData( TOfChain *self, void *data )
{
    TOfChainNode *find_node = NULL;

    TOfChainLock(self);

    find_node = TOfChainDoFindData(self, data);

    if (find_node != NULL)
    {
        self->count_--;

        if (self->head_ == find_node)
            self->head_ = TOfChainNodeNext(find_node);

        if (self->tail_ == find_node)
            self->tail_ = TOfChainNodePrev(find_node);

        find_node->freeing_ = True;
        TOfChainNodeRemove(find_node);
    }

    TOfChainUnlock(self);

    if (find_node == NULL)
        return False;
    else if (self->OnDeletion.Method != NULL)
        (self->OnDeletion.Method)(self->OnDeletion.object, find_node);

    OfDestroyObject((TOfObject *)find_node);

    return True;
}

void TOfChainDeleteNode( TOfChain *self, TOfChainNode *node )
{
    TOfChainLock(self);

    if (node != NULL && TOfChainDoFindNode(self, node))
    {
        self->count_--;

        if (self->head_ == node)
            self->head_ = TOfChainNodeNext(node);

        if (self->tail_ == node)
            self->tail_ = TOfChainNodePrev(node);

        node->freeing_ = True;
        TOfChainNodeRemove(node);
    }

    TOfChainUnlock(self);

    if (node != NULL)
    {
        if (self->OnDeletion.Method != NULL)
            (self->OnDeletion.Method)(self->OnDeletion.object, node);

        OfDestroyObject((TOfObject *)node);
    }
}

void TOfChainRemoveNode( TOfChain *self, TOfChainNode *node )
{
    TOfChainLock(self);

    if (node != NULL && TOfChainDoFindNode(self, node))
    {
        self->count_--;

        if (self->head_ == node)
            self->head_ = TOfChainNodeNext(node);

        if (self->tail_ == node)
            self->tail_ = TOfChainNodePrev(node);

        TOfChainNodeRemove(node);
    }

    TOfChainUnlock(self);
}

Bool TOfChainExistedData( TOfChain *self, void *data )
{
    return TOfChainFindData(self, data) != NULL;
}

TOfChainNode * TOfChainFindData( TOfChain *self, void *data )
{
    TOfChainNode *result;

    TOfChainLock(self);
    result = TOfChainDoFindData(self, data);
    TOfChainUnlock(self);

    return result;
}

Bool TOfChainFindNode( TOfChain *self, TOfChainNode *node )
{
    Bool result;

    TOfChainLock(self);
    result = TOfChainDoFindNode(self, node);
    TOfChainUnlock(self);

    return result;
}

TOfChainNode * TOfChainFirstNode( TOfChain *self )
{
    return self->head_;
}

TOfChainNode * TOfChainLastNode( TOfChain *self )
{
    return self->tail_;
}

TOfChainNode * TOfChainPrevNode( TOfChain *self, TOfChainNode *node )
{
    TOfChainNode *result = NULL;

    TOfChainLock(self);

    if (node == NULL)
        result = self->tail_;
    else if (!node->freeing_)
        result = TOfChainNodePrev(node);

    TOfChainUnlock(self);

    return result;
}

TOfChainNode * TOfChainNextNode( TOfChain *self, TOfChainNode *node )
{
    TOfChainNode *result = NULL;

    TOfChainLock(self);

    if (node == NULL)
        result = self->head_;
    else if (!node->freeing_)
        result = TOfChainNodeNext(node);

    TOfChainUnlock(self);

    return result;
}

TOfChain *OfCreateChain( TOfChainOnLock *lock, TOfChainOnLock *unlock )
{
    TOfChain *result = (TOfChain *)OfCreateObject(sizeof(*result));

    if (result != NULL)
        TOfChainCreate(result, lock, unlock);

    return result;
}
