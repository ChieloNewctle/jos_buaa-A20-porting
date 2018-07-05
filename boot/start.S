#include <armv7.h>
#include <memlayout.h>

.syntax unified


.section ".text.except_vec"

vec_reset:
    ldr pc, =_start
vec_undefined_instruct:
    movs pc, lr
vec_soft:
    movs pc, lr // reserve for syscall
vec_prefetch:
    subs pc, lr, #4
vec_data:
    subs pc, lr, #8
vec_reserved:
    subs pc, lr, #4
vec_irq:
    subs pc, lr, #4 // reserve for irq
vec_fiq:
    subs pc, lr, #4


.section ".text.boot"

.global _start

_start:
    mrs r0, cpsr
    bic r0, r0, #ARMV7_MODE_MASK
    orr r0, r0, #ARMV7_SVC_MODE
    msr cpsr_c, r0
    nop

    mrs r0, cpsr
    orr r0, r0, #ARMV7_IRQ_MASK
    orr r0, r0, #ARMV7_FIQ_MASK
    msr cpsr_c, r0
    nop

    ldr r0, =0x5
    ldr r1, =(boot_pgdir - ULIM + DRAMBASE)
    mov r2, #0
    mcr p15, 0, r0, c3, c0, 0 // set domain access
    mcr p15, 0, r1, c2, c0, 0 // ttb r0
    mcr p15, 0, r1, c2, c0, 1 // ttb r1
    mcr p15, 0, r2, c2, c0, 2 // ttb cr
    nop

    // Cortex-A9: must set ACTLR.SMP to 1 before caches and MMU are enabled
    mrc p15, 0, r0, c1, c0, 1
    orr r0, r0, #(1 << 6) // set SMP bit
    mcr p15, 0, r0, c1, c0, 1
    nop

    mrc p15, 0, r0, c1, c0
    orr r0, r0, #(ARMV7_C1_V_BIT)
    mcr p15, 0, r0, c1, c0

    mrc p15, 0, r0, c1, c0
    // enable MMU, data cache, instruction cache, flow prediction, align
    orr r0, r0, #(ARMV7_C1_M_BIT)
    orr r0, r0, #(ARMV7_C1_C_BIT)
    orr r0, r0, #(ARMV7_C1_I_BIT)
    mcr p15, 0, r0, c7, c5, 0
    orr r0, r0, #(ARMV7_C1_Z_BIT)
    mcr p15, 0, r0, c7, c5, 6
    orr r0, r0, #(ARMV7_C1_A_BIT)
    mcr p15, 0, r0, c1, c0
    nop

    ldr lr, =high_addr
    bx lr


high_addr:
    ldr sp, =KSTACKTOP
    bl init_boot_pgdir

    bl main


endless:
    b endless

