#include <stdint.h>

#include "bootboot.h"
#include "kprint.h"

void set_gdt(uint64_t addr, uint16_t size);

struct gdt_info {
	uint16_t size;
	uint64_t addr;
} __attribute__((packed));

struct gdt_entry {
	uint16_t limit_low : 16;
	unsigned int base_low : 24;
	uint8_t access : 8;
	unsigned int limit_high : 4;
	unsigned int flags : 4;
	uint8_t base_high : 8;
} __attribute__((packed));

struct idt_entry {
	uint16_t offset_low;
	uint16_t selector;  //  2 bits - rpl
			    //  1 bit  - table
			    // 13 bits - index
	uint8_t ist;	    // always 0
	uint8_t attributes; // 4 bits - type
			    // 1 bit  - reserved
			    // 2 bits - dpl
			    // 1 bit  - present
	unsigned long offset_high : 48;
	uint32_t reserved; // always 0
} __attribute__((packed));

/*
 * full of magic numbers but its okay because this doesnt matter for long mode
 * anyway
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
extern uint8_t fb;

void
_start()
{
	set_gdt((uint64_t)gdt, sizeof(gdt));

	kprintf("%zu", sizeof(struct idt_entry));

	while (1)
		;
}
