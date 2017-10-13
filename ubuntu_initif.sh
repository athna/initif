#!/bin/bash

INPUT="ubuntu_initif_input.sh"
SHAPE="initif_shape.out"
SET="ubuntu_initif_set.sh"

#compile
gcc -Wall -o init_shape.out initif_shape.c

#CONF=$1
CONF="initif.conf"
cat $CONF | while read line
do
    case ${line} in
        \#*)
        ;;
        eth*)
             bash ${INPUT} ${line};
        ;;
        enp0*)
             bash ${INPUT} ${line};
        ;;
        route*)
            Set_Route "${line}";
        ;;
    esac
done
./init_shape.out initif_input.txt

