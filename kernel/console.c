#include <stddef.h>
#include <stdint.h>

#include "bootboot.h"
#include "console.h"

#define WIDTH 8 // psf 1.0 fonts are always 8 wide

#define TAB_STOP 8

struct psf {
	uint16_t magic;
	uint8_t flags;
	uint8_t char_size;
	uint8_t glyphs[];
} __attribute__((packed));

extern unsigned char _binary_ter_n_psf_start;
extern unsigned char _binary_ter_b_psf_start;
extern BOOTBOOT bootboot;
extern uint8_t fb;
uint8_t *fb_ptr = &fb;

static struct psf *normal = (struct psf *)&_binary_ter_n_psf_start;
static struct psf *bold = (struct psf *)&_binary_ter_b_psf_start;
static struct psf *font = (struct psf *)&_binary_ter_n_psf_start;

static void console_scroll();

#define BLACK	0x000000
#define RED	0xFF0000
#define GREEN	0x00FF00
#define YELLOW	0xFFFF00
#define BLUE	0x0000FF
#define MAGENTA 0xFF00FF
#define CYAN	0x00FFFF
#define WHITE	0xFFFFFF

static const uint32_t colors[8] = { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA,
	CYAN, WHITE };

static uint32_t fg = WHITE, bg = BLACK;

/*
 * conputc -- prints a character to the system console
 */
void
conputc(int c)
{
	static uint32_t kx, ky, ansi_esc, esc_code;
	unsigned char *glyph, mask;
	int x, y, offs, i;

	if (ansi_esc)
		goto ansi;

	switch (c) {
	case '\n':
		goto newline;
	case '\t':
		kx = (kx / TAB_STOP + 1) * TAB_STOP;
		goto wrap;
	case '\b':
		if (kx > 0)
			kx--;
		return;
	case '\033':
		ansi_esc = 1;
		return;
	case ' ':
		goto space;
	}

	glyph = font->glyphs + c * font->char_size;
	offs = kx * WIDTH * 4 + ky * bootboot.fb_scanline * font->char_size;
	for (y = 0; y < font->char_size; y++) {
		mask = 1 << 7;
		i = offs;
		for (x = 0; x < WIDTH; x++) {
			*(uint32_t *)(fb_ptr + i) = (*glyph & mask) ? fg : bg;
			mask >>= 1;
			i += 4;
		}
		glyph++;
		offs += bootboot.fb_scanline;
	}
space:
	kx++;
wrap:
	if (kx * WIDTH > bootboot.fb_width) {
	newline:
		kx = 0;
		if (ky * font->char_size < bootboot.fb_height)
			ky++;
		else
			console_scroll();
	}
	return;
ansi:
	if (ansi_esc == 1) {
		if (c == '[')
			ansi_esc = 2;
		else
			ansi_esc = 0;
		return;
	}
	switch (c) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		esc_code = (esc_code * 10) + c - '0';
		break;
	case 'm':
		ansi_esc = 0;
	case ';':
		if (30 <= esc_code && 38 > esc_code)
			fg = colors[esc_code - 30];
		else if (esc_code == 39)
			fg = WHITE;
		else if (40 <= esc_code && 48 > esc_code)
			bg = colors[esc_code - 40];
		else if (esc_code == 49)
			bg = BLACK;
		else if (esc_code == 1)
			font = bold;
		else if (esc_code == 0) {
			fg = WHITE;
			bg = BLACK;
			font = normal;
		}
		esc_code = 0;
		break;
	default:
		ansi_esc = 0;
		break;
	}
}

static void
console_scroll()
{
	uint8_t *c, *fb_end;
	size_t offset;

	fb_end = fb_ptr + bootboot.fb_height * bootboot.fb_scanline;
	offset = bootboot.fb_scanline * font->char_size;
	for (c = fb_ptr; c + offset < fb_end; c++)
		*c = *(c + offset);
	for (; c < fb_end; c++)
		*c = bg;
}
