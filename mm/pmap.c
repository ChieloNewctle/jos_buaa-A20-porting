#include <pmap.h>

Pde boot_pgdir[4096] __attribute__((aligned(16 * 1024))) = {
    [PDX(IOPABASE)] = IOPABASE | PDE_SECTION,
    [PDX(DRAMBASE)] = DRAMBASE | PDE_SECTION,
    [PDX(ULIM)] = DRAMBASE | PDE_SECTION,
    [PDX(KSTACKTOP)] = ((KSTACKTOP - ULIM + DRAMBASE) & 0xFFF00000) | PDE_SECTION,
};

void init_boot_pgdir() {
    boot_pgdir[0] = DRAMBASE | PDE_SECTION_C; // except vec
    for(u_long i = 1; i < ULIM; i += PDMAP)
        boot_pgdir[PDX(i)] = 0;
    for(u_long i = 0; i < DRAMSIZE; i += PDMAP)
        boot_pgdir[PDX(i + ULIM)] = ((i + DRAMBASE) & PDE_SUPER_SECTION_BASE_MASK) | PDE_SUPER_SECTION_C;
    for(u_long i = IOPABASE; i < IOPATOP; i += PDMAP)
        boot_pgdir[PDX(i - IOPABASE + IOVABASE)] = i | PDE_SECTION_DEV;
}
