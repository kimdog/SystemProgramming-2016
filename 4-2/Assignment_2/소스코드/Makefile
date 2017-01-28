obj-m +=aa.o

KDIR =/usr/src/linux-4.4.1

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
    
