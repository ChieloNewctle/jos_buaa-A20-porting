// Copyright (C) 2002 ARM Limited, All Rights Reserved.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// Interrupt architecture for the GIC:
//
// o There is one Interrupt Distributor, which receives interrupts
//   from system devices and sends them to the Interrupt Controllers.
//
// o There is one CPU Interface per CPU, which sends interrupts sent
//   by the Distributor, and interrupts generated locally, to the
//   associated CPU. The base address of the CPU interface is usually
//   aliased so that the same address points to different chips depending
//   on the CPU it is accessed from.
//
// Note that IRQs 0-31 are special - they are local to each CPU.
// As such, the enable set/clear, pending set/clear and active bit
// registers are banked per-cpu for these sources.

#include <memlayout.h>

#define readl(addr)     (*((volatile unsigned long  *)(addr)))
#define writel(v, addr)     (*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

#define GICD_BASE (IOVABASE + 0x81000)
#define GICC_BASE (IOVABASE + 0x82000)

#define GICD_CTRL (GICD_BASE)
#define GICD_ISENABLER (GICD_BASE + 0x100)
#define GICD_ICENABLER (GICD_BASE + 0x180)
#define GICD_ICPENDR (GICD_BASE + 0x280)
#define GICD_ICACTIVER (GICD_BASE + 0x380)
#define GICD_IPRIORITYR (GICD_BASE + 0x400)
#define GICD_ITARGETSR (GICD_BASE + 0x800)
#define GICD_ICFGR (GICD_BASE + 0xC00)

#define GICC_CTLR (GICC_BASE)
#define GICC_PMR (GICC_BASE + 0x0004)
#define GICC_IAR (GICC_BASE + 0x000C)
#define GICC_EOIR (GICC_BASE + 0x0010)

#define GIC_IRQ_N 123

void gicd_ctrl_enable() {
    writel(0, GICD_CTRL);

    int i;
    for (i = 32; i < GIC_IRQ_N; i += 16)
        writel(0, GICD_ICFGR + i * 4 / 16);
    for (i = 32; i < GIC_IRQ_N; i += 4)
        writel(0x01010101, GICD_ITARGETSR + i * 4 / 4);
    for (i = 32; i < GIC_IRQ_N; i += 4)
        writel(0xA0A0A0A0, GICD_IPRIORITYR + i * 4 / 4);
    for (i = 32; i < GIC_IRQ_N; i += 32)
        writel(0xFFFFFFFF, GICD_ICENABLER + i * 4 / 32);

    writel(1, GICD_CTRL);
}

void gicd_set_enable(unsigned src_id)
{
    unsigned n = src_id >> 5, addr = GICD_ISENABLER + (n << 2);
    writel(1 << (src_id & 0x1F), addr);
    gicd_clear_pending(src_id);
}

void gicd_clear_pending(unsigned src_id)
{
    unsigned n = src_id >> 5, addr = GICD_ICPENDR + (n << 2);
    writel(1 << (src_id & 0x1F), addr);
    addr = GICD_ICACTIVER + (n << 2);
    writel(1 << (src_id & 0x1F), addr);
}

int gicc_get_interrupt_id() {
    unsigned iar = readl(GICC_IAR);
    writel(iar, GICC_EOIR);
    return iar & 0x3FF;
}

void gicc_ctrl_enable() {
    writel(0xffff0000, GICD_ICENABLER);
    writel(0x0000ffff, GICD_ISENABLER);

    int i;
    for (i = 0; i < 32; i += 4)
        writel(0xA0A0A0A0, GICD_IPRIORITYR + i * 4 / 4);

    writel(0xF0, GICC_PMR);
    writel(1, GICC_CTLR);
}
