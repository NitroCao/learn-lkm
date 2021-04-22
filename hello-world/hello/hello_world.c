#include <linux/module.h>
#include <linux/init.h>

MODULE_AUTHOR("Nitro Cao");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
MODULE_DESCRIPTION("Learn about the basic components of a LKM program");

#define MODULE_NAME "hello"

int __init hello_init(void) {
    printk(KERN_INFO "(%s) Hello, LKM!\n", MODULE_NAME);
    return 0;
}

void __exit hello_exit(void) {
    printk(KERN_INFO "(%s) Exited. Bye\n", MODULE_NAME);
    return;
}

module_init(hello_init);
module_exit(hello_exit);
