#! /bin/bash

if  [[ $1&&$2 ]];then
    sudo ip link add $1 type veth peer name $2
    echo "Link with names $1 and $2 are created successfully!"
else
    echo "Few arguments!"
fi


