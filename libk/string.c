#include <stddef.h>

const void *
memchr(const void *ptr, int value, size_t num)
{
	return __builtin_memchr(ptr, value, num);
}
