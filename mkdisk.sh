#!/bin/sh
rm disk.iso
dd if=/dev/zero of=disk.iso bs=1M count=40
mkfs.fat -F 32 disk.iso
