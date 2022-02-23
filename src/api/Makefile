CC=gcc
CFLAGS=-Wall -Wextra -g
ODIR=obs
CFILES=$(wildcard *.c)
OBJS=$(patsubst %.c, $(ODIR)/%.o, $(CFILES))

wbapi: $(ODIR) $(OBJS)
	$(CC) $(CFLAGS) -Wall -Wextra -o $@ $(ODIR)/*.o

debug: $(ODIR) $(OBJS) $(ODIR)/testd.o
	@rm -rf $(ODIR)/test.o
	$(CC) $(CFLAGS) -Wall -Wextra -o wbapi $(ODIR)/*.o

$(ODIR)/testd.o: test.c
	$(CC) -c -I ./ -MMD -c -g -o $@ $< -D DEBUG

$(ODIR)/wpa_ctrl.o: wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o $@ $< -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

$(ODIR)/os_unix.o: os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o $@ $< -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) -I ./ -MMD -c $< -o $@

$(ODIR)/%.o: crypto/%.c
	$(CC) $(CFLAGS) -I ./ -I crypto/ -MMD -c $< -o $@

$(ODIR):
	@mkdir $@

.PHONY clean:
clean:
	@rm -rf *.o *.d
