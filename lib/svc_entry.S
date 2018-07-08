#include <stackframe.h>
#include <unistd.h>


.global svc_vec
svc_vec:
    .align 4
    .word sys_putc
    .word sys_getc
    .word sys_env_alloc
    .word sys_env_destroy
    .word sys_getenvid
    .word sys_yield
    .word sys_mem_alloc
    .word sys_mem_map
    .word sys_mem_unmap
    .word sys_set_env_status
    .word sys_set_trapframe
    .word sys_panic
    .word sys_ipc_can_send
    .word sys_ipc_recv


.global svc_entry

svc_entry:
    EXC_SAVE_ALL
    
    cmp r0, #(__NR_SYSCALLS)
    blo normal

    // return -1, if `sysno` greater than or equal to the number of syscalls
    mvn r0, #0
    b svc_return

normal:
    mrs r7, cpsr
    bic r8, r7, #ARMV7_MODE_MASK
    orr r8, r8, #ARMV7_SYSTEM_MODE
    msr cpsr_c, r8

    // fetch the fifth and sixth argument of syscall
    ldm sp, {r4, r5}
    msr cpsr_c, r7

    stmfd sp!, {r4, r5}
    lsl r6, r0, #2
    ldr r7, =svc_vec
    add r6, r6, r7
    ldr r6, [r6]
    blx r6
    add sp, sp, #8

    b svc_return


.global svc_return

svc_return:
    str r0, [sp, #12] // Trapframe.regs[0]

    // mov r0, sp
    // bl print_tf

    RESTORE_ALL sp

