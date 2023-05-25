#include <stdint.h>

#include "bootboot.h"
#include "kprint.h"

void set_gdt(uint64_t addr, uint16_t size);

struct gdt_info {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

struct gdt_entry {
	unsigned int limit_low : 16;
	unsigned int base_low : 24;
	unsigned int access : 8;
	unsigned int limit_high : 4;
	unsigned int flags : 4;
	unsigned int base_high : 8;
} __attribute__((packed));

static struct gdt_entry gdt[6] = {
	{ 0, 0, 0, 0, 0, 0 },		  // null descriptor
	{ 0xFFFF, 0, 0x9A, 0xF, 0xA, 0 }, // kernel code
	{ 0xFFFF, 0, 0x92, 0xF, 0xC, 0 }, // kernel data
	{ 0xFFFF, 0, 0xFA, 0xF, 0xA, 0 }, // user code
	{ 0xFFFF, 0, 0xF2, 0xF, 0xC, 0 }, // user data
};

extern BOOTBOOT bootboot;
extern unsigned char environment[4096];
extern uint8_t fb;

void
_start()
{
	set_gdt((uint64_t)gdt, sizeof(gdt));

	while (1)
		;
}
