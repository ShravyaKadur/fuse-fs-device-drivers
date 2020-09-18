#!/bin/sh

ls_output=$(ls -l /dev/)

echo "$ls_output" | grep "," | awk '{print $5}' | tr "," " " | sort -n | uniq