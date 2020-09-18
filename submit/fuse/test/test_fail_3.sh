#!/bin/sh

cd /var/tmp/replica
mkdir master/old_dir
echo "Contents of slave: "
ls -ld slave/*_dir
echo "Renaming old_dir directory in slave.."
mv slave/old_dir slave/new_dir
echo "Contents of slave: "
ls -ld slave/*_dir
rmdir master/*_dir
