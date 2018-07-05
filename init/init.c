#include <printf.h>

void armv7_init()
{
    printf("init.c:\tarmv7_init() is called\n");

    int test = 391;
    printf("test(%d) is at %x\n", test, &test);
    test = 1096;
    printf("now test(%d) is at %x\n", test, &test);

    panic("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
}
