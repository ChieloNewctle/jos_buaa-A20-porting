#include <memlayout.h>
#include <printf.h>
#include <trap.h>
#include <timer2/timer2.h>
#include <gic/gic.h>
#include <armv7.h>
#include <sched.h>
#include <env.h>

unsigned get_cpsr() {
    unsigned cpsr;
    asm volatile("mrs %0, cpsr": "=r" (cpsr));
    return cpsr;
}

unsigned get_spsr() {
    unsigned spsr;
    asm volatile("mrs %0, spsr": "=r" (spsr));
    return spsr;
}

void save_trapframe(struct Trapframe *dst) {
    if(dst) {
        struct Trapframe *tf = (struct Trapframe *)(EXCSTACK - sizeof(struct Trapframe));
        bcopy(tf, dst, sizeof(struct Trapframe));
    }
}

void load_trapframe(struct Trapframe *src) {
    if(src) {
        struct Trapframe *tf = (struct Trapframe *)(EXCSTACK - sizeof(struct Trapframe));
        bcopy(src, tf, sizeof(struct Trapframe));
    }
}

void print_tf(struct Trapframe *tf) {
    printf(">>>>>>>>> TF\n");
    printf("user sp: %x\n", tf->usr_sp);
    printf("user lr: %x\n", tf->usr_lr);
    printf("original cpsr: %x\n", tf->ori_cpsr);
    for(int i = 0; i < 13; ++i) {
        printf("regs[%d]: %x\n", i, tf->regs[i]);
    }
    printf("exception return pc: %x\n", tf->exc_pc);
    printf("<<<<<<<<< TF\n");
}

void irq_handler() {
    int interrupt_id = gicc_get_interrupt_id();
    // printf("irq.c:\tIRQ triggered, id: %d\n", interrupt_id);

    struct Trapframe *tf = (struct Trapframe *)(EXCSTACK - sizeof(struct Trapframe));

    // for(const unsigned *u = EXCSTACK - 17 * 4; u < EXCSTACK; ++u) {
    //     printf("%x: %x\n", u, *u);
    // }
    // print_tf(tf);
    
    if((get_spsr() & ARMV7_MODE_MASK) == ARMV7_SVC_MODE) {
        // printf("from svc, saving Trapframe\n");
        save_trapframe(KSTACKTOP);
        // printf("saved to %x\n", KSTACKTOP);
    }
    
    if(interrupt_id == TMR2_GIC_SRC_ID) {
        timer2_clear_irq();
        sched_yield();
    }
}
