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

#Clean Files
clean:
	rm -rf $(BINARY) $(OBJECTS)












