#! /bin/bash

sudo ip netns exec $1 route add -net $2 gw $3
echo "Route in NetNs $1 $2 gw $3 is created!"
