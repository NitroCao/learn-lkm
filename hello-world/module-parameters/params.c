#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_AUTHOR("Nitro Cao");
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0.0");
MODULE_DESCRIPTION("Learn about the basic components of a LKM program");

#define MODULE_NAME "learn_params"
#define STR_PARAM_LEN 16

static int param1;
static int array_param1[2];
static char *string_param = "default value for the string parameter";

module_param(param1, int, 0644);
MODULE_PARM_DESC(param1, "This is a parameter of type int");
module_param_array(array_param1, int, NULL, 0644);
MODULE_PARM_DESC(array_param1, "This is a parameter of type array");
module_param(string_param, charp, 0644);
MODULE_PARM_DESC(string_param, "This is a parameter of type char pointer");

int __init params_init(void) {
    int i;
    printk(KERN_INFO "(%s) param1 = %d\n", MODULE_NAME, param1);
    printk(KERN_INFO "(%s) string_param = %s\n", MODULE_NAME, string_param);
    for (i = 0; i < (sizeof(array_param1) / sizeof(array_param1[0])); i++)
        printk(KERN_INFO "(%s) array_param1[%d] = %d\n", MODULE_NAME, i,
               array_param1[i]);
    return 0;
}

void __exit params_exit(void) {
    printk(KERN_INFO "(%s) Exited. Bye\n", MODULE_NAME);
    return;
}

module_init(params_init);
module_exit(params_exit);
