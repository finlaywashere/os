#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-x86_64 -M q35 -cdrom os.iso -drive id=disk,file=disk.iso,if=none -device ich9-ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 -boot d -no-reboot -no-shutdown -nic model=e1000 $@ 
