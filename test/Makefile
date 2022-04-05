CC=gcc
LFLAGS=-L ../src/api -lsurf
INCLUDE=-I ../src/api
OFLAG=-o api

api_test: api_test.c
	$(CC) $(OFLAG) api_test.c $(INCLUDE) $(LFLAGS)

debug: api_test.c
	$(CC) $(OFLAG) api_test.c -D DEBUG $(INCLUDE) $(LFLAGS)
