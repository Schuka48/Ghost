#! /bin/bash

if  [[ $1&&$2&&$3 ]];then
    if sudo ls /run/netns | grep $1 > /dev/null
        then
		if sudo ip netns exec $1 ip link list | grep $3 > /dev/null
			then 
			sudo ip netns exec $1 ip addr add $2 dev $3

			echo "Ip addres $2 is added to dev $3 successfully!"
		else 
			echo "Incorrect interface!"
		fi
    else
        echo "Incorrect NetNs"
    fi
else
    echo "Few arguments!"
fi


