#include "kprint.h"
#include "paging.h"
#include "tree.h"

#define PAGE_SIZE	   4096
#define PAGE_SHIFT	   12
#define PAGE_TABLE_ENTRIES 512

/*
 * Mask to get rid of the sign-extended part of addresses.
 */
#define VA_SIGN_MASK	0xffff000000000000
#define VA_SIGN_NEG(va) ((va) | VA_SIGN_MASK)
#define VA_SIGN_POS(va) ((va) & ~VA_SIGN_MASK)

#define L1_SHIFT 12
#define L2_SHIFT 21
#define L3_SHIFT 30
#define L4_SHIFT 39

#define NBPD_L1 (1ULL << L1_SHIFT) /* # bytes mapped by L1 ent (4K) */
#define NBPD_L2 (1ULL << L2_SHIFT) /* # bytes mapped by L2 ent (2MB) */
#define NBPD_L3 (1ULL << L3_SHIFT) /* # bytes mapped by L3 ent (1G) */
#define NBPD_L4 (1ULL << L4_SHIFT) /* # bytes mapped by L4 ent (512G) */

#define L4_MASK (0x1ffUL << L4_SHIFT)
#define L3_MASK (0x1ffUL << L3_SHIFT)
#define L2_MASK (0x1ffUL << L2_SHIFT)
#define L1_MASK (0x1ffUL << L1_SHIFT)

#define L4_FRAME L4_MASK
#define L3_FRAME (L4_FRAME | L3_MASK)
#define L2_FRAME (L3_FRAME | L2_MASK)
#define L1_FRAME (L2_FRAME | L1_MASK)

#define PTE_ADDR(p) p & 0x0000FFFFFFFFF000

// flags for PTEs
#define PRESENT	      0x01
#define WRITABLE      0x02
#define USER	      0x04
#define WRITE_THROUGH 0x08
#define CACHE_DISABLE 0x10
#define ACCESSED      0x20
#define DIRTY	      0x40
#define BIG_PAGE      0x80
#define XD	      1UL << 63

typedef uint64_t pte_t;

struct pmem_range {
	paddr_t low;  /* start address */
	paddr_t high; /* end address + 1 (so that its on a page boundary) */
	struct rb_entry pmr_tree; /* rb tree of pmrs sorted by addr */
};

static int pmr_addr_cmp(const struct pmem_range *const,
    const struct pmem_range *const);

static pte_t *kv_to_pte(const void *);

RBT_GENERATE(pmr_tree_t, struct pmem_range, pmr_tree, pmr_addr_cmp);

extern char xd_enable;

extern uint8_t __estart;

/* static pmr_tree_t free_tree = NULL; */
static const void *pte_base = ((void *)(255 * NBPD_L4));

static int
pmr_addr_cmp(const struct pmem_range *const l, const struct pmem_range *const r)
{
	return (l->low > r->low) - (r->low > l->low);
}

void
paging_init(MMapEnt *mmap, size_t n_mmap)
{
	pte_t *page_table;

	(void)mmap;
	(void)n_mmap;

	asm("movq %%cr3, %0" : "=r"(page_table));

	page_table[255] = (pte_t)page_table | PRESENT | WRITABLE;
}

static pte_t *
kv_to_pte(const void *p)
{
	return (
	    pte_t *)(pte_base + ((VA_SIGN_POS((vaddr_t)p) & L1_FRAME) >> 9));
}

paddr_t
va_to_pa(const void *p)
{
	return PTE_ADDR(*kv_to_pte(p)) + (paddr_t)p % PAGE_SIZE;
}
