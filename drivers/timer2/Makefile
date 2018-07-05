# Makefile for uart0
#
# Modified by Chielo Newctle <chielonewctle@yandex.com>
#
# Copyright (C) 2007 Beihang Unversity.
# Written by Zhu Like, zlike@cse.buaa.edu.cn

INCLUDES := -I../../include/

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $*.o

.PHONY: clean
all: timer2.o

clean:
	rm -rf *.o *~

include ../../include.mk
