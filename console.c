#include <stdint.h>

#include "bootboot.h"
#include "console.h"

#define WIDTH 8 // psf 1.0 fonts are always 8 wide

#define TAB_STOP 8

#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF

struct psf {
	uint16_t magic;
	uint8_t flags;
	uint8_t char_size;
	uint8_t glyphs[];
} __attribute__((packed));

extern unsigned char _binary_terminus_psf_start;
extern BOOTBOOT bootboot;
extern uint8_t fb;
uint8_t *fb_ptr = &fb;

static struct psf *font = (struct psf *)&_binary_terminus_psf_start;

/*
 * conputc -- prints a character to the system console
 */
void
conputc(int c)
{
	static uint32_t kx, ky;
	unsigned char *glyph, mask;
	int x, y, offs, i;

	switch (c) {
	case '\0':
		c = 'B';
		break;
	case '\n':
		kx = 0;
		ky++;
		return;
	case '\t':
		kx = (kx / TAB_STOP + 1) * TAB_STOP;
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
			*(uint32_t *)(fb_ptr + i) = (*glyph & mask) ? WHITE : 0;
			mask >>= 1;
			i += 4;
		}
		glyph++;
		offs += bootboot.fb_scanline;
	}
space:
	kx++;
	if (kx * WIDTH > bootboot.fb_width) {
		kx = 0;
		ky++;
	}
}
