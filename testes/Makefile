CC = gcc
CFLAGS = -m32 -Wall

LIB_DIR = ../lib

all: main

main: main.c $(LIB_DIR)/libcthread.a
	$(CC) $(CFLAGS) -o main main.c -L$(LIB_DIR) -lcthread

clean:
	-rm -f main