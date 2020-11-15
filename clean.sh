#!/bin/sh
set -e
. ./config.sh
 
for PROJECT in $PROJECTS; do
  (cd $PROJECT && $MAKE clean)
done
 
rm -rf sysroot
rm -rf isodir
rm -f os.iso
rm -f disk.iso
