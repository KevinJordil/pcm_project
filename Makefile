#  Copyright (c) 2012 Marcelo Pasin. All rights reserved.

CFLAGS=-O3 -Wall --std=c++20
LDFLAGS=-O3 -lm

all: main

main: main.o
	c++ -o main $(LDFLAGS) main.o

%.o: %.cpp %.hpp
	c++ $(CFLAGS) -c $< -o $@

main: main.o path.o threads_params.o
	c++ -o main $(LDFLAGS) main.o threads_params.o

testatom: testatom.cpp atomicstamped.hpp
	g++ $(CFLAGS) -o testatom testatom.cpp

testque: testque.cpp queue.hpp
	g++ $(CFLAGS) -o testque testque.cpp

omp:
	make main CFLAGS="-fopenmp -O3" LDFLAGS="-fopenmp -O3"

clean:
	rm -f *.o main atomic testatom omp testque
