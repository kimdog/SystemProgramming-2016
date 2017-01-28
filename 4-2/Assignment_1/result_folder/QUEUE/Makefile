obj-m +=myqueue.o

KDIR =/usr/src/linux-4.4.1

all:
	$(MAKE) $(CFLAG) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
