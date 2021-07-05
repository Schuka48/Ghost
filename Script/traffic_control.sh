#!/bin/bash

sudo tc qdisc add dev $1 root netem delay $2ms $3ms loss $4% $5% reorder $6% $7% rate $8kbit corrupt $9% 
