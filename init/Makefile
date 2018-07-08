INCLUDES := -I ./ -I ../ -I../include -I../drivers

fs_dir := ../fs

%.o: %.c
	$(CC) $(DEFS) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: init.o main.o $(fs_dir)

clean:
	rm -rf *~ *.o


include ../include.mk

