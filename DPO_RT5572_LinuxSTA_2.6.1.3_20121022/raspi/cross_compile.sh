#!/bin/sh

if [ $# != 3 ]
then
  echo "Usage: $0 CHIP CC_PREFIX LINUX_SRC"
  echo "Example: $0 3070 arm-linux-gnueabihf- /media/root/967cec13-9afa-45fb-b861-cb3255f6275d/rpi_kernel_23.03.2015/linux"
  exit 2
fi

#checking if cross compiler found
if which $2gcc > /dev/null
then
  echo "Crosscompiler found"
else
  echo "cross compiler with prefix $2 not found in your \$PATH. Do you need something like \"PATH=\$PATH:/home/ahmed/raspi_toolchains/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin\"?"
  exit 2
fi

chips="2770 3070 5370"
for chip in $chips
do
  if [ $1 -eq $chip ]
  then
    target_chip=$chip;
    break
  fi
done

if test -z "$target_chip"
then
  echo "Error, specify chipset in first parameter, only this chips are supported: $chips"
  exit 1
fi

FILES="include/os/rt_drv.h include/os/rt_linux.h include/rtmp_def.h common/rtusb_dev_id.c include/os/rt_linux_cmm.h"
for f in $FILES; do
  mv ../$f ../${f}_org
  ln -s `pwd`/$target_chip/$f ../$f
done

cd ..
CROSS_COMPILE=$2 LINUX_SRC=$3 make -j4

cd -
for f in $FILES; do
  rm ../$f
  mv ../${f}_org ../$f
done