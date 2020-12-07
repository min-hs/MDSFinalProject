obj-m += l298n.o
obj-m += hc_sr04_1.o
obj-m += hc_sr04_2.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
	rm -rf *.order
	rm -rf *.symvers
ins:
	sudo insmod l298n.ko
	sudo insmod hc_sr04_1.ko
	sudo insmod hc_sr04_2.ko
rm:
	sudo rmmod  l298n
	sudo rmmod  hc_sr04_1
	sudo rmmod  hc_sr04_2

