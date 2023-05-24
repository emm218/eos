#include <string.h>

#include "console.h"
#include "kprint.h"

int
kprintf(const char *fmt, ...)
{
	int ret;
	va_list ap;

	va_start(ap, fmt);
	ret = kvprintf(fmt, ap);
	va_end(ap);
	return ret;
}

int
kvprintf(const char *fmt0, va_list ap)
{
	char *fmt, *cp;
	int c, n;

	int prec, size;
	char sign;
	char buf[KPRINTF_BUFSIZE];

	fmt = (char *)fmt0;
loop:
	while (*fmt != '%' && *fmt) {
		conputc(*fmt++);
	}
	if (*fmt == 0)
		goto done;
	fmt++;

	sign = '\0';
	prec = -1;

	c = *fmt++;
	/* reswitch: */
	switch (c) {
	case 's':
		if ((cp = va_arg(ap, char *)) == NULL)
			cp = "(null)";
		if (prec >= 0) {
			char *p = memchr(cp, 0, prec);
			if (p != NULL) {
				size = p - cp;
				if (size > prec)
					size = prec;
			} else
				size = prec;
		}
	default:
		if (c == '\0')
			goto done;
		cp = buf;
		*cp = c;
		size = 1;
		sign = '\0';
		break;
	}

	if (sign)
		size++;

	if (sign)
		conputc(sign);

	for (n = prec - size; n > 0; n--)
		conputc('0');

	while (size--)
		conputc(*cp++);

	goto loop;
done:
	return 0;
}
