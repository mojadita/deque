# Makefile -- makefile for test_deque, test program for multithread safe
# double ended queues.
# Author: Luis Colorado <luiscoloradourcola@gmail.com>
# Date: Wed Mar  8 18:30:08 EET 2017

targets = test_deque
TOCLEAN = $(targets)
CFLAGS += -pthread

.PHONY: all clean

all: $(targets)
clean:
	$(RM) $(TOCLEAN)
.depend:
	$(CC) -MM $(sources) >$@

test_deque_objs = test_deque.o deque.o
test_deque_ldflags = -pthread
TOCLEAN += $(test_deque_objs)
sources += $(test_deque_objs:.o=.c)

test_deque: $(test_deque_objs)
	$(CC) $(LDFLAGS) -o $@ $($@_objs) $($@_libs) $($@_ldflags)

-include .depend
