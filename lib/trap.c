#include <trap.h>
#include <pmap.h>
#include <gic/gic.h>
#include <armv7.h>

void trap_init(void)
{
    // set exception vector
    extern u_long mCONTEXT;
    printf("mCONTEXT: %x\n", mCONTEXT);
    int r = page_insert(mCONTEXT, &pages[0], 0xFFFF0000, 0); // read-only for user
    assert(r == 0);
    printf("%x %x\n", mCONTEXT, *(const unsigned *)0xFFFF0000);

    // GIC initialization
    gicd_ctrl_enable();
    gicc_ctrl_enable();

    // IRQ enabled
    unsigned cpsr;
    asm volatile("mrs %0, cpsr": "=&r" (cpsr));
    cpsr &= ~ARMV7_IRQ_MASK;
    asm volatile("msr cpsr_c, %0": : "r" (cpsr));

    asm volatile("mrs %0, cpsr": "=&r" (cpsr));
    printf("trap.c:\ttrap init %x\n", cpsr);
}
