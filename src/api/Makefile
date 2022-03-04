CC=gcc
CFLAGS=-Wall -Wextra -g
CFILES=$(wildcard *.c)
OBJS=$(patsubst %.c, %.o, $(CFILES))

wbapi: wbapi.o test.o wpa_ctrl.o os_unix.o
	@rm -rf testd.o
	$(CC) $(CFLAGS) -o $@ wbapi.o test.o wpa_ctrl.o os_unix.o -lcrypto

debug: wbapi.o testd.o wpa_ctrl.o os_unix.o
	@rm -rf test.o
	$(CC) $(CFLAGS) -o wbapi wbapi.o testd.o wpa_ctrl.o os_unix.o -lcrypto

wbapi.o: wbapi.c
	$(CC) -c -I ./ -MMD -c -g -o $@ wbapi.c -lcrypto

testd.o: test.c
	$(CC) -c -I ./ -MMD -c -g -o $@ test.c -D DEBUG

wpa_ctrl.o: wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o $@ wpa_ctrl.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o $@ os_unix.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

%.o: %.c
	$(CC) $(CFLAGS) -I ./ -MMD -c $< -o $@

clean:
	@rm -rf *.o *.d wbapi
