#!/bin/sh

USR=/usr
USR_SRC=/usr/src
INCLUDE_SYS=include/sys
DRIVERS=drivers
HOME=/home
START=/home/src
RELEASETOOLS=releasetools

# Copy poly_list.h 
cp ${HOME}/${INCLUDE_SYS}/poly_list.h ${USR}/${INCLUDE_SYS}/poly_list.h

# Copy poly_list
cp -r ${START}/${DRIVERS}/poly_list ${USR_SRC}/${DRIVERS}/poly_list