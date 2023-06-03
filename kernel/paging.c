#include "kprint.h"
#include "paging.h"

void
print_pte(struct pte *p)
{
	kprintf("%p: 0x%02x\t0x%010lx%s\n", p, p->flags, p->address,
	    p->xd ? "\txd" : "");
}

uint64_t
get_physical_addr(void *p)
{
	struct pte *page_table, *cur;
	uint64_t a;
	int pml4, pdpt, pd, pt;

	asm("movq %%cr3, %%rax\n"
	    "movq %%rax, %0"
	    : "=rm"(page_table));

	a = ((uint64_t)p & (((long)1 << 48) - 1)) >> PAGE_SHIFT;
	pml4 = a >> 27;
	pdpt = a >> 18 & 0x01FF;
	pd = a >> 9 & 0x01FF;
	pt = a & 0x01FF;

	kprintf("%x %x %x %x\n", pml4, pdpt, pd, pt);

	cur = page_table;
	print_pte(cur + pml4);
	cur = (struct pte *)(cur[pml4].address * PAGE_SIZE);
	print_pte(cur + pdpt);
	cur = (struct pte *)(cur[pdpt].address * PAGE_SIZE);
	print_pte(cur + pd);
	cur = (struct pte *)(cur[pd].address * PAGE_SIZE);
	print_pte(cur + pt);

	return cur[pt].address * PAGE_SIZE + ((uint64_t)p) % PAGE_SIZE;
}
