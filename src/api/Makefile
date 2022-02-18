CC=gcc
CFLAGS=-Wall -Wextra -g

wbapi: wbapi.c test.o os_unix.o wpa_ctrl.o sha1.o sha1-pbkdf2.o
	$(CC) $(CFLAGS) -o wbapi wbapi.c test.o os_unix.o wpa_ctrl.o 

debug: wbapi.c testd.o os_unix.o wpa_ctrl.o sha1.o sha1-pbkdf2.o
	$(CC) $(CFLAGS) -o wbapi wbapi.c testd.o os_unix.o wpa_ctrl.o

test.o: test.c
	$(CC) $(CFLAGS) -c -o test.o test.c

testd.o: test.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -g -o testd.o test.c -D DEBUG

wpa_ctrl.o: wpa_ctrl.c
	$(CC) -I ./ -MMD -c -g -o wpa_ctrl.o wpa_ctrl.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o os_unix.o os_unix.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

sha1.o: crypto/sha1.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o sha1.o crypto/sha1.c

sha1-pbkdf2.o: crypto/sha1-pbkdf2.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o sha1-pbkdf2.o crypto/sha1-pbkdf2.c

.PHONY clean:
clean:
	@rm -rf *.o *.d
