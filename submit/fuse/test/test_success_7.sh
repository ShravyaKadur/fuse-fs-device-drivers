#!/bin/sh

cd /var/tmp/replica
touch master/suc7
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Deleting file suc7 in master.."
rm -f master/suc7
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
