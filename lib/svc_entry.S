#include <stackframe.h>

.global svc_entry

svc_entry:
    EXC_SAVE_ALL

    mrs r7, cpsr
    bic r8, r7, #ARMV7_MODE_MASK
    orr r8, r8, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r8

    // fetch the fifth and sixth argument of syscall
    ldm sp, {r4, r5}
    msr cpsr_c, r7

    stmfd sp!, {r4, r5}
    bl svc_call
    add sp, sp, #8

    str r0, [sp, #12] // Trapframe.regs[0]

    mov r0, sp
    bl print_tf

    b svc_return

.global svc_return

svc_return:
    RESTORE_ALL sp
