#!/bin/bash

DEV="enp0s8"
IP_addr=$2
broadcast=$3
gateway=$4
prefix=$5


#echo "dev: ${DEV}"
#echo "IP_addr: ${IP_addr}"
#echo "broadcast: ${broadcast}"
#echo "gateway: ${gateway}"
#echo "prefix: ${prefix}"

ip addr flush dev ${DEV}
ip addr add ${IP_addr}/${prefix} broadcast ${broadcast} dev ${DEV}
route add default gw ${gateway}
ip link set up ${DEV}
