#include <lib.h>

void umain(int argc, char *argv[]) {
    printf("BBBB>>> code_b forking\n");
    int pid = fork();
    printf("BBBB>>> pid: %d\n", pid);
    if(pid == 0) {
        printf("BBBB>>> I'm child\n");
        for(int i = 0, j = 0;; ++i) {
            if(i & 0x400000) {
                printf("%c", 'a' + j);
                i = 0;
                j = (j + 1) % 26;
            }
        }
    } else {
        printf("BBBB>>> I'm father\n");
    }
    return 0;
}
