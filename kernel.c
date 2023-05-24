#include <stdint.h>

#include "bootboot.h"

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

struct psf {
	uint16_t magic;
	uint8_t flags;
	uint8_t char_size;
	uint8_t glyphs[];
} __attribute__((packed));

extern unsigned char _binary_terminus_psf_start;

struct psf *font = (struct psf *)&_binary_terminus_psf_start;

void
_start()
{
	int x, y, w, h, s;
	w = bootboot.fb_width;
	h = bootboot.fb_height;
	s = bootboot.fb_scanline;

	for (y = 0; y < h; y++) {
		*((uint32_t *)(&fb + s * y + (w * 2))) = 0x00FFFFFF;
	}
	for (x = 0; x < w; x++) {
		*((uint32_t *)(&fb + s * (h / 2) + x * 4)) = 0x00FFFFFF;
	}
	while (1)
		;
}
