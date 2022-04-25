# $NetBSD: $

.include <bsd.own.mk>

COPTS += -I${.CURDIR}/../../src/surf
LIBSURF != cd ${.CURDIR}/../../src/surf && ${PRINTOBJDIR}
DPADD += ${LIBSURF}/libsurf.a
LDADD += -L${LIBSURF}/ -lsurf

