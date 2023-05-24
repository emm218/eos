#include <stddef.h>

const void *
memchr(const void *ptr, int value, size_t num)
{
	return __builtin_memchr(ptr, value, num);
}

size_t
strlen(const char *str)
{
	return __builtin_strlen(str);
}
