#include "timer2.h"
#include <memlayout.h>
#include "../gic/gic.h"

#define readl(addr)     (*((volatile unsigned long  *)(addr)))
#define writel(v, addr)     (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

#define TMR_BASE (IOVABASE + 0x20C00)

#define TMR_IRQ_EN TMR_BASE
#define TMR_IRQ_STA (TMR_BASE + 0x4)

#define TMR2_CTRL (TMR_BASE + 0x30)
#define TMR2_INTV (TMR_BASE + 0x34)

#define TMR2_IRQ_EN (1 << 2)
#define TMR2_IRQ_PENDING (1 << 2)
#define TMR2_CTRL_V 0x41 // scale: /16, enable

void timer2_init(void)
{
    writel(0x100, TMR2_INTV);
    writel(readl(TMR_IRQ_EN) | TMR2_IRQ_EN, TMR_IRQ_EN);
    writel(TMR2_CTRL_V, TMR2_CTRL);
    gicd_set_enable(TMR2_GIC_SRC_ID);
    timer2_clear_irq();
}

void timer2_clear_irq(void)
{
    writel(TMR2_IRQ_PENDING, TMR_IRQ_STA);
    gicd_clear_pending(TMR2_GIC_SRC_ID);
}
