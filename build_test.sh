#!/bin/bash
rm test.o test.bin

nasm -f elf64 test.S -o test.o
x86_64-elf-ld test.o libc/libc.a -o test.bin

