#ifndef _DT_H
#define _DT_H

#include <stdint.h>

#define KERNEL_CS 1 * sizeof(struct gdt_entry)
#define KERNEL_DS 2 * sizeof(struct gdt_entry)
#define USER_CS	  3 * sizeof(struct gdt_entry)
#define USER_DS	  4 * sizeof(struct gdt_entry)
#define TSS	  5 * sizeof(struct gdt_entry)

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

void set_gdt(void *addr, uint16_t size);
void set_idt(void *addr, uint16_t size);

#endif /* _DT_H */
