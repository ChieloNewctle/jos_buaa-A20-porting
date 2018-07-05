INCLUDES = -I../include/

#CROSS_COMPILE := /usr/eldk/usr/bin/mips_4KC-
#CC			  := $(CROSS_COMPILE)gcc
#CFLAGS		  := -O -G 0 -mno-abicalls -fno-builtin -Wa,-xgot -Wall -fPIC
#LD			  := $(CROSS_COMPILE)ld
# For embedding one program in another

REQUIRES := ../tools ../lib
BINTOC	:= ../tools/bintoc
USERLIB := libos.o syscall_wrap.o syscall_lib.o ../lib/print.o printf.o

CFLAGS += -nostdlib -static
INCLUDES += -I. -I../include

all: $(REQUIRES) $(USERLIB) code_a.b code_a.x

%.x: %.b.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.b.c: %.b
	$(BINTOC) user/$* $< $@
	
%.b: %.o $(USERLIB)
	$(LD) -o $@ $(LDFLAGS) -G 0 -static -n -nostdlib -T ./user.lds $^ $(LIBGCC)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: lib.h

.PHONY: clean

clean:
	rm -rf *~ *.o *.b.c *.x *.b

include ../include.mk
