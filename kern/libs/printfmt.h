#ifndef __LIBS_PRINT_FMT_H__
#define __LIBS_PRINT_FMT_H__

#include <defs.h>
#include <loongarch.h>
#include <error.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>



void vprintfmt(void (*putch)(int, void*, int), int fd, void *putdat, const char *fmt, va_list ap);
//int vsnprintf(char *str, size_t size, const char *fmt, va_list ap);
#endif    /* !__LIBS_PRINT_FMT_H__ */