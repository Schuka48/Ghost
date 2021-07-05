#!/bin/bash

if [ $1 ]; then

	sudo ip link add $1 type bridge
	echo "Master Bridge $1 is created successfully!"

else

	echo "No Bridge or $1 is created!"

fi	
