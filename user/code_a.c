#include <uart0/uart0.h>

unsigned get_cpsr() {
    unsigned cpsr;
    asm volatile("mrs %0, cpsr": "=r" (cpsr));
    return cpsr;
}

int main() {
    uart0_puts("cpsr: ");
    unsigned cpsr = get_cpsr();
    for(int i = 32; i > 0; i -= 4) {
        unsigned k = (cpsr >> (i - 4)) & 0xF;
        if(k >= 0xA) {
            uart0_putc('A' + k - 0xA);
        } else {
            uart0_putc('0' + k);
        }
    }
    uart0_putc('\t');
    for(int i = 0, j = 0;; ++i) {
        if(i & 0x400000) {
            uart0_putc('A' + j);
            i = 0;
            j = (j + 1) % 26;
        }
    }
    return 0;
}
