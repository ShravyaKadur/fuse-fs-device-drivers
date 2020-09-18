#!/bin/bash

touch replica_fs.log

if [ -d mount_dir ]; then
	rm -rf mount_dir/*
else
	mkdir mount_dir
fi

if [ ! -d /var/tmp/replica ]; then
	mkdir /var/tmp/replica
fi

if [ $1 == "0" ]; then
	python3 replica_fs.py ./mount_dir /var/tmp/replica
else
	python3 replica_fs_bonus.py ./mount_dir /var/tmp/replica $1
fi

