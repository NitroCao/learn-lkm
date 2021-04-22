obj-m := learn_export1.o
obj-m += learn_export2.o

KERN_DIR := /lib/modules/`uname -r`/build

all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	rm -rf $(PWD)/.cache
