#!/bin/sh

cd /var/tmp/replica
touch master/fail6
echo "Contents of slave: "
ls -l slave/fail6
echo "Deleting fail6 file in slave.."
rm slave/fail6
echo "Contents of slave: "
ls -l slave/fail6
rm master/fail6
