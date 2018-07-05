INCLUDES := -I./ -I../ -I../include/ -I../drivers/
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

%.o: %.S
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

.PHONY: clean

all: svc_entry.o svc.o kernel_elfloader.o irq_entry.o irq.o env.o env_pop_tf.o print.o printf.o sched.o trap.o

clean:
	rm -rf *~ *.o


include ../include.mk
