#!/bin/bash

temp1=$1
output1=$2
temp2=$3
output2=$4
CURRENT=$5

while true
do 
CURRENT=$(date)

temp1=`i2cget -y 1 0x4A`
output1=$(echo "scale=2;((9/5) * $(($temp1))) + 32" |bc)


temp2=`i2cget -y 1 0x48`
output2=$(echo "scale=2;((9/5) * $(($temp2))) + 32" |bc)

printf "$CURRENT\n"
printf "Temprature reading from the left is: $output1 ˚F\n"
printf "Temprature reading from the right is: $output2 ˚F\n"
printf "\n"

sleep 1
done
