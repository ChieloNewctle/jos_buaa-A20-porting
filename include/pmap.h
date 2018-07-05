#ifndef _PMAP_H_
#define _PMAP_H_

#include "types.h"
#include "queue.h"
#include "mmu.h"
#include "printf.h"


extern Pde boot_pgdir[(1 << (32 - PDSHIFT))] __attribute__((aligned(16 * 1024)));

LIST_HEAD(Page_list, Page);
typedef LIST_ENTRY(Page) Page_LIST_entry_t;

struct Page {
	Page_LIST_entry_t pp_link;	/* free list link */

	// Ref is the count of pointers (usually in page table entries)
	// to this page.  This only holds for pages allocated using
	// page_alloc.  Pages allocated at boot time using pmap.c's "alloc"
	// do not have valid reference count fields.

	u_short pp_ref;
};

extern struct Page *pages;

static inline u_long
page2ppn(struct Page *pp)
{
	return pp - pages;
}

/* Get the physical address of Page 'pp'.
 */
static inline u_long
page2pa(struct Page *pp)
{
	return (page2ppn(pp) << PGSHIFT) + DRAMBASE;
}

/* Get the Page struct whose physical address is 'pa'.
 */
static inline struct Page *
pa2page(u_long pa)
{
	if (PPN(pa - DRAMBASE) >= npage) {
		panic("pa2page called with invalid pa: %x", pa);
	}

	return &pages[PPN(pa - DRAMBASE)];
}

/* Get the kernel virtual address of Page 'pp'.
 */
static inline u_long
page2kva(struct Page *pp)
{
	return KADDR(page2pa(pp));
}

/* Transform the virtual address 'va' to physical address.
 */
static inline u_long
va2pa(Pde *pgdir, u_long va)
{
	pgdir = &pgdir[PDX(va)];

	if((*pgdir & PDE_SUPER_SECTION) == PDE_SUPER_SECTION) {
		return (*pgdir & PDE_SUPER_SECTION_BASE_MASK) | (va & ~PDE_SUPER_SECTION_BASE_MASK);
	}

	if((*pgdir & PDE_SECTION) == PDE_SECTION) {
		return (*pgdir & PDE_SECTION_BASE_MASK) | (va & ~PDE_SECTION_BASE_MASK);
	}

	if((*pgdir & PDE_PAGE_TABLE) == PDE_PAGE_TABLE) {
		Pte *p = (Pte *)KADDR(PTE_ADDR(*pgdir));
		if (!(p[PTX(va)] & PTE_V)) {
			return ~0;
		}
		return PG_ADDR_BASE(p[PTX(va)]) | PG_ADDR_OFFSET(va);
	}

	return ~0;
}

/********** functions for memory management(see implementation in mm/pmap.c). ***********/

void armv7_detect_memory();

void armv7_vm_init();

void page_init(void);
void page_check();
void physical_memory_manage_check();
int page_alloc(struct Page **pp);
void page_free(struct Page *pp);
void page_decref(struct Page *pp);
int pgdir_walk(Pde *pgdir, u_long va, int create, Pte **ppte);
int page_insert(Pde *pgdir, struct Page *pp, u_long va, u_int perm);
struct Page *page_lookup(Pde *pgdir, u_long va, Pte **ppte);
void page_remove(Pde *pgdir, u_long va) ;
void tlb_invalidate(Pde *pgdir, u_long va);

// void boot_map_segment(Pde *pgdir, u_long va, u_long size, u_long pa, int perm);

extern struct Page *pages;


#endif /* _PMAP_H_ */
