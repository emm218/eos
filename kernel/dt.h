#ifndef _DT_H
#define _DT_H

#include <stdint.h>

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

void set_gdt(uint64_t addr, uint16_t size);
void set_idt(uint64_t addr, uint16_t size);

#endif /* _DT_H */
