# $NetBSD: $

.include <bsd.own.mk>

COPTS += -I${.CURDIR}/../surf
LIBSURF != cd ${.CURDIR}/../surf && ${PRINTOBJDIR}
DPADD += ${LIBSURF}/libsurf.a
LDADD += -L${LIBSURF}/ -lsurf

