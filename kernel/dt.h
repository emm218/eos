#ifndef _DT_H
#define _DT_H

#include <stdint.h>

void set_gdt(uint64_t addr, uint16_t size);
void set_idt(uint64_t addr, uint16_t size);

#endif /* _DT_H */
