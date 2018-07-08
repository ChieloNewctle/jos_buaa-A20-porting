#include "lib.h"

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void umain(int argc, char *argv[])
{
    int r;
    int fdnum;
    char buf[512];
    int n;

    if ((r = open("/newmotd", O_RDWR)) < 0) {
        panic("open /newmotd: %d\n", r);
    }
    fdnum = r;
    printf("open is good\n");

    if ((n = read(fdnum, buf, 511)) < 0) {
        panic("read /newmotd: %d\n", n);
    }
    if (strcmp(buf, diff_msg) != 0) {
        panic("read returned wrong data\n");
    }
    printf("read is good\n");

    if ((r = ftruncate(fdnum, 0)) < 0) {
        panic("ftruncate: %d\n", r);
    }
    seek(fdnum, 0);

    if ((r = write(fdnum, msg, strlen(msg) + 1)) < 0) {
        panic("write /newmotd: %d\n", r);
    }

    if ((r = close(fdnum)) < 0) {
        panic("close /newmotd: %d\n", r);
    }

    //read again
    if ((r = open("/newmotd", O_RDONLY)) < 0) {
        panic("open /newmotd: %d\n", r);
    }
    fdnum = r;
    printf("open again: OK\n");

    if ((n = read(fdnum, buf, 511)) < 0) {
        panic("read /newmotd: %d\n", r);
    }
    if (strcmp(buf, msg) != 0) {
        panic("read returned wrong data\n");
    }
    printf("read again: OK\n");

    if ((r = close(fdnum)) < 0) {
        panic("close /newmotd: %d\n", r);
    }

    printf("file rewrite is good\n");

    printf("fstest.c: SUCCESS!\n");
    while (1) {
        //printf("IDLE!");
    }
}


