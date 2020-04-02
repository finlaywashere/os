#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-x86_64 -cdrom os.iso -hda disk.iso -boot d -no-reboot -no-shutdown $@
