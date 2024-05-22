CXX = g++
#CXXFLAGS = -O3 -Wall -std=c++20 -march=native -fsanitize=address -g -fno-omit-frame-pointer
CXXFLAGS = -O3 -Wall -std=c++20 -march=native
LDFLAGS=-O3 -lm -latomic

SRCS = main.cpp atomicstamped.cpp queue.cpp graph.cpp tspfile.cpp threads_params.cpp thread_worker.cpp tools.cpp
OBJS = $(SRCS:.cpp=.o)

EXEC = main

all: $(EXEC) clean_o

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(EXEC)

clean_o:
	rm -f *.o



#all: main clean_o
#
#main: main.o
#	c++ -o main $(LDFLAGS) main.o
#
#%.o: %.cpp %.hpp
#	c++ $(CFLAGS) -c $< -o $@
#
#path.o: path.cpp path.hpp
#	c++ $(CFLAGS) -c path.cpp -o path.o
#
#main: main.o path.o threads_params.o
#	c++ -o main $(LDFLAGS) main.o threads_params.o
#
#testatom: testatom.cpp atomicstamped.hpp
#	g++ $(CFLAGS) -o testatom testatom.cpp
#
#testque: testque.cpp queue.hpp
#	g++ $(CFLAGS) -o testque testque.cpp
#
#omp:
#	make main CFLAGS="-fopenmp -O3" LDFLAGS="-fopenmp -O3"
#
#clean_o:
#	rm -f *.o
#
#clean:
#	rm -f *.o main atomic testatom omp testque
