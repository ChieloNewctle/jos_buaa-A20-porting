# Main makefile
#
# Copyright (C) 2007 Beihang University
# Written by Zhu Like ( zlike@cse.buaa.edu.cn )
#

drivers_dir	:= drivers
boot_dir	:= boot
init_dir	:= init
lib_dir		:= lib
mm_dir		:= mm
user_dir	:= user
fs_dir		:= fs
tools_dir	:= tools
kernel_elf	:= kernel.elf
kernel_bin	:= kernel.bin
kernel_img	:= kernel.img

target		:= uImage.my
load_addr	:= 0x40000000
entry_addr	:= 0x40010000

boot_script	:= boot.cmd
boot_bin	:= boot.scr

fs_img := fs.img
fs_uImage := uImage.fs

link_script	:= $(tools_dir)/kernel.lds

modules		:= tools boot drivers init lib mm user fs $(test_dir)
objects		:= $(boot_dir)/start.o \
				$(init_dir)/main.o \
				$(init_dir)/init.o \
				$(drivers_dir)/uart0/uart0.o \
				$(drivers_dir)/gic/gic.o \
				$(drivers_dir)/timer2/timer2.o \
				$(lib_dir)/*.o \
				$(mm_dir)/*.o \
				$(user_dir)/*.x \
				$(fs_dir)/*.x


.PHONY: all $(modules) clean

all: $(modules) kernel

kernel: $(modules)
	$(LD) $(LDFLAGS) -o $(kernel_elf) -N -T $(link_script) $(objects) $(LIBGCC)
	$(OBJCOPY) -O binary $(kernel_elf) $(kernel_bin)
	$(tools_dir)/mkimage -A arm -O linux -T kernel -C none -a $(load_addr) -e $(entry_addr) -d $(kernel_bin) $(target)
	$(tools_dir)/mkimage -A arm -T script -O linux -d $(boot_script) $(boot_bin)

$(modules):
	$(MAKE) --directory=$@

clean: 
	for d in $(modules); \
		do \
			$(MAKE) --directory=$$d clean; \
		done; \
	rm -rf *.o *~ $(kernel_elf) $(kernel_bin) $(kernel_img) $(boot_bin) $(target)

include include.mk
