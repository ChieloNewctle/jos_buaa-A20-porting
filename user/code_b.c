#include <lib.h>

int k;

void umain(int argc, char *argv[]) {
    printf("BBBB>>> code_b forking\n");
    int pid = fork();
    printf("BBBB>>> pid: %d\n", pid);
    if(pid == 0) {
        printf("BBBB>>> I'm child\n");
        printf("BBBB>>> hmmm, k is %d\n", k);
        k = 1096;
        printf("BBBB>>> now k is %d\n", k);
    } else {
        printf("BBBB>>> I'm father\n");
        k = 391;
        printf("BBBB>>> k is %d for father\n", k);
    }
    return 0;
}
