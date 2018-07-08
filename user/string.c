// String routines.  Not hardware optimized, but not shabby.

#include "lib.h"

int
strlen(const char *s)
{
	int n;

	for (n = 0; *s; s++) {
		n++;
	}

	return n;
}

char *
strcpy(char *dst, const char *src)
{
	char *ret;

	ret = dst;

	while ((*dst++ = *src++) != 0)
		;

	return ret;
}

const char *
strchr(const char *s, char c)
{
	for (; *s; s++)
		if (*s == c) {
			return s;
		}

	return 0;
}

void bcopy(const void *src, void *dst, size_t n) {
    memcpy(dst, src, n);
}

void *memset(void *ptr, int value, u_long num) {
    // printf("string.c:\tmemset for %x with %x totally %d\n", (u_long)ptr, value, num);
    u_char v = value;
    u_char *p = ptr;
    for(u_long i = 0; i < num; ++i, ++p)
        *p = v;
    return ptr;
}

void *memcpy(void *dst, const void *src, u_long num) {
    // printf("string.c:\tmemcpy from %x to %x totally %d\n", src, dst, num);
    u_char *p = dst;
    const u_char *q = src;
    for(u_long i = 0; i < num; ++i, ++p, ++q)
        *p = *q;
    return dst;
}

int
strcmp(const char *p, const char *q)
{
	while (*p && *p == *q) {
		p++, q++;
	}

	if ((u_int)*p < (u_int)*q) {
		return -1;
	}

	if ((u_int)*p > (u_int)*q) {
		return 1;
	}

	return 0;
}

