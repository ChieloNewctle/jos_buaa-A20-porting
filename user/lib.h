#ifndef LIB_H
#define LIB_H

#include "fd.h"
#include <pmap.h>
#include <mmu.h>
#include <trap.h>
#include <env.h>
#include <error.h>
#include <unistd.h>
/////////////////////////////////////////////////////head
extern void umain();
extern void libmain();
extern void exit();

extern unsigned envid;


#define USED(x) (void)(x)
//////////////////////////////////////////////////////printf
#include <printf.h>


/////////////////////////////////////////////////////fork spawn
int spawn(char *prog, char **argv);
int spawnl(char *prot, char *args, ...);
int fork(void);
//////////////////////////////////////////////////syscall_lib
extern int msyscall(int, int, int, int, int, int);

int syscall_dev_read(u_long dstva, u_long offset, u_long length);
int syscall_dev_write(u_long srcva, u_long offset, u_long length);
int syscall_va_perm(u_long va);
int syscall_va_ref(u_long va);
void syscall_putc(char ch);
int syscall_getc();
u_int syscall_getenvid(void);
void syscall_yield(void);
int syscall_env_destroy(u_int envid);
int syscall_mem_alloc(u_int envid, u_int va, u_int perm);
int syscall_mem_map(u_int srcid, u_int srcva, u_int dstid, u_int dstva,
					u_int perm);
int syscall_mem_unmap(u_int envid, u_int va);

inline static int syscall_env_alloc(void)
{
    return msyscall(SYS_env_alloc, 0, 0, 0, 0, 0);
}

int syscall_set_env_status(u_int envid, u_int status);
int syscall_set_trapframe(u_int envid, struct Trapframe *tf);
void syscall_panic(char *msg);
int syscall_ipc_can_send(u_int envid, u_int value, u_int srcva, u_int perm);
int syscall_ipc_recv(u_int dstva, u_int *whom, u_int *perm);

// string.c
int strlen(const char *s);
char *strcpy(char *dst, const char *src);
const char *strchr(const char *s, char c);
void *memcpy(void *destaddr, const void *srcaddr, u_long len);
void *memset(void *ptr, int value, u_long num);
int strcmp(const char *p, const char *q);

// ipc.c
void	ipc_send(u_int whom, u_int val, u_int srcva, u_int perm);
u_int	ipc_recv(u_int *whom, u_int dstva, u_int *perm);

// wait.c
void wait(u_int envid);

// console.c
int opencons(void);
int iscons(int fdnum);

// pipe.c
int pipe(int pfd[2]);
int pipeisclosed(int fdnum);

// pgfault.c
void set_pgfault_handler(void (*fn)(u_int va));

// fprintf.c
int fprintf(int fd, const char *fmt, ...);

// fsipc.c
int	fsipc_open(const char *, u_int, struct Fd *);
int	fsipc_map(u_int, u_int, u_int);
int	fsipc_set_size(u_int, u_int);
int	fsipc_close(u_int);
int	fsipc_dirty(u_int, u_int);
int	fsipc_remove(const char *);
int	fsipc_sync(void);
int	fsipc_incref(u_int);

// fd.c
int	close(int fd);
int	read(int fd, void *buf, u_int nbytes);
int	write(int fd, const void *buf, u_int nbytes);
int	seek(int fd, u_int offset);
void	close_all(void);
int	readn(int fd, void *buf, u_int nbytes);
int	dup(int oldfd, int newfd);
int fstat(int fdnum, struct Stat *stat);
int	stat(const char *path, struct Stat *);

// file.c
int	open(const char *path, int mode);
int	read_map(int fd, u_int offset, void **blk);
int	delete(const char *path);
int	ftruncate(int fd, u_int size);
int	sync(void);

/* File open modes */
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */

#define	O_CREAT		0x0100		/* create if nonexistent */
#define	O_TRUNC		0x0200		/* truncate to zero length */
#define	O_EXCL		0x0400		/* error if already exists */
#define O_MKDIR		0x0800		/* create directory, not regular file */


#endif
