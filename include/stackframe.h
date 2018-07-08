#include <armv7.h>
#include <memlayout.h>

.macro EXC_SAVE_ALL
    ldr sp, =EXCSTACK

    stmfd sp!, {r0-r12, lr}

    mrs r7, cpsr
    bic r8, r7, #ARMV7_MODE_MASK
    orr r8, r8, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r8

    mov r4, sp
    mov r5, lr
    msr cpsr_c, r7

    mrs r6, spsr
    stmfd sp!, {r4-r6}
.endm

.macro RESTORE_ALL basereg
    ldmfd \basereg!, {r4-r6}
    msr spsr, r6
    
    mrs r7, cpsr
    bic r8, r7, #ARMV7_MODE_MASK
    orr r8, r8, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r8
    
    mov sp, r4
    mov lr, r5
    msr cpsr_c, r7

    ldmfd \basereg, {r0-r12, pc}^
.endm
