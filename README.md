# Learn LKM
This project aims to record my way to learn LKM (Linux Kernel Module).

## Prerequisites
This project focuses more on how to develop LKM. So I won't cover more basic knowledge of operating system and Linux system programming.
* You'd better know basic usage of Linux.
* You'd better know basic concepts of the operating system.
* You'd better know basic Linux system programming.
* You'd better know how to use Makefile.

## Development Environment
My development environment is `macOS` + `VirtualBox` + `Arch Linux` + `tmux` + `neovim`.  
Current kernel version is `5.10.19-1`. I use the lts version.
### tmux
Default configuration is enough.
### neovim plugins
I just list several important plugins.
* coc.nvim
  - coc-snippets
  - coc-pairs
  - coc-clangd
* LeaderF

## Lessons
### Lesson 1 - Hello World
The first lesson starts from a hello world LKM.
Lesson link: [Lesson 1 - Hello World](hello-world/README.md)
Basic targets:

  * Learn about the basic components of a LKM program.
  * Learn how to pass parameters to a LKM when loading.
  * Learn about the kernel symbol table.
