hello-objs := hello_world.o
obj-m := hello.o

KERN_DIR := /lib/modules/`uname -r`/build

all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean
	rm -rf $(PWD)/.cache
