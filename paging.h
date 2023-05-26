#ifndef _PAGING_H
#define _PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096
#define PAGE_SHIFT 12

struct pte {
	uint8_t flags;
	unsigned int avl_1 : 4;
	unsigned long address : 40;
	unsigned int avl_2 : 11;
	unsigned int xd : 1;
} __attribute__((packed));

void print_pte(struct pte *);

#endif
