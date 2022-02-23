CC=gcc
CFLAGS=-Wall -Wextra -g
ODIR=obs
CFILES=$(wildcard *.c)
OBJS=$(patsubst %.c, $(ODIR)/%.o, $(CFILES))

wbapi: $(ODIR) $(OBJS)
	@rm -rf $(ODIR)/testd.o
	$(CC) $(CFLAGS) -o $@ $(ODIR)/*.o -lcrypto

debug: $(ODIR) $(OBJS) $(ODIR)/testd.o
	@rm -rf $(ODIR)/test.o
	$(CC) $(CFLAGS) -Wall -Wextra -o wbapi $(ODIR)/*.o

$(ODIR)/wbapi.o: wbapi.c
	$(CC) -c -I ./ -MMD -c -g -o $@ $< -lcrypto

$(ODIR)/testd.o: test.c
	$(CC) -c -I ./ -MMD -c -g -o $@ $< -D DEBUG

$(ODIR)/wpa_ctrl.o: wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o $@ $< -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

$(ODIR)/os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o $@ $< -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -I ./ -MMD -c $< -o $@

$(ODIR):
	@mkdir $@

.PHONY clean:
clean:
	@rm -rf $(ODIR)/*.o $(ODIR)/*.d
