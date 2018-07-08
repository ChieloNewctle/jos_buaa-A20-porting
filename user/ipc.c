// User-level IPC library routines

#include "lib.h"
#include <mmu.h>
#include <env.h>
#include <error.h>

// Send val to whom.  This function keeps trying until
// it succeeds.  It should panic() on any error other than
// -E_IPC_NOT_RECV.
//
// Hint: use syscall_yield() to be CPU-friendly.
void
ipc_send(u_int whom, u_int val, u_int srcva, u_int perm)
{
	int r;

	while ((r = syscall_ipc_can_send(whom, val, srcva, perm)) == -E_IPC_NOT_RECV) {
		syscall_yield();
		//writef("QQ");
	}

	if (r == 0) {
		return;
	}

	panic("error in ipc_send: %d", r);
}

// Receive a value.  Return the value and store the caller's envid
// in *whom.
//
// Hint: use env to discover the value and who sent it.
u_int
ipc_recv(u_int *whom, u_int dstva, u_int *perm)
{
	//printf("ipc_recv:come 0\n");
	int ipc_value = syscall_ipc_recv(dstva, whom, perm);
	return ipc_value;
}

