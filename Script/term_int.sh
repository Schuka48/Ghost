#! /bin/bash

if  [[ $1&&$2 ]];then

		sudo ip link set $2 netns $1
		sudo ip netns exec $1 ip link set dev $2 up

echo " Interface dev $2 is added to NetNs $1 and set to UP!"

else
    echo "Few arguments!"
fi


