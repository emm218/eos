#ifndef _PAGING_H
#define _PAGING_H

#include <stddef.h>
#include <stdint.h>

#include "bootboot.h"

typedef uintptr_t vaddr_t;
typedef unsigned long paddr_t;

struct pte *kv_to_pte(vaddr_t);

void print_pte(struct pte *);

void paging_init(MMapEnt *, size_t);

#endif /* _PAGING_H */
