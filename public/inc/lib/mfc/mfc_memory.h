// =======================================
// Unit   : memory manager (内存管理单元)
// Version: 4.0.1.0 (build 2018.12.16)
// Author : Kyee Ye
// Email  : kyee_ye(at)126.com
// Copyright (C) Kyee workroom
// =======================================

#ifndef _KYMemory_H_
#define _KYMemory_H_

#ifdef __cplusplus
extern "C"
{
#endif

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 内存分配/释放函数 */

// 分配指定尺寸的内存
void* Malloc(int size);

// 释放已分配的内存
void  Free(void* mem);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

/* 内存管理的相关函数 */

// ---------------- 内存管理使用示例 ----------------

/*
// 1KB 内存项数
#define _1KB_Count1 64

// 索引项的全局占用空间
static T1KBIndex  L_Indexs[_1KB_Count1];

// 内存项的全局占用空间
static T1KBItem   L_Items[_1KB_Count1];

// 内存块列表
static TMemBlock  L_Blocks[] = {{_1KB_Count1, L_Items, L_Indexs}};

// 内存管理的全局加锁
static void L_Lock(void)
{
   // ... ...
}

// 内存管理的全局解锁
static void L_Unlock(void)
{
   // ... ...
}

// 扩展内存分配
static void* L_Malloc(int size)
{
   return malloc(size);
}

// 扩展内存释放
static void L_Free(void* mem)
{
   free(mem);
}

// on-init: 在工程的初始化函数中
{
   // 初始化内存管理
   KYMemory_Init(&L_Lock, &L_Unlock, &L_Malloc, &L_Free,
                  L_Blocks, sizeof(L_Blocks)/sizeof(L_Blocks[0]));

   // 其它初始化
   ... ...
}

// on-fini: 在工程的结束函数中
{
   // 其它释放
   ... ...

   // 释放内存管理
   KYMemory_Fini();
}
*/

// ---------------- 类型定义 ----------------

// 内存管理的内存项/索引项类型
typedef unsigned char T1KBItem[1<<10]; // 1KB 的内存项
typedef unsigned char T1KBIndex[1<<4]; // 1KB 的索引项

// 锁的回调函数指针类型
typedef void  (*TLockFunc)(void);

// 第三方内存分配/释放函数指针类型
typedef void* (*TMalloc)(int size);
typedef void  (*TFree)(void* mem);

// 内存块项结构
typedef struct
{
   int         Count;      // 内存块的 1KB 内存项数
   T1KBItem*   Items;      // 内存块的内存项列表, 项数为: Count
   T1KBIndex*  Indexs;     // 内存块的索引项列表, 项数为: Count
} TMemBlock;

// ---------------- 构造函数和析构函数 ----------------

/* 内存管理初始化
参数:
   lock     加锁函数
   unlock   解锁函数
   do_malloc第三方内存分配函数, 注: 不允许为当前的 Malloc 函数
   do_free  第三方内存释放函数, 注: 不允许为当前的 Free 函数
   blocks   内存块列表, 列表项数为: count
   count    内存块的项数
返回值:
   >0       初始化成功, 返回内存块的项数
    0       初始化失败, 无合法内存块
   -1       初始化失败, 参数不合法
   -2       初始化失败, 表示已初始化
   -3       初始化失败, 表示正在初始化
*/
int   KYMemory_Init(TLockFunc  lock,  TLockFunc unlock,
                    TMalloc    do_malloc, TFree do_free,
                    TMemBlock* blocks,      int count);

// 内存管理释放
void  KYMemory_Fini(void);

// ---------------- 属性方法 ----------------

// 1KB 内存的总项数, 若未初始化则返回值为 -1
int   KYMemory_1KBCount(void);

// 内存的总单元个数(注: 每个单元 16 字节), 若未初始化则返回值为 -1
int   KYMemory_CellCount(void);

// 内存的已分配单元个数(注: 每个单元 16 字节), 若未初始化则返回值为 -1
int   KYMemory_UsedCount(void);

// 内存块列表的项数, 若未初始化则返回值为 -1
int   KYMemory_BlockCount(void);

#ifdef __cplusplus
}     // end extern "C"
#endif

#endif
