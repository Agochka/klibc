/*
 * stdio.h
 */

#ifndef _STDIO_H
#define _STDIO_H

#include <extern.h>
#include <stdarg.h>
#include <stddef.h>

#define stdin  0
#define stdout 1
#define stderr 2

__extern int fputs(const char *s, int fd);

__extern int printf(const char *, ...);
__extern int vprintf(const char *, va_list);
__extern int sprintf(char *, const char *, ...);
__extern int vsprintf(char *, const char *, va_list);
__extern int snprintf(char *, size_t n, const char *, ...);
__extern int vsnprintf(char *, size_t n, const char *, va_list);

#endif /* _STDIO_H */
