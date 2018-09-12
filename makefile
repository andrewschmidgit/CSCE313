# makefile

CFLAGS=-g -std=c++11

all: memtest

Ackerman.o: Ackerman.cpp 
	g++ $(CFLAGS) -c Ackerman.cpp

BuddyAllocator.o : BuddyAllocator.cpp
	g++ $(CFLAGS) -c BuddyAllocator.cpp

Main.o : Main.cpp
	g++ $(CFLAGS) -c Main.cpp

memtest: Main.o Ackerman.o BuddyAllocator.o
	g++ $(CFLAGS) -o memtest Main.o Ackerman.o BuddyAllocator.o

clean:
	rm -f *.o memtest