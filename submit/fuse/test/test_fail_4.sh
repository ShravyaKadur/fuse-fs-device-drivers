#!/bin/sh

cd /var/tmp/replica
mkdir master/fail4
echo "Initial permissions:"
ls -ld slave/fail4
echo "Changing fail4 permissions in master.."
chmod 777 slave/fail4
echo "Final permissions:"
ls -ld slave/fail4
rmdir master/fail4
