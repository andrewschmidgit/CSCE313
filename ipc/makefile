# makefile

all: dataserver client

BoundedBuffer.o: BoundedBuffer.h BoundedBuffer.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c BoundedBuffer.cpp

Histogram.o: Histogram.h Histogram.cpp
	g++ -g -w -Wall -O1 -std=c++11 -c Histogram.cpp

dataserver: dataserver.cpp
	g++ -g -w -Wall -O1 -std=c++11 -o dataserver dataserver.cpp -lpthread -lrt

client: client.cpp BoundedBuffer.o Histogram.o
	g++ -g -w -Wall -O1 -std=c++11 -o client client.cpp BoundedBuffer.o Histogram.o -lpthread -lrt

clean:
	rm -rf *.o fifo* dataserver client
