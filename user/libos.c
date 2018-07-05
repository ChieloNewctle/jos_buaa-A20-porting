#include "lib.h"
#include <mmu.h>
#include <env.h>

void _exit(void)
{
	//close_all();
	syscall_env_destroy(0);
}

void raise(int sig)
{
    panic("signal 0x%x raised", sig);
}

unsigned envid;

void main(int argc, char **argv)
{
	// set env to point at our env structure in envs[].
	//writef("xxxxxxxxx %x  %x  xxxxxxxxx\n",argc,(int)argv);
	envid = syscall_getenvid();
	// call user main routine
	umain(argc, argv);
	// exit gracefully
	_exit();
	//syscall_env_destroy(0);
}
