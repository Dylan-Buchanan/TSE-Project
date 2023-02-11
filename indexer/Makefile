# Makefile for 'crawler' module
#
# Dylan Buchanan, January 2023

C = indexer
OBJS = indexer.o
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

indextest: indextest.o
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


.PHONY: test valgrind clean

update:
	make -C ../common

test: clean update indexer indextest
	bash -v testing.sh

valgrind: indexer indextest
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../tse-output/wikipedia-1 ../tse-output/wikipedia-1-index
	valgrind --leak-check=full --show-leak-kinds=all ./indextest ../tse-output/wikipedia-1-index ../tse-output/wikipedia-1-inverse

valindexer: indexer
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../tse-output/wikipedia-1 ../tse-output/wikipedia-1-index

valindextest: indextest
	valgrind --leak-check=full --show-leak-kinds=all ./indextest ../tse-output/wikipedia-1-index ../tse-output/wikipedia-1-inverse
# $(VALGRIND) ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f indexer
	rm -f indextest
	rm -f ../tse-output/wikipedia-1-index
	rm -f ../tse-output/wikipedia-1-inverse
	rm -f core
