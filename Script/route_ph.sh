#! /bin/bash

sudo route add -net $1 gw $2
echo "Route $1 gw $2 is created!"
