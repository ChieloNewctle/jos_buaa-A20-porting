INCLUDES = -I../include/

#CROSS_COMPILE := /usr/eldk/usr/bin/mips_4KC-
#CC			  := $(CROSS_COMPILE)gcc
#CFLAGS		  := -O -G 0 -mno-abicalls -fno-builtin -Wa,-xgot -Wall -fPIC
#LD			  := $(CROSS_COMPILE)ld
# For embedding one program in another

BINTOC	:= ../tools/bintoc
USERLIB := ../drivers/uart0/uart0.o syscall_wrap.o

CFLAGS += -nostdlib -static
INCLUDES += -I../drivers/ -I.

all: ../tools $(USERLIB) code_a.b code_a.x code_b.x

%.x: %.b.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.b.c: %.b
	$(BINTOC) user/$* $< $@
	
%.b: %.o $(USERLIB)
	$(LD) -o $@ $(LDFLAGS) -G 0 -static -n -nostdlib -T ./user.lds $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: lib.h

.PHONY: clean

clean:
	rm -rf *~ *.o *.b.c *.x *.b

include ../include.mk
