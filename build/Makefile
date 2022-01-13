CC=gcc
CFLAGS=-Wall -g
LDFLAGS=

#API Compilation
all: wbapi
wbapi: wbapi.o
	$(CC) -o wbapi wbapi.o $(LDFLAGS)
wbapi.o: . ../src/api/wbapi.c
	$(CC) $(CFLAGS) -c . ../src/api/wbapi.c

#TUI Compilation
tui: tui
tui: tui.o
	$(CC) -o tui tui.o $(LDFLAGS)
tui.o: . ../src/tui/tui.c
	$(CC) $(CFLAGS) -c . ../src/tui/tui.c

#CLI Compilation
cli: cli
cli: cli.o
	$(CC) -o cli cli.o $(LDFLAGS)
cli.o: . ../src/cli/cli.c
	$(CC) $(CFLAGS) -c . ../src/cli/cli.c

#Compile and Build
#$(BINARY): $(OBJECTS)
#	$(CC) -o $(BINARY) $(OBJECTS) $(LDFLAGS)
#$(OBJECTS): $(CFILES)
#	$(CC) $(CFLAGS) -c $(CFILES)

#Clean Files
clean:
	rm -rf $(BINARY) $(OBJECTS)












