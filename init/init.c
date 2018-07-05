#include <env.h>
#include <timer2/timer2.h>
#include <pmap.h>
#include <printf.h>
#include <trap.h>
#include <sched.h>

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
    env_check();

    trap_init();

    /*you can create some processes(env) here. in terms of binary code, please refer current directory/code_a.c
     * code_b.c*/
    /*you may want to create process by MACRO, please read env.h file, in which you will find it. this MACRO is very
     * interesting, have fun please*/
    ENV_CREATE_PRIORITY(user_code_a, 2);
    printf("env code a\n");
    // ENV_CREATE_PRIORITY(user_code_b, 1);
    // printf("env code b\n");

    timer2_init();

    for(int i = 0, j = 0;; ++i) {
        if(i & 0x4000000) {
            uart0_putc('0' + j);
            j = (j + 1) % 10;
            i = 0;
        }
    }

    panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
}

void bcopy(const void *src, void *dst, size_t len)
{
    void *max;

    max = dst + len;

    // copy machine words while possible
    while (dst + 3 < max) {
        *(int *)dst = *(int *)src;
        dst += 4;
        src += 4;
    }

    // finish remaining 0-3 bytes
    while (dst < max) {
        *(char *)dst = *(char *)src;
        dst += 1;
        src += 1;
    }
}

void * memset(void *ptr, int value, u_long num) {
    printf("init.c:\tmemset for %x with %x totally %d\n", (u_long)ptr, value, num);
    
    u_char v = value;
    u_long stacked_value = v | (v << 8) | (v << 16) | (v << 24), *stacked_ptr = ptr, i;
    for(i = 0; i < num; i += 4)
        *stacked_ptr = stacked_value;
    u_char *p = ptr;
    for(; i < num; ++i)
        *p = v;
}
