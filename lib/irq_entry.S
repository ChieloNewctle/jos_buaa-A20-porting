#include <armv7.h>
#include <memlayout.h>

.global irq_entry

irq_entry:
    sub lr, lr, #4
    ldr sp, =IRQSTACK
    
    stmfd sp!, {r0-r12, lr}

    mrs r0, cpsr
    bic r1, r0, #ARMV7_MODE_MASK
    orr r1, r1, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r1
    
    mov r3, sp
    mov r4, lr
    msr cpsr_c, r0
    
    mrs r5, spsr
    stmfd sp!, {r3-r5}

    bl irq_handler
    b irq_return

.global irq_return

irq_return:
    ldmfd sp!, {r3-r5}
    msr spsr_c, r5
    
    mrs r0, cpsr
    bic r1, r0, #ARMV7_MODE_MASK
    orr r1, r1, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r1
    
    mov sp, r3
    mov lr, r4
    msr cpsr_c, r0

    ldmfd sp!, {r0-r12, pc}^
