/**
 *@file ofc_string.h
 *@brief 定义标准库常用的跟字符串处理,内存复制等相关函数. 
 *@note ofc库并没有实现这些函数的功能.
 * @author zgq
 * @date 2021-10-24
 * @version 1.0

 * @par          修改日志
 * <table>
 * <tr><th>Date        <th>Version               <th>Author          <th>Description
 * <tr><td>2021-10-24  <td>1.0                   <td>zgq             <td>创建初始版本
  * @copyright<mrr_zhang@qq.com>
 */

#ifndef _OFC_STRING_H_
#define _OFC_STRING_H_

#include "ofc_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *memcpy(void *dst, const void *src, TSize size);
void *memmove(void *dst, const void *src, TSize size);
char *strcpy(char *dst, const char *src);
char *strncpy(char *dst, const char *src, TSize size);
char *strcat(char *dst, const char *src);
char *strncat(char *dst, const char *src, TSize size);
int memcmp(const void *dst, const void *src, TSize size);
int strcmp(const char *dst, const char *src);
int strncmp(const char *dst, const char *src, TSize size);
int strcoll(const char *dst, const char *src);
TSize strxfrm(char *dst, const char *src, TSize size);
void *memchr(const void *s, int c, TSize size);
char *strchr(const char *s, int c);
TSize strcspn(const char *dst, const char *src);
char *strpbrk(const char *dst, const char *src);
char *strrchr(const char *s, int c);
TSize strspn(const char *dst, const char *src);
char *strstr(const char *dst, const char *src);
char *strtok(char *dst, const char *src);
void *memset(void *s, int c, TSize size);
char *strerror(int errnum);
TSize strlen(const char *s);
TSize memicmp(const void *buf1, const void *buf2, TSize count);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _OFC_STRING_H_
