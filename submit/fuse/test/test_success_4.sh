#!/bin/sh

cd /var/tmp/replica
mkdir master/old_dir
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
echo "Renaming old_dir directory in master.."
mv master/old_dir master/new_dir
echo "Contents of master: "
ls master/
echo "Contents of slave: "
ls slave/
rmdir master/*_dir
