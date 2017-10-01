CC = gcc
CFLAGS = -m32 -Wall
AR = ar
ARFLAGS = crs

TARGET = lib/libcthread.a
OBJ = bin/cthread.o
SUPPORT = bin/support.o

HEADERS = $(wildcard include/*.h)
SRC = $(wildcard src/*.c)

all: $(OBJ) $(TARGET)

$(OBJ): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ) $(SUPPORT)

clean:
	-rm -f $(OBJ)
	-rm -f $(TARGET)