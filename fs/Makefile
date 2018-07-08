INCLUDES = -I../include/ -I../user/

user_dir := ../user
lib_dir := ../lib
tools_dir := ../tools

REQUIRES := $(user_dir) $(lib_dir) $(tools_dir)

BINTOC := $(tools_dir)/bintoc
FSFORMAT := $(tools_dir)/fsformat

USERLIB := $(user_dir)/printf.o \
		$(lib_dir)/print.o \
		$(user_dir)/libos.o \
		$(user_dir)/fork.o \
		$(user_dir)/syscall_lib.o \
		$(user_dir)/syscall_wrap.o \
		$(user_dir)/ipc.o \
		$(user_dir)/string.o \
		$(user_dir)/fd.o \
		$(user_dir)/file.o \
		$(user_dir)/fsipc.o \
		$(user_dir)/pipe.o \
		$(user_dir)/console.o \
		$(user_dir)/fprintf.o

FSLIB := fs.o \
		ide.o \
		test.o

FSIMGFILES := motd newmotd
FSIMG := fs.img
FSIMG_OBJ := fs.img.x

CFLAGS += -nostdlib -static

all: $(REQUIRES) $(USERLIB) $(FSLIB) serv.x serv.b $(FSIMG).b.c $(FSIMG_OBJ)

%.x: %.b.c
	$(CC) $(INCLUDES) $(CFLAGS) -c -o $@ $<

%.b.c: %.b
	$(BINTOC) fs/$* $< $@

$(FSIMG).b.c: $(FSIMG)
	$(BINTOC) fs_img $< $@

%.b: %.o $(USERLIB) $(FSLIB)
	$(LD) -o $@ $(LDFLAGS) -G 0 -static -n -nostdlib -T $(user_dir)/user.lds $^ $(LIBGCC)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<


%.o: $(user_dir)/lib.h

$(FSIMG): $(FSIMGFILES)
	dd if=/dev/zero of=$@ bs=4096 count=1024 2> /dev/null
	$(FSFORMAT) $@ $^

.PHONY: clean

clean:
	rm -rf *~ *.o *.b.c *.b *.x

include ../include.mk
