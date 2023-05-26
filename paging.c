#include "kprint.h"
#include "paging.h"

void
print_pte(struct pte *p)
{
	kprintf("0x%x\t%p%s\n", p->flags, (void *)p->address,
	    p->xd ? "\txd" : "");
}
