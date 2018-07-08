#include <stackframe.h>

.global env_pop_tf

env_pop_tf:
    RESTORE_ALL r0
