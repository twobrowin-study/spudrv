#!/bin/bash

BAIKAL_SDK="/home/$USER/.baikal"
SSH_USER="$USER@leonhard"

##########################################
# Copy files to Leonhard server
##########################################

while [ -n "$1" ]
do
  case "$1" in
    "-d")
      DRIVER_NAME=$2
      echo "Copy Leonhard driver $DRIVER_NAME"
      scp $DRIVER_NAME/$DRIVER_NAME.ko $SSH_USER:~/tftpboot/
      ;;

    "-b")
      echo "Copy Baikal MIPS system files"
      scp $BAIKAL_SDK/img/bfkx.vmlinux.bin $SSH_USER:~/tftpboot/
      scp $BAIKAL_SDK/img/initrd.gz $SSH_USER:~/tftpboot/
      scp $BAIKAL_SDK/img/bfkx.dtb $SSH_USER:~/tftpboot/
      scp $BAIKAL_SDK/usr/x-tools/mipsel-unknown-linux-gnu/mipsel-unknown-linux-gnu/sysroot/lib/libstdc++.so.6.0.25 $SSH_USER:~/tftpboot/libstdc++.so.6
      ;;
  esac
  shift
done
