#!/bin/sh

cd /var/tmp/replica
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Creating suc2 file in master.."
touch master/suc2
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
rm master/suc2
