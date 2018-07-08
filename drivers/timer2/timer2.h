// timer2 driver interface

#ifndef TIMER2_H
#define TIMER2_H

#define TMR2_GIC_SRC_ID 56
#define TIMER_INVERTAL 0x100

void timer2_init(void);
void timer2_clear_irq(void);

#endif
