#include "lib.h"
#include <unistd.h>
#include <mmu.h>
#include <env.h>
#include <trap.h>

int syscall_dev_read(u_long dstva, u_long offset, u_long length) {
	return msyscall(SYS_dev_read, dstva, offset, length, 0, 0);
}

int syscall_dev_write(u_long srcva, u_long offset, u_long length) {
	return msyscall(SYS_dev_write, srcva, offset, length, 0, 0);
}

int syscall_va_perm(u_long va) {
	return msyscall(SYS_va_perm, va, 0, 0, 0, 0);
}

int pageref(u_long va) {
	return syscall_va_ref(va);
}

int syscall_va_ref(u_long va) {
	return msyscall(SYS_va_ref, va, 0, 0, 0, 0);
}

void syscall_putc(char ch)
{
	msyscall(SYS_putc, (int)ch, 0, 0, 0, 0);
}

u_int
syscall_getenvid(void)
{
	return msyscall(SYS_getenvid, 0, 0, 0, 0, 0);
}

void
syscall_yield(void)
{
	msyscall(SYS_yield, 0, 0, 0, 0, 0);
}

int
syscall_env_destroy(u_int envid)
{
	return msyscall(SYS_env_destroy, envid, 0, 0, 0, 0);
}

int
syscall_mem_alloc(u_int envid, u_int va, u_int perm)
{
	return msyscall(SYS_mem_alloc, envid, va, perm, 0, 0);
}

int
syscall_mem_map(u_int srcid, u_int srcva, u_int dstid, u_int dstva, u_int perm)
{
	return msyscall(SYS_mem_map, srcid, srcva, dstid, dstva, perm);
}

int
syscall_mem_unmap(u_int envid, u_int va)
{
	return msyscall(SYS_mem_unmap, envid, va, 0, 0, 0);
}

int
syscall_set_env_status(u_int envid, u_int status)
{
	return msyscall(SYS_set_env_status, envid, status, 0, 0, 0);
}

int
syscall_set_trapframe(u_int envid, struct Trapframe *tf)
{
	return msyscall(SYS_set_trapframe, envid, (int)tf, 0, 0, 0);
}

void
syscall_panic(char *msg)
{
	msyscall(SYS_panic, (int)msg, 0, 0, 0, 0);
}

int
syscall_ipc_can_send(u_int envid, u_int value, u_int srcva, u_int perm)
{
	return msyscall(SYS_ipc_can_send, envid, value, srcva, perm, 0);
}

int
syscall_ipc_recv(u_int dstva, u_int *whom, u_int *perm)
{
	return msyscall(SYS_ipc_recv, dstva, whom, perm, 0, 0);
}

int
syscall_getc()
{
	return msyscall(SYS_getc, 0, 0, 0, 0, 0);
}
