// gic driver interface

#ifndef GIC_H
#define GIC_H

void gicd_ctrl_enable(void);
void gicd_set_enable(unsigned);
void gicd_clear_pending(unsigned);
int gicc_get_interrupt_id();
void gicc_ctrl_enable();

#endif
