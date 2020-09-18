#!/bin/sh

cd /var/tmp/replica
mkdir master/fail2
echo "Contents of slave: "
ls -ld slave/fail2
echo "Deleting fail2 directory in slave.."
rmdir slave/fail2
echo "Contents of slave: "
ls -ld slave/fail2
rmdir master/fail2
