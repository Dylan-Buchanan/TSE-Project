# Makefile for 'querier' module
#
# Dylan Buchanan, January 2023

C = querier
OBJS = querier.o
LIBS = ../common/common.a ../libcs50/libcs50-given.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(C): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

.PHONY: test valgrind clean

update:
	make -C ../common

test: clean update querier
	./testing.sh

valgrind: $(C)
	$(VALGRIND) ./querier ../tse-output/wikipedia-1 ../tse-output/wikipedia-1-index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f querier
	rm -f core
