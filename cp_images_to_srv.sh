#!/bin/bash

BAIKAL_SDK="/home/twobrowin/.baikal"
SSH_USER="twobrowin@leonhard"

DRIVER_NAME="spudrv.ko"
TEST_NAME="test04"

##########################################
# Копирование файлов системы Baikal MIPS
##########################################

# Копирование образа Linux
# scp $BAIKAL_SDK/img/bfkx.vmlinux.bin $SSH_USER:~/tftpboot/
# scp $BAIKAL_SDK/img/initrd.gz $SSH_USER:~/tftpboot/
# scp $BAIKAL_SDK/img/bfkx.dtb $SSH_USER:~/tftpboot/

# Копирование библиотеки C++
# scp $BAIKAL_SDK/usr/x-tools/mipsel-unknown-linux-gnu/mipsel-unknown-linux-gnu/sysroot/lib/libstdc++.so.6.0.25 $SSH_USER:~/tftpboot/libstdc++.so.6

# Копирование драйвера Leonhard
scp spudrv/$DRIVER_NAME $SSH_USER:~/tftpboot/

# Копирование файлов программ
scp libspu/$TEST_NAME $SSH_USER:~/tftpboot/

# Выполнение команды передачи прав на файлы в tftpboot пользователю nobody - Операция не позволена
# ssh $SSH_USER 'chown -R nobody: ~/tftpboot/'

##########################################
# Вывод подсказки
##########################################

echo "To download this files to Baikal, use following commands:"
echo "ifconfig eth1 192.168.1.3"
echo "tftp -g -r libstdc++.so.6 192.168.1.2"
echo "mv libstdc++.so.6 /lib/libstdc++.so.6"
echo "tftp -g -r $DRIVER_NAME 192.168.1.2"
echo "insmod $DRIVER_NAME"
echo "tftp -g -r $TEST_NAME 192.168.1.2"
echo "chmod +x $TEST_NAME"
echo
echo
echo "rmmod $DRIVER_NAME && tftp -g -r $DRIVER_NAME 192.168.1.2 && insmod $DRIVER_NAME"
echo "tftp -g -r $TEST_NAME 192.168.1.2 && ./$TEST_NAME"
