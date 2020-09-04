#!/bin/bash
rm test.o test.bin

#nasm -f elf64 test.S -o test.o
x86_64-elf-gcc --sysroot=/home/finlay/active/programming/C/OS/sysroot -isystem=/usr/include -MD -c test.c -o test.o -std=gnu11 -O2 -g -ffreestanding -Wextra -Wall -mno-red-zone
x86_64-elf-ld test.o libc/libc.a -o test.bin

mkdir disk_mnt
echfs-fuse disk.iso disk_mnt
rm disk_mnt/test.bin
cp test.bin disk_mnt/test.bin
umount disk_mnt

