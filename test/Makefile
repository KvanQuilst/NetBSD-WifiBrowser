CC=gcc
LFLAGS=-L ../src/surf -lsurf
INCLUDE=-I ../src/surf
OFLAG=-o surf

api_test: surf_test.c
	$(CC) $(OFLAG) surf_test.c $(INCLUDE) $(LFLAGS)

debug: surf_test.c
	$(CC) $(OFLAG) surf_test.c -D DEBUG $(INCLUDE) $(LFLAGS)
