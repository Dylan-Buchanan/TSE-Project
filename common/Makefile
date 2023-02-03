# Makefile for 'hashtable' module
#
# Dylan Buchanan, January 2023

OBJS = pagedir.o ../libcs50/mem.o ../libcs50/file.o
LIB = common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h ../libcs50/mem.h ../libcs50/file.h
../libcs50/mem.o: ../libcs50/mem.h
../libcs50/file.o: ../libcs50/file.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f crawler
	rm -f core
	rm -f common.a
