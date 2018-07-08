#include <stackframe.h>

.global env_pop_tf

env_pop_tf:
    mov sp, r0
    ldmfd sp!, {r4-r6}
    msr spsr, r6
    
    mrs r7, cpsr
    bic r8, r7, #ARMV7_MODE_MASK
    orr r8, r8, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r8
    
    mov sp, r4
    mov lr, r5

    msr cpsr_c, r7

    ldmfd sp, {r0-r12, pc}^
