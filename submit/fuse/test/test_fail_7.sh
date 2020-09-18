#!/bin/sh

cd /var/tmp/replica
touch master/old_file
echo "Contents of slave: "
ls -l slave/*_file
echo "Renaming old_file in slave.."
mv slave/old_file slave/new_file
echo "Contents of slave: "
ls -l slave/*_file
rm master/*_file
