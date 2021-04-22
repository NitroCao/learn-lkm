learn_params-objs := params.o
obj-m := learn_params.o

KERN_DIR := /lib/modules/`uname -r`/build

all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	rm -rf $(PWD)/.cache
