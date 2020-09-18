#!/bin/sh

cd /var/tmp/replica
touch master/fail9
echo "Writing to fail9 file in slave.."
echo "Test case for fail scenario #9" >> slave/fail9
echo "Contents of slave/fail9: "
cat slave/fail9
echo "Info:"
ls -l slave/fail9
rm master/fail9
