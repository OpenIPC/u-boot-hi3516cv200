#!/bin/sh -e

export ARCH=arm
export CROSS_COMPILE=${CROSS_COMPILE:-arm-hisiv510-linux-}

OUTPUTDIR="${HOME}/src/hisi/uboot"
SOCS="hi3518ev200 hi3516cv200"

for soc in ${SOCS};do

make clean
make ${soc}_config
cp reg_info_${soc}.bin .reg
make  -j 8
make mini-boot.bin
cp mini-boot.bin u-boot-${soc}.bin

cp u-boot-${soc}.bin ${OUTPUTDIR}/u-boot-${soc}-universal.bin
#cp u-boot-${soc}.bin /srv/tftp/u-boot-${soc}-universal.bin

done
