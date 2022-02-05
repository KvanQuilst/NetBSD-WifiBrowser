CC=gcc
CFLAGS=-Wall -Wextra -g
LDFLAGS=

#CLI Compilation
cli: cli.o commands.o handle_commands.o
	$(CC) -o cli cli.o commands.o handle_commands.o $(LDFLAGS)
cli.o: cli.c cli.h
	$(CC) $(CFLAGS) -c cli.c
commands.o: commands.c commands.h commands_def.h
	$(CC) $(CFLAGS) -c commands.c 
handle_commands.o: handle_commands.c handle_commands.h
	$(CC) $(CFLAGS) -c handle_commands.c

#Clean Files
clean:
	rm -rf *.o *.d












