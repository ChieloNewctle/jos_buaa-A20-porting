#include <uart0/uart0.h>
#include <mmu.h>
#include <env.h>
#include <printf.h>
#include <pmap.h>
#include <error.h>
#include <sched.h>
#include <unistd.h>

extern struct Env *curenv;

/* Overview:
 * 	This function is used to print a character on screen.
 * 
 * Pre-Condition:
 * 	`c` is the character you want to print.
 */
int sys_putc(int sysno, int c)
{
	uart0_putc(c);
	return 0;
}

int sys_getc(int sysno)
{
	return uart0_getc_non_block();
}

/* Overview:
 *	This function provides the environment id of current process.
 *
 * Post-Condition:
 * 	return the current environment id
 */
u_int sys_getenvid(void)
{
	return curenv->env_id;
}

/* Overview:
 *	This function enables the current process to give up CPU.
 *
 * Post-Condition:
 * 	Deschedule current environment. This function will never return.
 */
void sys_yield(void)
{
	sched_yield();
}

/* Overview:
 * 	This function is used to destroy the current environment.
 *
 * Pre-Condition:
 * 	The parameter `envid` must be the environment id of a 
 * process, which is either a child of the caller of this function 
 * or the caller itself.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 when error occurs.
 */
int sys_env_destroy(int sysno, u_int envid)
{
	int r;
	struct Env *e;

	if ((r = envid2env(envid, &e, 1)) < 0) {
		return r;
	}

	printf("[%08x] destroying %08x\n", curenv->env_id, e->env_id);
	env_destroy(e);
	return 0;
}

/* Overview:
 * 	Allocate a page of memory and map it at 'va' with permission
 * 'perm' in the address space of 'envid'.
 *
 * 	If a page is already mapped at 'va', that page is unmapped as a
 * side-effect.
 * 
 * Pre-Condition:
 * perm -- PTE_V is required,
 *         PTE_COW is not allowed(return -E_INVAL),
 *         other bits are optional.
 *
 * Post-Condition:
 * Return 0 on success, < 0 on error
 *	- va must be < UTOP
 *	- env may modify its own address space or the address space of its children
 */
int sys_mem_alloc(int sysno, u_int envid, u_int va, u_int perm)
{
	if(va >= UTOP || va < 0) {
		printf("sys_mem_alloc: UTOP: %x, va:%x\n", UTOP, va);
		return -E_UNSPECIFIED;
	}
	if(!(perm & PTE_V)) {
		printf("sys_mem_alloc: wrong permission\n");
		return -E_INVAL;
	}
	// Your code here.
	struct Env *env;
	struct Page *ppage;
	int ret;
	ret = 0;
	if((ret = envid2env(envid, &env, 0)) < 0) {
		printf("sys_mem_alloc: wrong env id\n");
		return ret;
	}
	if((ret = page_alloc(&ppage)) < 0) {
		printf("sys_mem_alloc: wrong env id\n");
		return ret;
	}
	// ++ppage->pp_ref;
	if((ret = page_insert(env->env_pgdir, ppage, va, perm)) < 0) {
		printf("sys_mem_alloc: cannot insert page into page directory\n");
		return ret;
	}
	return ret;
}

/* Overview:
 * 	Map the page of memory at 'srcva' in srcid's address space
 * at 'dstva' in dstid's address space with permission 'perm'.
 * Perm has the same restrictions as in sys_mem_alloc.
 * (Probably we should add a restriction that you can't go from
 * non-writable to writable?)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Note:
 * 	Cannot access pages above UTOP.
 */
int sys_mem_map(int sysno, u_int srcid, u_int srcva, u_int dstid, u_int dstva,
				u_int perm)
{
	int ret;
	u_int round_srcva, round_dstva;
	struct Env *srcenv;
	struct Env *dstenv;
	struct Page *ppage;
	Pte *ppte;

	ppage = NULL;
	ret = 0;
	round_srcva = ROUNDDOWN(srcva, BY2PG);
	round_dstva = ROUNDDOWN(dstva, BY2PG);

    //your code here
	if(srcva >= UTOP || dstva >= UTOP || srcva < 0 || dstva < 0) {
		printf("sys_mem_map: srcva:%x and dstva:%x must less than UTOP(%x).\n", srcva, dstva, UTOP);
		return -E_UNSPECIFIED;
	}
	if(!(perm & PTE_V)) {
		printf("sys_mem_map: perm is illegal.\n");
		return -E_INVAL;
	}
	if((ret = envid2env(srcid, &srcenv, 0)) < 0) {
		printf("sys_mem_map: wrong srcid!\n");
		return ret;
	}
	if((ret = envid2env(dstid, &dstenv, 0)) < 0) {
		printf("sys_mem_map: wrong dstid!\n");
		return ret;
	}
	if((ppage = page_lookup(srcenv->env_pgdir, round_srcva, &ppte)) == NULL) {
		printf("sys_mem_map: page at %x does not exist.\n", round_srcva);
		return -E_UNSPECIFIED;
	}
	if((ret = page_insert(dstenv->env_pgdir, ppage, round_dstva, perm)) < 0) {
		printf("sys_mem_map: fail to insert src page to dst page.\n");
		return ret;
	}

	return ret;
}

/* Overview:
 * 	Unmap the page of memory at 'va' in the address space of 'envid'
 * (if no page is mapped, the function silently succeeds)
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Cannot unmap pages above UTOP.
 */
int sys_mem_unmap(int sysno, u_int envid, u_int va)
{
	// Your code here.
	int ret = 0;
	struct Env *env;

	if(va >= UTOP) {
		printf("sys_mem_unmap: va: %x >= UTOP(%x).\n", va, UTOP);
		return -E_INVAL;
	}
	if((ret = envid2env(envid, &env, PTE_V)) < 0) {
		printf("sys_mem_unmap: wrong envid!\n");
		return ret;
	}
	page_remove(env->env_pgdir, va);

	return ret;
	//	panic("sys_mem_unmap not implemented");
}

/* Overview:
 * 	Allocate a new environment.
 *
 * Pre-Condition:
 * The new child is left as env_alloc created it, except that
 * status is set to ENV_NOT_RUNNABLE and the register set is copied
 * from the current environment.
 *
 * Post-Condition:
 * 	In the child, the register set is tweaked so sys_env_alloc returns 0.
 * 	Returns envid of new environment, or < 0 on error.
 */
int sys_env_alloc(void)
{
	int r;
	struct Env *e;

	if((r = env_alloc(&e, curenv->env_id)) < 0) {
		printf("sys_env_alloc: unable allocate a env\n");
		return r;
	}
	Pde *src_pgdir = curenv->env_pgdir, *dst_pgdir = e->env_pgdir;
	for(u_long pdx = 0; pdx < PDX(ULIM); ++pdx) {
		if((src_pgdir[pdx] & PDE_PAGE_TABLE) != PDE_PAGE_TABLE)
			continue;
		Pte *table = KADDR(PTE_ADDR(src_pgdir[pdx]));
		for(u_long offset = 0; offset < PDMAP; offset += BY2PG) {
			if((table[PTX(offset)] & PTE_V) != PTE_V)
				continue;
			u_long va = (pdx << PDSHIFT) | offset;
			struct Page *p = pa2page(PG_ADDR_BASE(table[PTX(offset)]));
			if(!p) {
				printf("sys_env_alloc: unable to fetch page\n");
				return -E_INVAL;
			}
			struct Page *np;
			if((r = page_alloc(&np)) < 0) {
				printf("sys_env_alloc: unable to alloc a page\n");
				return r;
			}
			bcopy(page2kva(p), page2kva(np), BY2PG);
			if((r = page_insert(dst_pgdir, np, va, PG_ADDR_OFFSET(table[PTX(offset)]))) < 0) {
				printf("sys_env_alloc: unable to insert a page\n");
				return r;
			}
		}
	}
	bcopy((void *)EXCSTACK - sizeof(struct Trapframe), &(e->env_tf), sizeof(struct Trapframe));
	e->env_status = ENV_NOT_RUNNABLE;
	e->env_tf.regs[0] = 0;

	return e->env_id;
}

void my_set_env_status(struct Env *env, u_int status) {
	if(env->env_status == status) {
		return;
	}
	env->env_status = status;
	if(status == ENV_RUNNABLE) {
		LIST_INSERT_HEAD(&env_sched_list, env, env_sched_link);
	} else if(status == ENV_NOT_RUNNABLE) {
		LIST_REMOVE(env, env_sched_link);
	} else if(status == ENV_FREE) {
		sys_env_destroy(0, env->env_id);
	}
}


/* Overview:
 * 	Set envid's env_status to status.
 *
 * Pre-Condition:
 * 	status should be one of `ENV_RUNNABLE`, `ENV_NOT_RUNNABLE` and
 * `ENV_FREE`. Otherwise return -E_INVAL.
 * 
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if status is not a valid status for an environment.
 * 	The status of environment will be set to `status` on success.
 */
int sys_set_env_status(int sysno, u_int envid, u_int status)
{
	struct Env *env;
	int ret;

	if(status != ENV_RUNNABLE && status != ENV_NOT_RUNNABLE && status != ENV_FREE) {
		printf("sys_set_env_status: invalid status\n");
		return -E_INVAL;
	}
	if((ret = envid2env(envid, &env, 1)) < 0) {
		printf("sys_set_env_status: invalib envid\n");
		return ret;
	}
	my_set_env_status(env, status);

	return 0;
}

/* Overview:
 * 	Set envid's trap frame to tf.
 *
 * Pre-Condition:
 * 	`tf` should be valid.
 *
 * Post-Condition:
 * 	Returns 0 on success, < 0 on error.
 * 	Return -E_INVAL if the environment cannot be manipulated.
 *
 * Note: This hasn't be used now?
 */
int sys_set_trapframe(int sysno, u_int envid, struct Trapframe *tf)
{
	int ret;
	struct Env *env;
	if((ret = envid2env(envid, &env, PTE_V)) < 0) {
		return ret;
	}
	bcopy(tf, &env->env_tf, sizeof(struct Trapframe));
	return 0;
}

/* Overview:
 * 	Kernel panic with message `msg`. 
 *
 * Pre-Condition:
 * 	msg can't be NULL
 *
 * Post-Condition:
 * 	This function will make the whole system stop.
 */
void sys_panic(int sysno, char *msg)
{
	panic("%s", msg);
}

/* Overview:
 * 	This function enables caller to receive message from 
 * other process. To be more specific, it will flag 
 * the current process so that other process could send 
 * message to it.
 *
 * Pre-Condition:
 * 	`dstva` is valid (Note: NULL is also a valid value for `dstva`).
 * 
 * Post-Condition:
 * 	This syscall will set the current process's status to 
 * ENV_NOT_RUNNABLE, giving up cpu. 
 */
int sys_ipc_recv(int sysno, u_int dstva, u_int *whom, u_int *perm)
{
	if(dstva >= UTOP){
		printf("sys_ipc_recv: wrong dstva %x\n", dstva);
		return;
	}
	curenv->env_ipc_recving = 1;
	curenv->env_ipc_dstva = dstva;
	my_set_env_status(curenv, ENV_NOT_RUNNABLE);
	sys_yield();
}

/* Overview:
 * 	Try to send 'value' to the target env 'envid'.
 *
 * 	The send fails with a return value of -E_IPC_NOT_RECV if the
 * target has not requested IPC with sys_ipc_recv.
 * 	Otherwise, the send succeeds, and the target's ipc fields are
 * updated as follows:
 *    env_ipc_recving is set to 0 to block future sends
 *    env_ipc_from is set to the sending envid
 *    env_ipc_value is set to the 'value' parameter
 * 	The target environment is marked runnable again.
 *
 * Post-Condition:
 * 	Return 0 on success, < 0 on error.
 *
 * Hint: the only function you need to call is envid2env.
 */
int sys_ipc_can_send(int sysno, u_int envid, u_int value, u_int srcva,
					 u_int perm)
{

	int r;
	struct Env *e;
	struct Page *p;

	if(srcva >= UTOP) {
		printf("sys_ipc_can_send: wrong srcva %x\n", srcva);
		return -E_INVAL;
	}
	if(envid2env(envid, &e, 0) < 0) {
		printf("sys_ipc_can_send: can't found env with envid %d\n", envid);
		return -E_INVAL;
	}

	if(e->env_ipc_recving == 0) {
		return -E_IPC_NOT_RECV;
	}

	if(srcva) {
		if((p = page_lookup(curenv->env_pgdir, srcva, 0)) <= 0) {
			printf("sys_ipc_can_send: send srcva %x is not exist\n", srcva);
			return p;
		}
		if(page_insert(e->env_pgdir, p, e->env_ipc_dstva, perm) < 0) {
			printf("sys_ipc_can_send: dst pot failed.\n");
			return -E_INVAL;
		}
	}

	e->env_ipc_perm = perm | PTE_V | PTE_R;
	e->env_ipc_recving = 0;
	e->env_ipc_value = value;
	e->env_ipc_from = curenv->env_id;

	e->env_tf.regs[0] = e->env_ipc_value;
	u_int *r_whom = (u_int *)e->env_tf.regs[2], *r_perm = (u_int *)e->env_tf.regs[3];
	printf("r_whom: %x, r_perm: %x\n", r_whom, r_perm);
	if(r_whom) {
		struct Page *p = page_lookup(e->env_pgdir, r_whom, NULL);
		if(p > 0) {
			*(u_int *)(page2kva(p) | PG_ADDR_OFFSET(r_whom)) = e->env_ipc_from;
		} else {
			printf("sys_ipc_can_send: r_whom is not valid\n");
		}
	}
	if(r_perm) {
		struct Page *p = page_lookup(e->env_pgdir, r_perm, NULL);
		if(p > 0) {
			*(u_int *)(page2kva(p) | PG_ADDR_OFFSET(r_perm)) = e->env_ipc_perm;
		} else {
			printf("sys_ipc_can_send: r_perm is not valid\n");
		}
	}

	// e->env_status = ENV_RUNNABLE;
	my_set_env_status(e, ENV_RUNNABLE);

	return 0;
}

