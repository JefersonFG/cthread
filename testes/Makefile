CC = gcc
CFLAGS = -m32 -Wall

LIB_DIR = ../lib

all: main main2 main3

main: main.c $(LIB_DIR)/libcthread.a
	$(CC) $(CFLAGS) -o main main.c -L$(LIB_DIR) -lcthread
      
main2: main2.c $(LIB_DIR)/libcthread.a
	$(CC) $(CFLAGS) -o main2 main2.c -L$(LIB_DIR) -lcthread

main3: main3.c $(LIB_DIR)/libcthread.a
	$(CC) $(CFLAGS) -o main3 main3.c -L$(LIB_DIR) -lcthread 

clean:
	-rm -f main main2 main3
