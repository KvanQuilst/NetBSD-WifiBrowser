CC=gcc
CFLAGS=-Wall -Wextra -g

wbapi: wbapi.c test.o os_unix.o wpa_ctrl.o sha1.o sha1-internal.o
	$(CC) $(CFLAGS) -Wall -Wextra -o wbapi wbapi.c test.o os_unix.o \
		wpa_ctrl.o sha1.o sha1-internal.o

debug: wbapi.c testd.o os_unix.o wpa_ctrl.o
	$(CC) $(CFLAGS) -Wall -Wextra -o wbapi wbapi.c testd.o os_unix.o wpa_ctrl.o

testd.o: test.c
	$(CC) -c -I ./ -MMD -c -g -o testd.o test.c -D DEBUG

wpa_ctrl.o: wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o wpa_ctrl.o wpa_ctrl.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o os_unix.o os_unix.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

%.o: %.c
	$(CC) $(CFLAGS) -I ./ -MMD -c $< -o $@

%.o: crypto/%.c
	$(CC) $(CFLAGS) -I ./ -MMD -c $< -o $@

.PHONY clean:
clean:
	@rm -rf *.o *.d
