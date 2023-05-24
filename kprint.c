#include <sys/types.h>

#include <stdint.h>
#include <string.h>

#include "console.h"
#include "kprint.h"

/*
 * macros for converting digits to letters and vice versa
 */
#define to_digit(c) ((c) - '0')
#define is_digit(c) ((unsigned)to_digit(c) <= 9)
#define to_char(n) ((n) + '0')

#define UARG()                                                      \
	(flags & LONG_LONG_INT	 ? va_arg(ap, unsigned long long) : \
		flags & LONG_INT ? va_arg(ap, unsigned long) :      \
		flags & SIZE_INT ? va_arg(ap, size_t) :             \
				   va_arg(ap, unsigned int))

#define SARG()                                             \
	(flags & LONG_LONG_INT	 ? va_arg(ap, long long) : \
		flags & LONG_INT ? va_arg(ap, long) :      \
		flags & SIZE_INT ? va_arg(ap, ssize_t) :   \
				   va_arg(ap, int))
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

// flags for kprintf

#define HEX_PREFIX 0x01
#define ZERO_PAD 0x02
#define LONG_INT 0x04
#define LONG_LONG_INT 0x08
#define SIZE_INT 0x10

static const char *xdigs = "0123456789abcdef";

int
kvprintf(const char *fmt0, va_list ap)
{
	char *fmt, *cp;
	int c, n;

	uint64_t ulong;
	enum { DEC, HEX } base;
	int flags, prec, rlsz, size, width;
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
	flags = 0;
rflag:
	c = *fmt++;
reswitch:
	switch (c) {
	case '0':
		flags |= ZERO_PAD;
		goto rflag;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		n = 0;
		do {
			n = 10 * n + to_digit(c);
			c = *fmt++;
		} while (is_digit(c));
		width = n;
		goto reswitch;
	case '.':
		n = 0;
		while (is_digit(c = *(++fmt))) {
			n = 10 * n + to_digit(c);
		}
		prec = n < 0 ? -1 : n;
		goto reswitch;
	case '+':
		sign = '+';
		goto rflag;
	case 'l':
		if (*fmt == 'l') {
			fmt++;
			flags |= LONG_LONG_INT;
		} else
			flags |= LONG_INT;
		goto rflag;
	case 'z':
		flags |= SIZE_INT;
		goto rflag;
	case 'd':
		ulong = SARG();
		if ((long long)ulong < 0) {
			ulong = -ulong;
			sign = '-';
		}
		base = DEC;
		goto number;
	case 'p':
		ulong = (uint64_t)va_arg(ap, void *);
		base = HEX;
		flags |= HEX_PREFIX;
		goto nosign;
	case 'u':
		ulong = UARG();
		base = DEC;
		goto nosign;
	case 'x':
		ulong = UARG();
		base = HEX;
	nosign:
		sign = '\0';
	number:
		if (prec >= 0)
			flags &= ~ZERO_PAD;
		cp = buf + KPRINTF_BUFSIZE;
		if (ulong != 0 || prec != 0) {
			switch (base) {
			case DEC:
				while (ulong >= 10) {
					*--cp = to_char(ulong % 10);
					ulong /= 10;
				}
				*--cp = to_char(ulong % 10);
				break;
			case HEX:
				do {
					*--cp = xdigs[ulong & 0xF];
					ulong >>= 4;
				} while (ulong);
				break;
			}
		}
		size = buf + KPRINTF_BUFSIZE - cp;
		break;
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
		} else
			size = strlen(cp);
		sign = '\0';
		break;
	default:
		if (c == '\0')
			goto done;
		cp = buf;
		*cp = c;
		size = 1;
		sign = '\0';
		break;
	}

	rlsz = prec > size ? prec : size;

	if (sign)
		rlsz++;
	else if (flags & HEX_PREFIX)
		rlsz += 2;

	if (!(flags & ZERO_PAD)) {
		for (n = width - rlsz; n > 0; n--)
			conputc(' ');
	}

	if (sign)
		conputc(sign);
	else if (flags & HEX_PREFIX) {
		conputc('0');
		conputc('x');
	}

	if (flags & ZERO_PAD) {
		for (n = width - rlsz; n > 0; n--)
			conputc('0');
	}

	for (n = prec - size; n > 0; n--)
		conputc('0');

	while (size--)
		conputc(*cp++);

	goto loop;
done:
	return 0;
}
