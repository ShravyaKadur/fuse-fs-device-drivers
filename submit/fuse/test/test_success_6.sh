#!/bin/sh

cd /var/tmp/replica
mkdir master/suc6
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Deleting suc6 directory in master.."
rmdir master/suc6
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
