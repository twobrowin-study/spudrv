#!/bin/bash

DRIVER_NAME="$1".ko
TEST_NAME="$2"

##########################################
# Print help
##########################################

echo
echo "****************************************"
echo "To download this files to Baikal, use:"
echo "****************************************"
echo
echo "ifconfig eth1 192.168.1.3"
echo "tftp -g -r libstdc++.so.6 192.168.1.2"
echo "mv libstdc++.so.6 /lib/libstdc++.so.6"
echo "tftp -g -r $DRIVER_NAME 192.168.1.2"
echo "insmod $DRIVER_NAME"
echo "tftp -g -r $TEST_NAME 192.168.1.2"
echo "chmod +x $TEST_NAME"
echo
echo
echo "****************************************"
echo "To update this files on Baikal, use:"
echo "****************************************"
echo
echo "rmmod $DRIVER_NAME && tftp -g -r $DRIVER_NAME 192.168.1.2 && insmod $DRIVER_NAME"
echo "tftp -g -r $TEST_NAME 192.168.1.2 && ./$TEST_NAME"
echo
