#include "lib.h"

int outer = 391;

void umain()
{
	int a = 1096;
	int stack, id = fork();

	writef("stack is %d, id is %x, env(%x)->id is %x\n", stack, id, env, env->env_id);

	if (id == 0) {
		writef("I'm a child\nI wonder what will happen if I change a(%d) into 999\n", a);
		a = 999;
		writef("now a is %d\n", a);
		writef("I wonder what will happen if I change outer(%d) into 555\n", outer);
		outer = 555;
		writef("now outer is %d\n", outer);
		int forechild = fork();
		writef("A: forechild is %x, env(%x)->id is %x\n", forechild, env, env->env_id);
		a = 391;
		forechild = fork();
		writef("B: forechild is %x, env(%x)->id is %x\n", forechild, env, env->env_id);
		outer = 1096;
		forechild = fork();
		writef("C: forechild is %x, env(%x)->id is %x\n", forechild, env, env->env_id);
		a = 555;
		outer = 999;
		writef("now return\n");
		return;
	}

	char ta = 0, touter = 0;
	for (;;) {
		if(!ta && a != 1096) {
			ta = 1;
			writef("someone changed a into %d\n", a);
		}
		if(!touter && outer != 391) {
			touter = 1;
			writef("someone changed outer into %d\n", outer);
		}
	}
}

