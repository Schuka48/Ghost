#! /bin/bash

if  [ $1 ];then
    if  sudo ls /run/netns | grep $1 > /dev/null
        then
        echo "This name already created!"
    else
        sudo ip netns add $1
        echo "You add netns with name $1!"
    fi
else
    echo "No netns name specified"
fi
