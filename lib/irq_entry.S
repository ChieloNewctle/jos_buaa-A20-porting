#include <stackframe.h>

.global irq_entry

irq_entry:
    sub lr, lr, #4
    EXC_SAVE_ALL

    bl irq_handler
    b irq_return

.global irq_return

irq_return:
    RESTORE_ALL sp
