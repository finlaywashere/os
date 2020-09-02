#!/bin/bash
rm test.o test.bin

nasm -f elf64 test.S -o test.o
x86_64-elf-ld test.o libc/libc.a -o test.bin

mkdir disk_mnt
echfs-fuse disk.iso disk_mnt
rm disk_mnt/test.bin
cp test.bin disk_mnt/test.bin
umount disk_mnt

