#ifndef _MMU_H_
#define _MMU_H_

#include "types.h"
#include "memlayout.h"
/*
 * This file contains:
 *
 *	Part 1.  Definitions.
 *	Part 2.  Our conventions.
 *	Part 3.  Our helper functions.
 */

#define PDX(va)		((((u_long)(va)) >> PDSHIFT) & 0xFFF)
#define PTX(va)		((((u_long)(va)) >> PGSHIFT) & 0xFF)

// page number field of address
#define PPN(va)		(((u_long)(va)) >> PGSHIFT)
#define VPN(va)		PPN(va)

#define VA2PFN(va)		(((u_long)(va)) & 0xFFFFF000) // va 2 PFN for EntryLo0/1
//$#define VA2PDE(va)		(((u_long)(va)) & 0xFFC00000 ) // for context

/* Page Table/Directory Entry flags
 *   these are defined by the hardware
 */
#define PDE_SECTION 0x10402 // section + shareable + domain 0 + inaccessible for user
#define PDE_SECTION_C (PDE_SECTION | 0x8) // PDE_SECTION + cachable
#define PDE_SECTION_DEV (PDE_SECTION | 0x4) // PDE_SECTION + bufferable
#define PDE_SUPER_SECTION 0x50402 // super section + shareable + inaccessible for user (domain 0 always)
#define PDE_SUPER_SECTION_C (PDE_SUPER_SECTION | 0x8) // PDE_SUPER_SECTION + cachable
#define PDE_PAGE_TABLE 0x21 // page table + domain 1

#define PDE_SECTION_SHIFT 20
#define PDE_SECTION_BASE_MASK 0xFFF00000
#define PDE_SUPER_SECTION_SHIFT 24
#define PDE_SUPER_SECTION_BASE_MASK 0xFF000000
#define PDE_PAGE_TABLE_SHIFT 10
#define PDE_PAGE_TABLE_BASE_MASK 0xFFFFFC00

#define PTE_ADDR(pte) ((u_long)(pte) & PDE_PAGE_TABLE_BASE_MASK)

#define PTE_V 0x42A // small page + shareable + cachable + read only for user
#define PTE_R 0x30 // read and write for user

#define PTE_PAGE_BASE_MASK 0xFFFFF000

#define PG_ADDR_BASE(content) ((u_long)(content) & PTE_PAGE_BASE_MASK)
#define PG_ADDR_OFFSET(va) ((u_long)(va) & ~PTE_PAGE_BASE_MASK)


void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);

extern char bootstacktop[], bootstack[];

extern u_long npage;

typedef u_long Pde;
typedef u_long Pte;

#define PADDR(kva)						\
({								\
	u_long a = (u_long) (kva);				\
	if (a < ULIM)					\
		panic("PADDR called with invalid kva %08x", a);	\
	(a < IOVABASE ? a - ULIM + DRAMBASE : a - IOVABASE + IOPABASE);	\
})


// translates from physical address to kernel virtual address
#define KADDR(pa)						\
({								\
	u_long ppn = PPN((pa) - DRAMBASE);					\
	if (ppn >= npage)					\
		panic("KADDR called with invalid pa %08x", (u_long)pa);\
	(pa) - DRAMBASE + ULIM;					\
})

#define assert(x)	\
	do {	if (!(x)) panic("assertion failed: %s", #x); } while (0)

#endif // !_MMU_H_
