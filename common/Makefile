# Makefile for 'common.a' module
#
# Dylan Buchanan, January 2023

CSL = ../libcs50/
OBJS = pagedir.o $(CSL)mem.o $(CSL)file.o $(CSL)webpage.o index.o word.o
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

pagedir.o: pagedir.h $(CSL)mem.h $(CSL)file.h $(CSL)webpage.h
$(CSL)mem.o: $(CSL)mem.h
$(CSL)file.o: $(CSL)file.h
$(CSL)webpage.o: $(CSL)webpage.h
index.o: index.h $(CSL)hashtable.h $(CSL)mem.h $(CSL)file.h $(CSL)counters.h
word.o: word.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f common.a
