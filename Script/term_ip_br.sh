#!/bin/bash

if [[ $1&&$2 ]]; then

	sudo ip addr add $1 dev $2
	echo "IP address assigned successfully"

else

	echo "Wrong arguments!"

fi

