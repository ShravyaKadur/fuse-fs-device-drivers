#!/bin/sh

cd /var/tmp/replica
touch master/old_file
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Renaming old_file in master.."
mv master/old_file master/new_file
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
rm master/*_file
