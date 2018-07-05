#include <armv7.h>

.global env_pop_tf

env_pop_tf:
	ldmfd r0!, {r3-r5}

    msr spsr_c, r5
    
    mrs r2, cpsr
    bic r1, r2, #ARMV7_MODE_MASK
    orr r1, r1, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r1
    
    mov sp, r3
    mov lr, r4
    msr cpsr_c, r2

    ldmfd r0, {r0-r12, pc}^
