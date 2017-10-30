#!/bin/bash

DEV=$1
IP_addr=$2
dev_num=$3
MAC_addr_begin=`echo "1 + 18 * ( ${dev_num} - 1 )" | bc`
MAC_addr_end=`echo "${MAC_addr_begin} + 16" | bc`

#Ubuntu
MAC_addr_all=`ifconfig  | grep  HWaddr | sed -E "s@.*HWaddr\s(\S+)(\s.*|$)@\1@g"`
MAC_addr=`echo ${MAC_addr_all} | cut -c ${MAC_addr_begin}-${MAC_addr_end}`

echo "${DEV} ${IP_addr} ${MAC_addr}"
