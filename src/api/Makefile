CC=gcc
CFLAGS=-Wall -Wextra -g
OUT=surf

surf: $(OUT).o wpa_ctrl.o os_unix.o
	@rm -f lib$(OUT).a
	ar -cq lib$(OUT).a $(OUT).o wpa_ctrl.o os_unix.o

$(OUT).o: $(OUT).c
	$(CC) -c -I ./ -MMD -c -g -o $@ $(OUT).c -lcrypto

wpa_ctrl.o: wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o $@ wpa_ctrl.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o $@ os_unix.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

clean:
	@rm -rf *.o *.d libsurf.a
