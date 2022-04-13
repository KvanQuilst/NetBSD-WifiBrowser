# Surf library makefile

.include <bsd.own.mk>

LIBISPRIVATE      = yes

LIB                 = surf
SRCS                = surf.c wpa_ctrl.c os_unix.c
LDADD               += -lcrypto
CPPFLAGS.wpa_ctrl.c += -MMD -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX
CPPFLAGS.os_unix.c  += -MMD -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX

.include <bsd.lib.mk>
