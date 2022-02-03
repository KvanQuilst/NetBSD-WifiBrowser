CC=gcc
CFLAGS=-Wall -Wextra -g
LDFLAGS=

#TUI Compilation
tui: tui.o
	$(CC) -o tui tui.o $(LDFLAGS)
tui.o: . ../src/tui/tui.c
	$(CC) $(CFLAGS) -c . ../src/tui/tui.c

#CLI Compilation
cli: cli.o commands.o 
	$(CC) -o cli cli.o commands.o $(LDFLAGS)
cli.o: . ../src/cli/cli.c ../src/cli/cli.h
	$(CC) $(CFLAGS) -c . ../src/cli/cli.c
commands.o: . ../src/cli/commands.c ../src/cli/commands.h ../src/cli/commands_def.h
	$(CC) $(CFLAGS) -c . ../src/cli/commands.c 

wbapi: ../src/api/test.c ../src/api/wbapi.c os_unix.o wpa_ctrl.o
	$(CC) $(CFLAGS) -Wall -Wextra -o wbapi ../src/api/test.c ../src/api/wbapi.c os_unix.o wpa_ctrl.o 
wpa_ctrl.o: ../src/api/wpa_ctrl.c
	$(CC) -c -I ./ -MMD -c -g -o ../src/api/wpa_ctrl.o ../src/api/wpa_ctrl.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX
os_unix.o: ../src/api/os_unix.c
	$(CC) $(CFLAGS) -I ./ -MMD -c -o ../src/api/os_unix.o ../src/api/os_unix.c -D CONFIG_CTRL_IFACE -D CONFIG_CTRL_IFACE_UNIX

#Clean Files
clean:
	rm -rf *.o












