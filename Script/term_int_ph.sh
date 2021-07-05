#! /bin/bash

if  [[ $1 ]];then

		sudo ip link set $1 master $2
		sudo ip link set dev $1 up

echo "Interface $2 connected to Bridge $1 successfully and set to UP!"
else
    echo "Few arguments!"
fi


