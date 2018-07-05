#include <lib.h>

unsigned get_cpsr() {
    unsigned cpsr;
    asm volatile("mrs %0, cpsr": "=r" (cpsr));
    return cpsr;
}

void printHex(unsigned u) {
    for(int i = 32; i > 0; i -= 4) {
        unsigned k = (u >> (i - 4)) & 0xF;
        if(k >= 0xA) {
            syscall_putc('A' + k - 0xA);
        } else {
            syscall_putc('0' + k);
        }
    }
}

static int data_seg[4] = {3911096, 716};
static int bss_seg[4];

void umain(int argc, char **argv) {
    printf("cpsr: %x\n", get_cpsr());
    printf("syscall_getenvid return: %x\n", syscall_getenvid());
    printf("envid: %d\n", envid);
    printf("data_seg(%x): %d\n", data_seg, data_seg[0]);
    *data_seg = 1096391;
    printf("data_seg(%x): %d\n", data_seg, data_seg[0]);
    printf("bss_seg(%x): %d\n", bss_seg, bss_seg[0]);
    bss_seg[0] = data_seg[1];
    printf("bss_seg(%x): %d\n", bss_seg, bss_seg[0]);

    printf("Try to access 0x70000000\n");
    printf("0x70000000: %x\n", *(const unsigned *)0x70000000);
    *(unsigned *)0x70000000 = data_seg[0] * bss_seg[0];
    printf("0x70000000: %x\n", *(const unsigned *)0x70000000);

    printf("Try to access 0x7123456C\n");
    printf("0x7123456C: %x\n", *(const unsigned *)0x7123456C);
    *(unsigned *)0x7123456C = data_seg[0] * bss_seg[0];
    printf("0x7123456C: %x\n", *(const unsigned *)0x7123456C);

    while(1) {
        int c = syscall_getc();
        if(c) {
            syscall_putc(c);
        }
    }

    for(int i = 0, j = 0;; ++i) {
        if(i & 0x400000) {
            syscall_putc('A' + j);
            i = 0;
            j = (j + 1) % 26;
        }
    }
    return;
}
