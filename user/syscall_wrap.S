// int msyscall(int, int, int, int, int, int);
.global msyscall

msyscall:
    svc 0
    bx lr

