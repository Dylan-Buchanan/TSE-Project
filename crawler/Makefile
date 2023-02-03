# Makefile for 'hashtable' module
#
# Dylan Buchanan, January 2023

C = crawler
OBJS = crawler.o
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

test: clean update crawler 
	./testing.sh

valgrind: update crawler
	valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../tse-output 2
# $(VALGRIND) ./testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core
	rm -rf ../tse-output/*
