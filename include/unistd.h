#ifndef UNISTD_H
#define UNISTD_H

#define __NR_SYSCALLS 14

#define SYS_putc 0
#define SYS_getc 1
#define SYS_env_alloc 2
#define SYS_env_destroy 3
#define SYS_getenvid 4
#define SYS_yield 5
#define SYS_mem_alloc 6
#define SYS_mem_map 7
#define SYS_mem_unmap 8
#define SYS_set_env_status 9
#define SYS_set_trapframe 10
#define SYS_panic 11
#define SYS_ipc_can_send 12
#define SYS_ipc_recv 13

#endif
