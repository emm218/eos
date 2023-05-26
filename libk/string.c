#include <stddef.h>

const void *
memchr(const void *ptr, int c, size_t n)
{
	const unsigned char *s = ptr;
	c = (unsigned char)c;
	for (; n && *s != c; s++, n--)
		;
	return n ? (void *)s : 0;
}

size_t
strlen(const char *s)
{
	const char *a;
	for (a = s; *s; s++)
		;
	return s - a;
}
