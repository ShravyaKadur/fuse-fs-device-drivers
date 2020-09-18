#!/bin/sh

cd /var/tmp/replica
echo "Creating fail5 file in slave.."
touch slave/fail5
echo "Contents of slave: "
ls -l slave/
