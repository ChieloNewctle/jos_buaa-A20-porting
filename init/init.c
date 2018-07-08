#include <env.h>
#include <timer2/timer2.h>
#include <pmap.h>
#include <printf.h>
#include <trap.h>
#include <sched.h>
#include <fs.h>

u_long mCONTEXT = 0;

void armv7_init()
{
    printf("init.c:\tarmv7_init() is called\n");

    armv7_detect_memory();
    armv7_vm_init();
    page_init();

    physical_memory_manage_check();
    page_check();

    env_init();
    sched_init();
    // env_check();

    trap_init();

    /*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
     * code_b.c*/
    /*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
     * interesting, have fun please*/

    ENV_CREATE_PRIORITY(fs_serv, 1);
    // ENV_CREATE_PRIORITY(user_code_a, 2);
    // ENV_CREATE_PRIORITY(user_code_b, 1);
    // ENV_CREATE_PRIORITY(user_fktest, 1);
    // ENV_CREATE_PRIORITY(user_pingpong, 1);
    ENV_CREATE_PRIORITY(user_fstest, 1);

    timer2_init();
    printf("init.c: timer2 initialed\n");

    panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
}

void bcopy(const void *src, void *dst, size_t n) {
    memcpy(dst, src, n);
}

void *memset(void *ptr, int value, u_long num) {
    // printf("init.c:\tmemset for %x with %x totally %d\n", (u_long)ptr, value, num);
    u_char v = value;
    u_char *p = ptr;
    for(u_long i = 0; i < num; ++i, ++p)
        *p = v;
    return ptr;
}

void *memcpy(void *dst, const void *src, u_long num) {
    // printf("init.c:\tmemcpy from %x to %x totally %d\n", src, dst, num);
    u_char *p = dst;
    const u_char *q = src;
    for(u_long i = 0; i < num; ++i, ++p, ++q)
        *p = *q;
    return dst;
}
