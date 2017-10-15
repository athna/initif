#!/bin/bash

INPUT_PATH="ubuntu_initif_input.sh"
SHAPE_PATH="initif_shape.out"
SET_PATH="ubuntu_initif_set.sh"
dev_num=1

#compile
#gcc -Wall -o initif_shape.out initif_shape.c

#CONF=$1
CONF="initif.conf"
cat $CONF | while read line
do
    case ${line} in
        \#*)
        ;;
        eth*)
            input_data=`bash ${INPUT_PATH} ${line} ${dev_num};`
	    #echo ${input_data}
	    set_data=`./initif_shape.out ${input_data}`
	    #echo ${set_data}
	    bash ${SET_PATH} ${set_data}
	    dev_num=$(( dev_num + 1 ));
            ;;
        route*)
            Set_Route "${line}";
            ;;
    esac
done
