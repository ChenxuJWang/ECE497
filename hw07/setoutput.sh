#!/bin/sh
cd /sys/class/gpio
echo 97 >> export
cd gpio97
echo out >> direction
