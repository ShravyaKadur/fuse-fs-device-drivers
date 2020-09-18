#!/bin/sh

cd /var/tmp/replica
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Creating suc1 directory in master.."
mkdir master/suc1
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
rmdir master/suc1
