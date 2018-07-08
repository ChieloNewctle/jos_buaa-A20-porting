// implement fork from user space

#include "lib.h"
#include <mmu.h>
#include <env.h>

/* Overview:
 * 	User-level fork. Create a child and then copy our address space
 * and page fault handler setup to the child.
 *
 * Hint: remember to fix "envid" in the child process!
 */
int fork(void) {
	printf(">>>>> forking <<<<<\n");

	u_int newenvid;
	extern struct Env *envs;
	extern struct Env *env;
	u_int i, j;

	// alloc a new alloc
	if((newenvid = syscall_env_alloc()) < 0) {
		panic("fork failed\n");
		return newenvid;
	}
	// page duplication is done by syscall_env_alloc
	if(newenvid == 0) {
		envid = syscall_getenvid();
		printf("fork.c: reset envid %d\n", envid);
		return 0;
	}
	if(syscall_set_env_status(newenvid, ENV_RUNNABLE) < 0) {
		panic("failed to change child env status.\n");
		return -1;
	}
	printf("OK! newenvid is:%d\n",newenvid);

	return newenvid;
}
