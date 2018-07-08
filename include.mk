# Common includes in Makefile
#
# Copyright (C) 2007 Beihang University
# Written By Zhu Like ( zlike@cse.buaa.edu.cn )


CROSS_COMPILE	:= arm-linux-gnueabi-
CC				:= $(CROSS_COMPILE)gcc
CFLAGS			:= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -marm -nostartfiles -nostdlib -fno-zero-initialized-in-bss
LD				:= $(CROSS_COMPILE)ld
LDFLAGS			:= -static
OBJCOPY			:= $(CROSS_COMPILE)objcopy

LIBGCC			:= /usr/lib/gcc-cross/arm-linux-gnueabi/5.4.0/libgcc.a

