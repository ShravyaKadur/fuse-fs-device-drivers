#!/bin/sh

cd /var/tmp/replica
touch master/suc9
echo "Initial permissions:"
ls -l master/suc9
ls -l slave/suc9
echo "Changing suc9 permissions in master.."
chmod 600 master/suc9
sleep 1
echo "Final permissions:"
ls -ld master/suc9
ls -ld slave/suc9
rm master/suc9
