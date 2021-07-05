#!/bin/bash

	sudo brctl addbr br0
	sudo brctl addif br0 ens33
	sudo ip addr add $1 dev br0
	sudo ip link set dev br0 up
	sudo ip addr del $1 dev ens33

	echo "Bridge br0 is created successufully!"
