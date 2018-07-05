#include <uart0/uart0.h>

int main() {
    for(int i = 0, j = 0;; ++i) {
        if(i & 0x400000) {
            uart0_putc('a' + j);
            i = 0;
            j = (j + 1) % 26;
        }
    }
    return 0;
}
