KBUILD_EXTRA_SYMBOLS := /home/osta/SP_01/QUEUE/Module.symbers

obj-m +=myproc.o

KDIR =/usr/src/linux-4.4.1

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
