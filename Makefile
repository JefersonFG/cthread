CC = gcc
CFLAGS = -m32 -Wall
AR = ar
ARFLAGS = crs

LIB = lib/libcthread.a
CTHREAD = bin/cthread.o
SUPPORT = bin/support.o
SCHEDULER = bin/scheduler.o

CTHREAD_SRC = src/cthread.c
SCHEDULER_SRC = src/scheduler.c

HEADERS := $(wildcard include/*.h)

all: $(LIB)

$(SCHEDULER): $(SCHEDULER_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(CTHREAD): $(CTHREAD_SRC) $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB): $(CTHREAD) $(SCHEDULER)
	$(AR) $(ARFLAGS) $@ $(CTHREAD) $(SCHEDULER) $(SUPPORT)

clean:
	-rm -f $(CTHREAD)
	-rm -f $(SCHEDULER)
	-rm -f $(LIB)