#!/bin/sh

cd /var/tmp/replica
touch master/fail8
echo "Initial permissions:"
ls -l slave/fail8
echo "Changing fail8 permissions in master.."
chmod 600 slave/fail8
echo "Final permissions:"
ls -l slave/fail8
rm master/fail8
