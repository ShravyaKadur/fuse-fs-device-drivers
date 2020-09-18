#!/bin/sh

cd /var/tmp/replica
echo "Writing to suc3 file in master.."
echo "Test case for success scenario #3" >> master/suc3
echo "Contents of master suc3: "
cat master/suc3
echo "Contents of slave suc3: "
cat slave/suc3
echo ""
echo "Compare sizes:"
ls -l master/suc3
ls -l slave/suc3
rm master/suc3
