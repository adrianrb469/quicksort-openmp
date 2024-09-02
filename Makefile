CC=g++
CFLAGS=-std=c++11 -Wall -O3
OMPFLAGS=-fopenmp

all: sequential parallel

sequential: src/sequential.cpp
	$(CC) $(CFLAGS) src/sequential.cpp -o bin/sequential

parallel: src/parallel.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) src/parallel.cpp -o bin/parallel

clean:
	rm -f bin/sequential bin/parallel