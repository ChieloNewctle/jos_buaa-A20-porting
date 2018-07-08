lib_dir := ../lib
tools_dir := ../tools

BINTOC := $(tools_dir)/bintoc
FSFORMAT := $(tools_dir)/fsformat

REQUIRES := $(lib_dir) $(tools_dir)

USERLIB := libos.o syscall_wrap.o syscall_lib.o ../lib/print.o printf.o fork.o ipc.o \
	string.o fd.o file.o fsipc.o pipe.o console.o fprintf.o

CFLAGS += -nostdlib -static
INCLUDES += -I. -I../include

all: $(REQUIRES) $(USERLIB) code_a.x code_b.x fktest.b fktest.x pingpong.x fstest.x

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
