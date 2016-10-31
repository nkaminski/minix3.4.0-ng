# Makefile for mcast server proc
PROG=	mcast
SRCS=	main.c groups.c reply.c table.c utility.c deadlock.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

.include <minix.service.mk>
