# Lesson 1 - Hello World
## Basic Components

The source code related to this section is located in the [hello](hello) directory.

The source code of each module consists of three basic components: **metadata**, **an init function**, **an exit function** and a **Makefile**.

Generally, we declare metadata at the beginning of the source code file, whereas put `module_init()` and `module_exit()` to the end of the file.

### Metadata

Metadata specifies the author, version, lincese and description of the module. From the source code, we know that:

* `MODULE_AUTHOR` is to specify the author.
* `MODULE_LICENSE` is to specify the license.
* `MODULE_VERSION` is to specify the version number.
* `MODULE_DESCRIPTION` is to specify the description.

These macros are declared in `<linux/module.h>` file.

We can use `modinfo` command to check the metadata of a module:

```shell
$ modinfo ./hello.ko
filename:       /home/nitroc/learn-lkm/hello-world/hello/./hello.ko
description:    Learn about the basic components of a LKM program
version:        1.0.0
license:        GPL
author:         Nitro Cao
srcversion:     12900B8B4A57BAAE4BC34D0
depends:
retpoline:      Y
name:           hello
vermagic:       5.4.84-1-lts SMP mod_unload
```

### Init function

The init function is used to initialize the module.

Use `module_init()` function to specify the init function.

The prototype of the init function is `int __init func_name(void)`.

### Exit function

The exit function is used to clean up before removing the module from the kernel.  

Use `module_exit()` function to specify the exit function.

The prototype of the exit function is `void __exit func_name(void)`.

### Makefile

a Makefile tells the compiler how to build projects.  

In our makefile, `obj-m` specifies object files which are built as LKMs. The name of the object file would be the module name. `<module-name>-objs` specifies which source files the LKM needs.  

For the make command of the first target `all`, `-C` parameter tells make command enter the build directory of the kernel. `M=$(PWD)` specifies the current direcotry of the Makefile as our project directory. With the `M` variable, make command knows where our actual project directory is and can change back to it. `clean` target will delete all files generated during compilation.

## /sys/module

The kernel will create a directory under `sys/module` for each loaded module. The directory name is the name of the module.

## Module Parameters

The source code related to this section is located in the [module-parameters](module-parameters) directory.

We can pass parameters to a module when loading it with `insmod` command. Use **module_param(name, type, perm)** macro to declare a parameter. `name` specifies the variable to store the value, `type` specifies the type of the parameter, and `perm` specifies the visibility of the parameter in sysfs. For our module `learn_params`, it has three parameters. The kernel creates a file for each parameter under `sys/module/learn_params/parameters`. The content of each file is the value of the corresponding parameter. So `perm` specifies the file permission.

From the comment of module_param, we know it supports the following data types:

* **byte**
* **short**
* **ushort**
* **int**
* **uint**
* **long**
* **ulong**
* **charp**: a character pointer.
* **bool**: possible values are 0/1, y/n, Y/N.
* **invbool**: similar to `bool`, only sense-reversed (N = true).

We can also use **module_param_array(name, type, nump, perm)** to declare a parameter of type array. Multiple values are separated by comma.

### Export Symbols

The source code related to this section is located in the [export-symbols](export-symbols) directory.

If we use `EXPORT_SYMBOL` macro to export a symbol, then other modules can use the exported symbols. There is another macro called `EXPORT_SYMBOL_GPL`, it's similar to `EXPORT_SYMBOL` except that the symbols exported by `EXPORT_SYMBOL_GPL` can only be used by modules that declares a GPL compatible license.

`modinfo` command can display the dependency of a module:

```shell
$ modinfo ./learn_export2.ko
filename:       /home/nitroc/learn-lkm/hello-world/export-symbols/./learn_export_2.ko
description:    Learn about the basic components of a LKM program
version:        1.0.0
license:        GPL
author:         Nitro Cao
srcversion:     D270CB485B81133EFB81D4D
depends:        learn_export_1
retpoline:      Y
name:           learn_export_2
vermagic:       5.4.84-1-lts SMP mod_unload
```

If we insert `learn_export2.ko` first, we would get the error that `could not insert module learn_export2.ko: Unknown symbol in module`. So we need insert `learn_export1.ko` first.

```shell
$ sudo insmod ./learn_export2.ko
insmod: ERROR: could not insert module learn_export2.ko: Unknown symbol in module
$ dmesg | tail
...
[7566646.000971] learn_export2: Unknown symbol module1_greeting (err -2)
$ sudo insmod ./learn_export1.ko
$ sudo insmod ./learn_export2.ko
$ dmesg | tail
...
[7566755.190772] (module1) Hello, LKM!
[7566757.643008] (module2) Hello, LKM!
[7566757.643009] Greeting from module module1
```

For the same reason, we cannot remove module `learn_export1` first.

```shell
$ sudo rmmod learn_export1
rmmod: ERROR: Module learn_export1 is in use by: learn_export2
$ sudo rmmod learn_export2
$ sudo rmmod learn_export1
```

## Advanced Topics

### \_\_init & \_\_exit

The two macros are declared in `<linux/init.h>` [https://elixir.bootlin.com/linux/v5.10.19/source/include/linux/init.h#L15](https://elixir.bootlin.com/linux/v5.10.19/source/include/linux/init.h#L15) file.

The following is the comment and definition from source code:

```c
/* These macros are used to mark some functions or
 * initialized data (doesn't apply to uninitialized data)
 * as `initialization' functions. The kernel can take this
 * as hint that the function is used only during the initialization
 * phase and free up used memory resources after
 *
 * Usage:
 * For functions:
 *
 * You should add __init immediately before the function name, like:
 *
 * static void __init initme(int x, int y)
 * {
 *    extern int z; z = x * y;
 * }
 *
 * If the function has a prototype somewhere, you can also add
 * __init between closing brace of the prototype and semicolon:
 *
 * extern int initialize_foobar_device(int, int, int) __init;
 *
 * For initialized data:
 * You should insert __initdata or __initconst between the variable name
 * and equal sign followed by value, e.g.:
 *
 * static int init_variable __initdata = 0;
 * static const char linux_logo[] __initconst = { 0x32, 0x36, ... };
 *
 * Don't forget to initialize data not at file scope, i.e. within a function,
 * as gcc otherwise puts the data into the bss section and not into the init
 * section.
 */
#define __init		__section(".init.text") __cold  __latent_entropy __noinitretpoline
#define __initdata	__section(".init.data")
#define __initconst	__section(".init.rodata")
#define __exitdata	__section(".exit.data")
#define __exit_call	__used __section(".exitcall.exit")
...
#define __exit          __section(".exit.text") __exitused __cold notrace
```

The `__init` macro is used to let the compiler put the machine code of the object into the `.init.text` section in the generated ELF file. Similarly, the `__exit` macro lets the compiler put the machine code of the object into the `.exit.text` section. The two macros tell the kernel that the functions are used only during the initialization phase and free up used memory resources.

If you have variables used only during the initialization, you can also use the macros such as `__initdata` for them.

We can use `objdump` command to verify it.

```shell
$ objdump ./hello.ko
hello.ko:     file format elf64-x86-64


Disassembly of section .init.text:

0000000000000000 <hello_init>:
   0:   e8 00 00 00 00          call   5 <hello_init+0x5>
   5:   48 c7 c6 00 00 00 00    mov    $0x0,%rsi
   c:   48 c7 c7 00 00 00 00    mov    $0x0,%rdi
  13:   e8 00 00 00 00          call   18 <hello_init+0x18>
  18:   31 c0                   xor    %eax,%eax
  1a:   c3                      ret

Disassembly of section .exit.text:

0000000000000000 <cleanup_module>:
   0:   48 c7 c6 00 00 00 00    mov    $0x0,%rsi
   7:   48 c7 c7 00 00 00 00    mov    $0x0,%rdi
   e:   e9 00 00 00 00          jmp    13 <cleanup_module+0x13>
...
```

We can see that in `.exit.text` section, the function name is `cleanup_module`. But our exit function is `hello_exit`. The reason is in the comment of `module_exit()` function [https://elixir.bootlin.com/linux/v5.10.19/source/include/linux/module.h#L95](https://elixir.bootlin.com/linux/v5.10.19/source/include/linux/module.h#L95):

```c
/**
 * module_exit() - driver exit entry point
 * @x: function to be run when driver is removed
 *
 * module_exit() will wrap the driver clean-up code
 * with cleanup_module() when used with rmmod when
 * the driver is a module.  If the driver is statically
 * compiled into the kernel, module_exit() has no effect.
 * There can only be one per module.
 */
```

### Linked Lists of All Modules

In the Linux kernel, all modules are stored in a linked list called `modules` (https://elixir.bootlin.com/linux/v5.10.19/source/kernel/module.c#L92)[https://elixir.bootlin.com/linux/v5.10.19/source/kernel/module.c#L92], which is protected by a mutex called `module_mutex`. The kernel uses `add_unformed_module()` function (https://elixir.bootlin.com/linux/v5.10.19/source/kernel/module.c#L3731)[https://elixir.bootlin.com/linux/v5.10.19/source/kernel/module.c#L3731] to add a new module into the linked list.