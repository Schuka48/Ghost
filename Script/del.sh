#!/bin/bash


sudo ip -all netns delete
for (( i = 1; i <= $#; i++ ))
do
sudo ip link del dev '$i'
done
