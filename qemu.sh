#!/bin/sh
set -e
. ./iso.sh
 
qemu-system-i386 -cdrom os.iso -hda disk.iso -boot d $@
