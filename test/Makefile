# Surf_Test Makefile

DEBUG ?= 0
.if ${DEBUG} == "1"
COPTS += -DDEBUG
.endif

PROG = surf
SRCS = surf_test.c
SUBDIR += ../src/surf

COPTS += -I../src/surf
LDADD += -lsurf
LDFLAGS += -L../src/surf

MAN =

.include <bsd.prog.mk>
