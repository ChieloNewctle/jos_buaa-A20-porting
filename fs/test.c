#include "fs.h"
#include "lib.h"

int
strecmp(char *a, char *b)
{
	while (*b)
		if (*a++ != *b++) {
			return 1;
		}

	return 0;
}

static char *msg = "This is the NEW message of the day!\r\n\r\n";
static char *diff_msg = "This is a different massage of the day!\r\n\r\n";

void
fs_test(void)
{
	struct File *f;
	int r;
	void *blk;
	u_int *bits;

	// back up bitmap
	if ((r = syscall_mem_alloc(0, BY2PG, PTE_V | PTE_R)) < 0) {
		panic("syscall_mem_alloc: %e\n", r);
	}

	bits = (u_int *)BY2PG;
	bcopy(bitmap, bits, BY2PG);

	// allocate block
	if ((r = alloc_block()) < 0) {
		panic("alloc_block: %e\n", r);
	}

	// check that block was free
	assert(bits[r / 32] & (1 << (r % 32)));
	// and is not free any more
	assert(!(bitmap[r / 32] & (1 << (r % 32))));
	printf("alloc_block is good\n");

	if ((r = file_open("/not-found", &f)) < 0 && r != -E_NOT_FOUND) {
		panic("file_open /not-found: %e\n", r);
	} else if (r == 0) {
		panic("file_open /not-found succeeded!\n");
	}

	if ((r = file_open("/newmotd", &f)) < 0) {
		panic("file_open /newmotd: %d\n", r);
	}

	printf("file_open is good\n");

	if ((r = file_get_block(f, 0, &blk)) < 0) {
		panic("file_get_block: %e\n", r);
	}

	if (strecmp(blk, msg) != 0) {
		panic("file_get_block returned wrong data\n");
	}

	printf("file_get_block is good\n");

	*(volatile char *)blk = *(volatile char *)blk;
	file_flush(f);
	printf("file_flush is good\n");

	if ((r = file_set_size(f, 0)) < 0) {
		panic("file_set_size: %e\n", r);
	}

	assert(f->f_direct[0] == 0);
	printf("file_truncate is good\n");

	if ((r = file_set_size(f, strlen(diff_msg))) < 0) {
		panic("file_set_size 2: %e\n", r);
	}

	if ((r = file_get_block(f, 0, &blk)) < 0) {
		panic("file_get_block 2: %e\n", r);
	}

	strcpy((char *)blk, diff_msg);
	file_flush(f);
	file_close(f);
	printf("file rewrite is good\n");
}
