#ifndef _KPRINT_H
#define _KPRINT_H

#include <stdarg.h>

#define STR(x)	XSTR(x)
#define XSTR(x) #x

#define PANIC(...) __panic(__FILE__ ":" STR(__LINE__) "\t" __VA_ARGS__)

#define KPRINTF_BUFSIZE (sizeof(long long int) * 8 + 2)

int kprintf(const char *, ...) __attribute__((format(printf, 1, 2)));
int kvprintf(const char *, va_list);
void __panic(const char *, ...) __attribute__((format(printf, 1, 2)));

#endif
