#include <stackframe.h>

.global pageout_entry

pageout_entry:
    subs lr, lr, #8
    EXC_SAVE_ALL

    mrc p15, 0, r0, c6, c0, 0
    bl pageout
    b pageout_return

.global pageout_return

pageout_return:
    RESTORE_ALL sp
