# sample cli processor

PROG   = cli
SRCS   = cli.c command.c

MAN  = cli.1

COPTS += -I../surf
LDADD += -lsurf
LDFLAGS += -L../surf

.include <bsd.prog.mk>

