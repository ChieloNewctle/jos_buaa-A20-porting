#include "lib.h"


void umain(int argc, char *argv[])
{
	int a = 0;
	int id = 0;

	if ((id = fork()) == 0) {
		if ((id = fork()) == 0) {
			a += 3;

			for (int i = 0; i < 8; ++i) {
				printf("\t\tthis is child2 :a:%d\n", a);
			}
			// for (;;) {
			// }
			return;
		}

		a += 2;

		for (int i = 0; i < 8; ++i) {
			printf("\tthis is child :a:%d\n", a);
		}
		// for (;;) {
		// }
		return;
	}

	a++;

	for (int i = 0; i < 8; ++i) {
		printf("this is father: a:%d\n", a);
	}
	// for (;;) {
	// }
	return;
}
