#!/bin/bash

INPUT_FILE="initif_input.txt"
DEV=$1
IPaddr=$2

#Ubuntu
MACaddr=`ifconfig  | grep  HWaddr | sed -E "s@.*HWaddr\s(\S+)(\s.*|$)@\1@g"`

touch ${INPUT_FILE}
echo ${DEV}>${INPUT_FILE}
echo ${IPaddr}>>${INPUT_FILE}
echo ${MACaddr}>>${INPUT_FILE}