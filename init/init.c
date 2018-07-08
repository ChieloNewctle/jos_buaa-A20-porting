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

    // ENV_CREATE_PRIORITY(user_code_a, 2);
    // ENV_CREATE_PRIORITY(user_code_b, 1);
    ENV_CREATE_PRIORITY(user_fktest, 1);

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
    u_long stacked_value = v | (v << 8) | (v << 16) | (v << 24), *stacked_ptr = ptr;
    u_long i;
    for(i = 0; i + 3 < num; i += 4, ++stacked_ptr)
        *stacked_ptr = stacked_value;
    u_char *p = stacked_ptr;
    for(; i < num; ++i, ++p)
        *p = v;
    return ptr;
}

void *memcpy(void *dst, const void *src, u_long num) {
    // printf("init.c:\tmemcpy from %x to %x totally %d\n", src, dst, num);
    
    u_long *dst_long = dst, *src_long = src;
    u_long i;
    for(i = 0; i + 3 < num; i += 4, ++src_long, ++dst_long)
        *dst_long = *src_long;
    u_char *dst_char = dst_long, *src_char = src_long;
    for(; i < num; ++i, ++src_char, ++dst_char)
        *dst_char = *src_char;
    return dst;
}
