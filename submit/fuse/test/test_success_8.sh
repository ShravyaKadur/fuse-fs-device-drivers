#!/bin/sh

cd /var/tmp/replica
mkdir master/suc8
echo "Initial permissions:"
ls -ld master/suc8
ls -ld slave/suc8
echo "Changing suc8 permissions in master.."
chmod 777 master/suc8
sleep 1
echo "Final permissions:"
ls -ld master/suc8
ls -ld slave/suc8
rmdir master/suc8
