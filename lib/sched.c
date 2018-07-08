#include <env.h>
#include <pmap.h>
#include <printf.h>

/* Overview:
 *  Check whether current environment has used up its time. If yes, then 
 *  check whether the list env_sched_list[pos] is empty. If yes, change the
 *  pointer to point the other list. Next, if current list is not empty, 
 *  pop the first one and allocate running time to it.
 * Hints:
 *  The variable which is for counting should be defined as 'static'.
 */
void sched_yield(void)
{
	static struct Env *iter = NULL;
	static int ts = 0;
	while(!LIST_EMPTY(&env_sched_list)) {
		if(ts <= 0) {
			if(iter) {
				iter = LIST_NEXT(iter, env_sched_link);
			}
			if(!iter) {
				iter = LIST_FIRST(&env_sched_list);
			}
			if(iter) {
				ts = iter->env_pri;
			} else {
				ts = 0;
			}
		}
		--ts;
		if(iter && iter->env_status == ENV_RUNNABLE) {
			env_run(iter);
		}
	}
	load_trapframe(KSTACKTOP);
}

void sched_init() {
    LIST_INIT(&env_sched_list);
}
