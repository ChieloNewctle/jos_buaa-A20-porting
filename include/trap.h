#ifndef _TRAP_H_
#define _TRAP_H_

#include <types.h>

struct Trapframe {
	/* Saved special registers. */
    unsigned long usr_sp;
	unsigned long usr_lr;
    unsigned long ori_cpsr;

	/* Saved main processor registers. */
	unsigned long regs[13];
    unsigned long exc_pc;
};
void trap_init();

#endif /* _TRAP_H_ */
