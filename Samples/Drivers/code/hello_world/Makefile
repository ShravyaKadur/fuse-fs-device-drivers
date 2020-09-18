# Makefile for the hello_world driver.
PROG=   hello_world
SRCS=   hello_world.c
 
FILES=${PROG}.conf
FILESNAME=${PROG}
FILESDIR= /etc/system.conf.d
 
DPADD+= ${LIBCHARDRIVER} ${LIBSYS}
LDADD+= -lchardriver -lsys
 
MAN=
 
.include <minix.service.mk>