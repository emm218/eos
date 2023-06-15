#ifndef _PAGING_H
#define _PAGING_H

#include <stddef.h>
#include <stdint.h>

#include "bootboot.h"

typedef uintptr_t vaddr_t;
typedef uint64_t paddr_t;

void paging_init(MMapEnt *, size_t);
paddr_t va_to_pa(const void *);

#endif /* _PAGING_H */
