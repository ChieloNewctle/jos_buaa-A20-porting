// uart0 driver interface

#ifndef UART0_H
#define UART0_H

void uart0_putc(char);
void uart0_puts(const char *);
char uart0_getc(void);

#endif
