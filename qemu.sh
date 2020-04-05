#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-x86_64 -cdrom os.iso -M q35 -drive id=disk,file=disk.iso,if=none -device ich9-ahci,id=ahci -device ide-drive,drive=disk,bus=ahci.0 -boot d -no-reboot -no-shutdown $@
