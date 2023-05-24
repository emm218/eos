#ifndef _KPRINT_H
#define _KPRINT_H

#include <stdarg.h>

#define KPRINTF_BUFSIZE (sizeof(long long int) * 8 + 2)

int kprintf(const char *, ...) __attribute__((format(printf, 1, 2)));
int kvprintf(const char *, va_list);

#endif
