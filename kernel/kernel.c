/* @doc false */
#include <stddef.h>
#include <stdint.h>

#include "bootboot.h"
#include "dt.h"
#include "kprint.h"
#include "paging.h"

#define ARRAY_LEN(a) (sizeof(a) / sizeof(a[0]))

struct h_size {
	size_t size;
	char suffix;
};

struct h_size human_size(size_t);

/*
 * full of magic numbers but its okay because this doesnt matter for long mode
 * anyway. empty 6th entry is for TSS
 */
static struct gdt_entry gdt[6] = {
	{ 0, 0, 0, 0, 0, 0 },		  // null descriptor
	{ 0xFFFF, 0, 0x9A, 0xF, 0xA, 0 }, // kernel code
	{ 0xFFFF, 0, 0x92, 0xF, 0xC, 0 }, // kernel data
	{ 0xFFFF, 0, 0xFA, 0xF, 0xA, 0 }, // user code
	{ 0xFFFF, 0, 0xF2, 0xF, 0xC, 0 }, // user data
};

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern char fb;
extern char __kernel_brk;
extern char __estart;

static const char *const MMAP_TYPES[] = { "USED", "FREE", "ACPI", "MMIO" };
static const char SUFFIX[] = { ' ', 'K', 'M', 'G', 'T' };

/*
 * main entry point
 */
void
_start()
{
	MMapEnt *cur;
	struct h_size cur_size;
	size_t n_mmap, i;

	set_gdt((uint64_t)gdt, sizeof(gdt));

	/*
	unsigned int x, y;
	uint8_t r, g;
	uint32_t *cur = (uint32_t *)&fb;
	for (y = 0; y < bootboot.fb_height; y++) {
		r = y * 0xFF / (bootboot.fb_height - 1);
		for (x = 0; x < bootboot.fb_width; x++) {
			g = x * 0xFF / (bootboot.fb_width - 1);
			*cur = r * 0x10000 + g * 0x100 + 0x7F;
			cur++;
		}
	}
	*/

	cur_size = human_size(bootboot.initrd_size);

	kprintf("%p %4zu%cB\n\n", (void *)bootboot.initrd_ptr, cur_size.size,
	    cur_size.suffix);

	kprintf("%p\n\n", &__estart);

	n_mmap = (bootboot.size - sizeof(bootboot)) / sizeof(MMapEnt) + 1;

	for (i = 0; i < n_mmap; i++) {
		cur = &bootboot.mmap + i;
		cur_size = human_size(MMapEnt_Size(cur));

		kprintf("%p  %4zu%cB  %s\n", (void *)cur->ptr, cur_size.size,
		    cur_size.suffix, MMAP_TYPES[MMapEnt_Type(cur)]);
	}

	kprintf("\n");

	paging_init(&bootboot.mmap, n_mmap);

	while (1) { }
}

struct h_size
human_size(size_t bytes)
{
	size_t i;

	for (i = 0; bytes >> 10 > 0 && i < ARRAY_LEN(SUFFIX); i++, bytes >>= 10)
		;

	return (struct h_size) { bytes, SUFFIX[i] };
}
