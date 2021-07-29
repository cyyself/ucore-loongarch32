#ifndef __LIBS_STDIO_H__
#define __LIBS_STDIO_H__

#include <defs.h>
#include <stdarg.h>

/* kern/libs/stdio.c */
int vcprintf(const char *fmt, va_list ap);
int cprintf(const char *fmt, ...);
void cputchar(int c);
int getchar(void);

void printhex(unsigned int x);
void printbase10(int x);

/* kern/libs/readline.c */
char *readline(const char *prompt);


#endif /* !__LIBS_STDIO_H__ */


