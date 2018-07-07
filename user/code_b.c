#include <lib.h>

void umain(int argc, char *argv[]) {
    printf("code_b forking\n");
    int pid = fork();
    if(pid == 0) {
        printf("I'm child\n");
        for(int i = 0, j = 0;; ++i) {
            if(i & 0x400000) {
                printf("%c", 'a' + j);
                i = 0;
                j = (j + 1) % 26;
            }
        }
    } else {
        printf("I'm father\n");
    }
    return 0;
}
